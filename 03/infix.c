#include <assert.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>

#include "parse.h"
#include "value.h"

/*
 *	infix driver
 */

struct Type {
	const char * name;		/* node's name */
	char rank, rpar;
	void * (* new) (va_list ap);
	void (* exec) (const void * tree, int rank, int par);
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

static void exec (const void * tree, int rank, int par)
{
	assert(tree && * (struct Type **) tree
		&& (* (struct Type **) tree) -> exec);

	(* (struct Type **) tree) -> exec(tree, rank, par);
}

void process (const void * tree)
{
	putchar('\t');
	exec(tree, (* (struct Type **) tree) -> rank, 0);
	putchar('\n');
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

static void doVal (const void * tree, int rank, int par)
{
	printf("%g", ((struct Val *) tree) -> value);
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

static void doUn (const void * tree, int rank, int par)
{	const struct Type * type = * (struct Type **) tree;

	printf("%s ", type -> name);
	exec(((struct Un *) tree) -> arg, type -> rank, 0);
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

static void doBin (const void * tree, int rank, int par)
{	const struct Type * type = * (struct Type **) tree;

	par = type -> rank < rank
			|| (par && type -> rank == rank);

	if (par) putchar('(');
	exec(((struct Bin *) tree) -> left, type -> rank, 0);
	printf(" %s ", type -> name);
	exec(((struct Bin *) tree) -> right,
			type -> rank, type -> rpar);
	if (par) putchar(')');
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

static struct Type _Add = {"+", 1, 0, mkBin, doBin, freeBin};
static struct Type _Sub = {"-", 1, 1, mkBin, doBin, freeBin};
static struct Type _Mult = { "*", 2, 0, mkBin, doBin, freeBin };
static struct Type _Div = { "/", 2, 1, mkBin, doBin, freeBin };
static struct Type _Minus = { "-", 3, 0, mkUn, doUn, freeUn };
static struct Type _Value = { "", 4, 0, mkVal, doVal, free };

const void * Add = & _Add;
const void * Sub = & _Sub;
const void * Mult = & _Mult;
const void * Div = & _Div;
const void * Minus = & _Minus;
const void * Value = & _Value;
