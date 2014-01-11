#include <assert.h>
#include <stdio.h>

#include "new.h"
#include "Set.h"
#include "Object.h"

const void * Set;
const void * Object;

#if ! defined MANY || MANY < 1
#define	MANY	10
#endif

static int heap [MANY];

void * new (const void * type, ...)
{	int * p;							/* & heap[1..] */

	for (p = heap + 1; p < heap + MANY; ++ p)
		if (! * p)
			break;
	assert(p < heap + MANY);
	* p = MANY;
	return p;
}

void delete (void * _item)
{	int * item = _item;

	if (item)
	{	assert(item > heap && item < heap + MANY);
		* item = 0;
	}
}

void * add (void * _set, const void * _element)
{	int * set = _set;
	const int * element = _element;

	assert(set > heap && set < heap + MANY);
	assert(* set == MANY);
	assert(element > heap && element < heap + MANY);

	if (* element == MANY)
		* (int *) element = set - heap;
	else
		assert(* element == set - heap);

	return (void *) element;
}

void * find (const void * _set, const void * _element)
{	const int * set = _set;
	const int * element = _element;

	assert(set > heap && set < heap + MANY);
	assert(* set == MANY);
	assert(element > heap && element < heap + MANY);
	assert(* element);

	return * element == set - heap ? (void *) element : 0;
}

int contains (const void * _set, const void * _element)
{
	return find(_set, _element) != 0;
}

void * drop (void * _set, const void * _element)
{	int * element = find(_set, _element);

	if (element)
		* element = MANY;
	return element;
}

int differ (const void * a, const void * b)
{
	return a != b;
}
