#ifndef	VALUE_R
#define	VALUE_R

#include <stdarg.h>

struct Type {
	void * (* new) (va_list ap);
	double (* exec) (const void * tree);
	void (* delete) (void * tree);
};

double exec (const void * tree);

struct Bin {
	const void * type;
	void * left, * right;
};

#define	left(tree)	(((struct Bin *) tree) -> left)
#define	right(tree)	(((struct Bin *) tree) -> right)

void * mkBin (va_list ap);
void freeBin (void * tree);

#endif
