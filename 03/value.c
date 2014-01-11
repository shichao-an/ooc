#include <assert.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>

#include "parse.h"
#include "value.h"

/*
 *	evaluation driver
 */

struct Type {
	void * (* new) (va_list ap);
	double (* exec) (const void * tree);
	void (* delete) (void * tree);
};

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

static double exec (const void * tree)
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

static void * mkVal (va_list ap)
{	struct Val * node = malloc(sizeof(struct Val));

	assert(node);
	node -> value = va_arg(ap, double);
	return node;
}

static double doVal (const void * tree)
{
	return ((struct Val *) tree) -> value;
}

/*
 *	unary operators
 */

struct Un {
	const void * type;
	void * arg;
};

static void * mkUn (va_list ap)
{	struct Un * node = malloc(sizeof(struct Un));

	assert(node);
	node -> arg = va_arg(ap, void *);
	return node;
}

static double doMinus (const void * tree)
{
	return - exec(((struct Un *) tree) -> arg);
}

static void freeUn (void * tree)
{
	delete(((struct Un *) tree) -> arg);
	free(tree);
}

/*
 *	binary operators
 */

struct Bin {
	const void * type;
	void * left, * right;
};

static void * mkBin (va_list ap)
{	struct Bin * node = malloc(sizeof(struct Bin));

	assert(node);
	node -> left = va_arg(ap, void *);
	node -> right = va_arg(ap, void *);
	return node;
}

static double doAdd (const void * tree)
{
	return exec(((struct Bin *) tree) -> left) +
			exec(((struct Bin *) tree) -> right);
}

static double doSub (const void * tree)
{
	return exec(((struct Bin *) tree) -> left) -
		exec(((struct Bin *) tree) -> right);
}

static double doMult (const void * tree)
{
	return exec(((struct Bin *) tree) -> left) *
		exec(((struct Bin *) tree) -> right);
}

static double doDiv (const void * tree)
{	double left = exec(((struct Bin *) tree) -> left);
	double right = exec(((struct Bin *) tree) -> right);

	if (right == 0.0)
		error("division by zero");
	return left / right;
}

static void freeBin (void * tree)
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
