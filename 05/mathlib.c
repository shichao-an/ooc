#include <assert.h>
#include <errno.h>
#include <math.h>
#include <stdlib.h>
#include <string.h>

#include "Name.h"
#include "Name.r"
#include "mathlib.h"
#include "parse.h"
#include "value.h"
#include "value.r"

/*
 *	libm(3) functions
 *	tree -> struct Bin
 *			type: Math
 *			left -> struct Math for sin, etc.
 *			right -> argument tree
 */

struct Math { struct Name _;
	double (* funct) (double);
};

#define	funct(tree)	(((struct Math *) left(tree)) -> funct)

static double doMath (const void * tree)
{	double result = exec(right(tree));

	errno = 0;
	result = funct(tree)(result);
	if (errno)
		error("error in %s: %s",
			((struct Math *) left(tree)) -> _.name,
			strerror(errno));
	return result;
}

static void freeMath (void * tree)
{
	delete(right(tree));
	free(tree);
}

static const struct Type _Math = { mkBin, doMath, freeMath };
const void * Math = & _Math;

void initMath (void)
{	static const struct Math functions [] = {
		{ { &_Math, "sin",   MATH },  sin },
		{ { &_Math, "cos",   MATH },  cos },
		{ { &_Math, "tan",   MATH },  tan },
		{ { &_Math, "asin",  MATH },  asin },
		{ { &_Math, "acos",  MATH },  acos },
		{ { &_Math, "atan",  MATH },  atan },
		{ { &_Math, "sinh",  MATH },  sinh },
		{ { &_Math, "cosh",  MATH },  cosh },
		{ { &_Math, "tanh",  MATH },  tanh },
		{ { &_Math, "exp",   MATH },  exp },
		{ { &_Math, "log",   MATH },  log },
		{ { &_Math, "log10", MATH },  log10 },
		{ { &_Math, "sqrt",  MATH },  sqrt },
		{ { &_Math, "ceil",  MATH },  ceil },
		{ { &_Math, "floor", MATH },  floor },
		{ { &_Math, "abs",   MATH },  fabs },
		{ { 0 } } };

	const struct Math * mp;

	for (mp = functions; mp -> _.name; ++ mp)
		install(mp);
}
