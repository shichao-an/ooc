#include <assert.h>
#include <stdlib.h>
#include <string.h>

#include "Name.h"
#include "Name.r"
#include "parse.h"
#include "Var.h"
#include "value.h"
#include "value.r"

/*
 *	variables
 *	tree -> struct Var	(also located in symbol table)
 */

struct Var { struct Name _; double value; };

#define	value(tree)	(((struct Var *) tree) -> value)

static void * mkVar (va_list ap)
{	struct Var * node = calloc(1, sizeof(struct Var));
	const char * name = va_arg(ap, const char *);
	size_t len = strlen(name);

	assert(node);
	node -> _.name = malloc(len+1);
	assert(node -> _.name);
	strcpy((void *) node -> _.name, name);
	node -> _.token = VAR;
	return node;
}

static double doVar (const void * tree)
{
	return value(tree);
}

static void freeVar (void * tree)
{
}

/*
 *	assignment
 *	reuses struct Bin, mkBin()
 */

static double doAssign (const void * tree)
{
	return value(left(tree)) = exec(right(tree));
}

/*
 *	variable types
 */

static struct Type _Var = { mkVar, doVar, freeVar };
static struct Type _Assign = { mkBin, doAssign, freeBin };

const void * Var = & _Var;
const void * Assign = & _Assign;

/*
 *	constants
 *	like variables, but with a distinct token
 */

void initConst (void)
{	static const struct Var constants [] = {	/* like hoc */
		{ { &_Var, "PI",    CONST },  3.14159265358979323846 },
		{ { &_Var, "E",     CONST },  2.71828182845904523536 },
		{ { &_Var, "GAMMA", CONST },  0.57721566490153286060 }, /* Euler */
		{ { &_Var, "DEG",   CONST }, 57.29577951308232087680 }, /* deg/radian */
		{ { &_Var, "PHI",   CONST },  1.61803398874989484820 }, /* gold ratio */
		{ { 0 } } };

	const struct Var * vp;

	for (vp = constants; vp -> _.name; ++ vp)
		install(vp);
}
