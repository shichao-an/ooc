#include <assert.h>
#include <ctype.h>
#include <errno.h>
#include <math.h>
#include <setjmp.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "parse.h"
#include "Node.h"
#include "Symbol.h"
#include "Symtab.h"

#ifndef	SYMTABFILE
#define	SYMTABFILE	"value.stb"
#endif
#ifndef	SYMBOLFILE
#define	SYMBOLFILE	"%s.sym"
#endif

/*
 *	reserved words
 */

static void * table;		/* symbol table */

enum tokens {				/* must not clash with operators, parameter */
	NUMBER = 'n',			/* literal constant */
	VAR = 'v',				/* variable name */
	CONST = 'c',				/* constant name */
	LET = 'l',				/* LET VAR = sum */
	MATH = 'm',				/* library function name */
	DEF = 'd',				/* DEF FUN = sum */
	FUN = 'f',				/* user function name */
	LOAD = 'L',				/* LOAD FUN */
	SAVE = 'S',				/* SAVE FUN */
	UNDEF = 'u'				/* undefined name */
};

static void initNames (void)
{	static const struct {
		const char * name;
		int lex;
	} names [] = {
		{ "def",	DEF },
		{ "let",	LET },
		{ "load",	LOAD },
		{ "save",	SAVE },
	{ 0 }}, * np = names;

	while (np -> name)
		install(table, new(Reserved(), np -> name, np -> lex)),
		++ np;
}

static void initConst (void)
{	static const struct {
		const char * name;
		double value;
	} constants [] = {
		{ "PI",		3.14159265358979323846 },
		{ "E",		2.71828182845904523536 },
		{ "GAMMA",	0.57721566490153286060 },	/* Euler */
		{ "DEG",	57.29577951308232087680 },	/* deg/radian */
		{ "PHI",	1.61803398874989484820 },	/* golden ratio */
	{ 0 }}, * cp = constants;

	while (cp -> name)
		install(table, new(Const(), cp -> name, CONST, cp -> value)),
		++ cp;
}

static void initMath (void)
{	static const struct {
		const char * name;
		function lib;
	} functions [] = {
		{ "sin",   sin },
		{ "cos",   cos },
		{ "tan",   tan },
		{ "asin",  asin },
		{ "acos",  acos },
		{ "atan",  atan },
		{ "sinh",  sinh },
		{ "cosh",  cosh },
		{ "tanh",  tanh },
		{ "exp",   exp },
		{ "log",   log },
		{ "log10", log10 },
		{ "sqrt",  sqrt },
		{ "ceil",  ceil },
		{ "floor", floor },
		{ "abs",   fabs },
	{ 0 }}, * fp = functions;

	while (fp -> name)
		install(table, new(Math(), fp -> name, MATH, fp -> lib)),
		++ fp;
}

/*
 *	symbol transput
 */

int putsymbol (const void * sym, FILE * fp)
{
	return fprintf(fp, "\tname %s\n\tlex %d\n",
									name(sym), lex(sym));
}

void * getsymbol (const void * class, FILE * fp)
{	char buf [BUFSIZ];
	int token;
	void * result;

	if (fscanf(fp, "\tname %s\n\tlex %d\n", buf, & token) != 2)
		assert(0);
	result = screen(table, buf, UNDEF);
	if (lex(result) == UNDEF)
		install(table, result =
							new(class, name(result), token));
	else if (lex(result) != token)
	{	fclose(fp);
		error("%s: need a %s, got a %s",
				buf, nameOf(class), nameOf(classOf(result)));
	}
	return result;
}

/*
 *	scanner
 */

#define ALNUM	"ABCDEFGHIJKLMNOPQRSTUVWXYZ" \
				"abcdefghijklmnopqrstuvwxyz" \
				"_" "0123456789"

static enum tokens token;	/* current input symbol */
static double number;		/* if NUMBER: numerical value */
static void * symbol;		/* else: symbol */

static enum tokens scan (const char * buf)
					/* return token = next input symbol */
{	static const char * bp;

	if (buf)
		bp = buf;			/* new input line */

	while (isspace(* bp & 0xff))
		++ bp;
	if (isdigit(* bp & 0xff) || * bp == '.')
	{	errno = 0;
		token = NUMBER, number = strtod(bp, (char **) & bp);
		if (errno == ERANGE)
			error("bad value: %s", strerror(errno));
	}
	else if (isalpha(* bp & 0xff) || * bp == '_')
	{	char buf [BUFSIZ];
		int len = strspn(bp, ALNUM);

		if (len >= BUFSIZ)
			error("name too long: %-.10s...", bp);

		strncpy(buf, bp, len), buf[len] = '\0', bp += len;
		symbol = screen(table, buf, UNDEF), token = lex(symbol);
	}
	else
		token = * bp ? * bp ++ : 0;
	return token;
}

/*
 *	factor : + factor
 *			 - factor
 *			 NUMBER
 *			 CONST
 *			 VAR
 *			 MATH ( sum )
 *			 FUN ( sum )
 *			 $
 *			 ( sum )
 */

