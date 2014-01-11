#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

#include "parse.h"
#include "value.h"
#include "value.r"

/*
 *	evaluation driver
 */

void * new (const void * type, ...)
{	va_list ap;
	void * result;

	assert(type && ((struct Type *) type) -> new);

	va_start(ap, type);
	result = ((struct Type *) type) -> new(ap);
	* (const struct Type **) result = type;
	va_end(ap);
	return result;
}

double exec (const void * tree)
{
	assert(tree && * (struct Type **) tree
		&& (* (struct Type **) tree) -> exec);

	return (* (struct Type **) tree) -> exec(tree);
}

void process (const void * tree)
{
	printf("\t%g\n", exec(tree));
}

void delete (void * tree)
{
	assert(tree && * (struct Type **) tree
		&& (* (struct Type **) tree) -> delete);

	(* (struct Type **) tree) -> delete(tree);
}

/*
 *	NUMBER
 */

struct Val {
	const void * type;
	double value;
};

#define	value(tree)	(((struct Val *) tree) -> value)

static void * mkVal (va_list ap)
{	struct Val * node = malloc(sizeof(struct Val));

	assert(node);
	node -> value = va_arg(ap, double);
	return node;
}

static double doVal (const void * tree)
{
	return value(tree);
}

/*
 *	unary operators
 */

struct Un {
	const void * type;
	void * arg;
};

#define	arg(tree)	(((struct Un *) tree) -> arg)

static void * mkUn (va_list ap)
{	struct Un * node = malloc(sizeof(struct Un));

	assert(node);
	node -> arg = va_arg(ap, void *);
	return node;
}

static double doMinus (const void * tree)
{
	return - exec(arg(tree));
}

static void freeUn (void * tree)
{
	delete(((struct Un *) tree) -> arg);
	free(tree);
}

/*
 *	binary operators
 */

void * mkBin (va_list ap)
{	struct Bin * node = malloc(sizeof(struct Bin));

	assert(node);
	node -> left = va_arg(ap, void *);
	node -> right = va_arg(ap, void *);
	return node;
}

static double doAdd (const void * tree)
{
	return exec(left(tree)) + exec(right(tree));
}

static double doSub (const void * tree)
{
	return exec(left(tree)) - exec(right(tree));
}

static double doMult (const void * tree)
{
	return exec(left(tree)) * exec(right(tree));
}

static double doDiv (const void * tree)
{	double left = exec(left(tree));
	double right = exec(right(tree));

	if (right == 0.0)
		error("division by zero");
	return left / right;
}

void freeBin (void * tree)
{
	delete(((struct Bin *) tree) -> left);
	delete(((struct Bin *) tree) -> right);
	free(tree);
}

/*
 *	Types
 */

static struct Type _Add = { mkBin, doAdd, freeBin };
static struct Type _Sub = { mkBin, doSub, freeBin };
static struct Type _Mult = { mkBin, doMult, freeBin };
static struct Type _Div = { mkBin, doDiv, freeBin };
static struct Type _Minus = { mkUn, doMinus, freeUn };
static struct Type _Value = { mkVal, doVal, free };

const void * Add = & _Add;
const void * Sub = & _Sub;
const void * Mult = & _Mult;
const void * Div = & _Div;
const void * Minus = & _Minus;
const void * Value = & _Value;
