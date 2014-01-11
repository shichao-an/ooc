#ifndef	BINARY_H
#define	BINARY_H

/*
 *	binary()
 *	search and maintain a sorted array
 *
 *	key	search argument, passed as first argument to cmp
 *		if not found, entered as width bytes into table
 *	base	begin of table with *nelp elements of width bytes
 *	cmp()	comparison, must return < == > 0
 *
 *	returns pointer to table entry containing key, e.g., base, base+1...
 */

#include <stddef.h>

void * binary (const void * key, void * base, size_t * nelp,
	size_t width, int (* cmp) (const void * key, const void * elt));

#endif
