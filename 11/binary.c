/*
 *	binary()
 *	search and maintain a sorted array
 */

#include <string.h>
#include "binary.h"

void * binary (const void * key,
	void * _base, size_t * nelp, size_t width,
	int (* cmp) (const void * key, const void * elt))
{	size_t nel = * nelp;
#define	base	(* (char **) & _base)
	char * lim = base + nel * width, * high;

	if (nel > 0)
	{	for (high = lim - width; base <= high; nel >>= 1)
		{	char * mid = base + (nel >> 1) * width;
			int c = cmp(key, mid);

			if (c < 0)
				high = mid - width;
			else if (c > 0)
				base = mid + width, -- nel;
			else
				return (void *) mid;
		}
		memmove(base + width, base, lim - base);
	}
	++ *nelp;
	return memcpy(base, key, width);
#undef	base
}
