#include <ctype.h>
#include <errno.h>
#include <setjmp.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "parse.h"			/* defines NUMBER */
#include "value.h"
#include "mathlib.h"
#include "Var.h"

/*
 *	reserved words
 */

#include "Name.h"
#include "Name.r"

static void initNames (void)
{	static const struct Name names [] = {
		{ 0, "let",	LET },
		{ 0 } };
	const struct Name *  np;

	for (np = names; np -> name; ++ np)
		install(np);
}

/*
 *	scanner
 */

#define ALNUM	"ABCDEFGHIJKLMNOPQRSTUVWXYZ" \
				"abcdefghijklmnopqrstuvwxyz" \
				"_" "0123456789"

static enum tokens token;	/* current input symbol */
static double number;		/* if NUMBER: numerical value */

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
		token = screen(buf);
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
 *			 ( sum )
 */

static void * sum (void);

static void * factor (void)
{	void * result;

	switch ((int)token) {
	case '+':
		scan(0);
		return factor();
	case '-':
		scan(0);
		return new(Minus, factor());
	default:
		error("bad factor: '%c' 0x%x", token, token);
	case NUMBER:
		result = new(Value, number);
		break;
	case CONST:
	case VAR:
		result = symbol;
		break;
	case MATH:
	{	const struct Name * fp = symbol;

		if (scan(0) != '(')
			error("expecting (");
		scan(0);
		result = new(Math, fp, sum());
		if (token != ')')
			error("expecting )");
		break;
	}
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
			type = Mult;
			break;
		case '/':
			type = Div;
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
			type = Add;
			break;
		case '-':
			type = Sub;
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
 *		   sum
 */

static void * stmt (void)
{	void * result;

	switch (token) {
	case LET:
		if (scan(0) != VAR)
			error("bad assignment");
		result = symbol;
		if (scan(0) != '=')
			error("expecting =");
		scan(0);
		return new(Assign, result, sum());
	default:
		return sum();
	}
}

/*
 *	stmt \n ...
 */

static jmp_buf onError;

int main (void)
{	volatile int errors = 0;
	char buf [BUFSIZ];

	initNames();
	initConst();
	initMath();

	if (setjmp(onError))
		++ errors;

	while (fgets(buf, sizeof buf, stdin))
		if (scan(buf))
		{	void * e = stmt();

			if (token)
				error("trash after sum");
			process(e);
			delete(e);
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