static void * sum (void);
static void * parameter;	/* inside DEF: parameter symbol */

static void * factor (void)
{	void * result;

	switch ((int)token) {
	case '+':
		scan(0);
		return factor();
	case '-':
		scan(0);
		return new(Minus(), factor());
	default:
		error("bad factor: '%c' 0x%x", token, token);
	case NUMBER:
		result = new(Number(), number);
		break;
	case CONST:
	case VAR:
		result = new(Global(), symbol);
		break;
	case MATH:
	{	const void * fp = symbol;

		if (scan(0) != '(')
			error("expecting (");
		scan(0);
		result = new(Builtin(), fp, sum());
		if (token != ')')
			error("expecting )");
		break;
	}
	case FUN:
	{	const void * fp = symbol;

		if (scan(0) != '(')
			error("expecting (");
		scan(0);
		result = new(User(), fp, sum());
		if (token != ')')
			error("expecting )");
		break;
	}
	case '$':
		if (! parameter)
			error("illegal use of parameter symbol");
		result = new(Parm(), parameter);
		break;
	case '(':
		scan(0);
		result = sum();
		if (token != ')')
			error("expecting )");
	}
	scan(0);
	return result;
}

/*
 *	product : factor { *|/ factor }...
 */

static void * product (void)
{	void * result = factor();
	const void * type;

	for (;;)
	{	switch ((int)token) {
		case '*':
			type = Mult();
			break;
		case '/':
			type = Div();
			break;
		default:
			return result;
		}
		scan(0);
		result = new(type, result, factor());
	}
}

/*
 *	sum : product { +|- product }...
 */

static void * sum (void)
{	void * result = product();
	const void * type;

	for (;;)
	{	switch ((int)token) {
		case '+':
			type = Add();
			break;
		case '-':
			type = Sub();
			break;
		default:
			return result;
		}
		scan(0);
		result = new(type, result, product());
	}
}

/*
 *	stmt : let VAR = sum
 *		   def FUN = sum
 *		   load [symbol...]
 *		   save [symbol...]
 *		   sum
 */

static void * stmt (void)
{	void * sym, * node;

	parameter = 0;
	switch (token) {
	case LET:
		switch (scan(0)) {
		case UNDEF:
		case VAR:
			sym = symbol;
			if (scan(0) != '=')
				error("expecting =");
			scan(0);
			node = sum();
			if (! token)
			{	if (lex(sym) == UNDEF)
				{	sym = new(Var(), name(sym), VAR);
					install(table, sym);
				}
				return new(Assign(), sym, node);
			}
		default:
			error("bad assignment");
		}
	case DEF:
		scan(0);
		sym = symbol;
		switch (token) {
		case UNDEF:
			sym = new(Fun(), name(sym), FUN);
			install(table, sym);
		case FUN:
			parameter = sym;
			if (scan(0) != '=')
				error("expecting =");
			scan(0);
			node = sum();
			if (! token)
			{	setfun(sym, node);
				reclaim(Node(), sunder);
				return 0;
			}
		default:
			error("bad definition");
		}
	case LOAD:
		if (! scan(0))				/* entire symbol table */
		{	if (load(table, 0, SYMTABFILE))
				error("cannot load symbol table");
		}
		else						/* list of symbols */
			do
			{	char fnm [BUFSIZ];

				sprintf(fnm, SYMBOLFILE, name(symbol));
				if (load(table, symbol, fnm))
					error("cannot load %s", name(symbol));
			} while (scan(0));
		reclaim(Node(), sunder);
		return 0;
	case SAVE:
		if (! scan(0))				/* entire symbol table */
		{	if (save(table, 0, SYMTABFILE))
				error("cannot save symbol table");
		}
		else						/* list of symbols */
			do
			{	char fnm [BUFSIZ];

				sprintf(fnm, SYMBOLFILE, name(symbol));
				if (save(table, symbol, fnm))
					error("cannot save %s", name(symbol));
			} while (scan(0));
		return 0;
	default:
		node = sum();
		if (token)
			error("trash after sum");
		return node;
	}
}

/*
 *	stmt \n ...
 */

static jmp_buf onError;

int main (void)
{	volatile int errors = 0;
	char buf [BUFSIZ];

	table = new(Symtab(), 10);
	initNames();
	initConst();
	initMath();

	if (setjmp(onError))
	{	++ errors;
		reclaim(Node(), delete);
	}

	while (fgets(buf, sizeof buf, stdin))
		if (scan(buf))
		{	void * e = stmt();

			if (e)
			{	printf("\t%g\n", exec(e));
				delete(e);
			}
		}

	return errors > 0;
}

void error (const char * fmt, ...)
{	va_list ap;

	va_start(ap, fmt);
	vfprintf(stderr, fmt, ap), putc('\n', stderr);
	va_end(ap);
	longjmp(onError, 1);
}
