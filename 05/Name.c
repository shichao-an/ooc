#include <assert.h>
#include <stdarg.h>
#include <stdlib.h>
#include <string.h>

#include "binary.h"
#include "parse.h"
#include "value.h"
#include "Name.h"
#include "Name.r"
#include "Var.h"	/* to create variables */

/*
 *	Name
 *	maintain a table of Name structures
 */

#ifndef	NAMES	/* tunable: initial size of symbol table */
#define	NAMES	4
#endif

static int cmp (const void * _key, const void * _elt)
{	const char * const * key = _key;
	const struct Name * const * elt = _elt;

	return strcmp(* key, (* elt) -> name);
}

static struct Name ** search (const char ** name)
{	static const struct Name ** names;	/* dynamic table */
	static size_t used, max;

	if (used >= max)
	{	names = names
			? realloc(names, (max *= 2) * sizeof * names)
			: malloc((max = NAMES) * sizeof * names);
		assert(names);
	}
	return binary(name, names, & used, sizeof * names, cmp);
}

void install (const void * np)
{	const char * name = ((struct Name *) np) -> name;
	struct Name ** pp = search(& name);

	if (* pp != (void *) name)
		error("cannot install name twice: %s", name);
	* pp = (struct Name *) np;
}

void * symbol;		/* -> last struct Name found by screen() */

int screen (const char * name)
{	struct Name ** pp = search(& name);

	if (* pp == (void *) name)	/* entered name */
		* pp = new(Var, name);
	symbol = * pp;
	return (* pp) -> token;
}
