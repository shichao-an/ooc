#include <assert.h>
#include <stdlib.h>
#include <string.h>

#include "String.h"
#include "new.h"
#include "new.r"

struct String {
	const void * class;			/* must be first */
	char * text;
	struct String * next;
	unsigned count;
};

static struct String * ring;	/* of all strings */

static void * String_ctor (void * _self, va_list * app)
{	struct String * self = _self;
	const char * text = va_arg(* app, const char *);

	if (ring)
	{	struct String * p = ring;

		do
			if (strcmp(p -> text, text) == 0)
			{	++ p -> count;
				free(self);
				return p;
			}
		while ((p = p -> next) != ring);
	}
	else
		ring = self;

	self -> next = ring -> next, ring -> next = self;
	self -> count = 1;

	self -> text = malloc(strlen(text) + 1);
	assert(self -> text);
	strcpy(self -> text, text);
	return self;
}

static void * String_dtor (void * _self)
{	struct String * self = _self;

	if (-- self -> count > 0)
		return 0;

	assert(ring);
	if (ring == self)
		ring = self -> next;
	if (ring == self)
		ring = 0;
	else
	{	struct String * p = ring;

		while (p -> next != self)
		{	p = p -> next;
			assert(p != ring);
		}
		p -> next = self -> next;
	}

	free(self -> text), self -> text = 0;
	return self;
}

static void * String_clone (const void * _self)
{	struct String * self = (void *) _self;

	++ self -> count;
	return self;
}

static int String_differ (const void * self, const void * b)
{
	return self != b;
}

static const struct Class _String = {
	sizeof(struct String),
	String_ctor, String_dtor,
	String_clone, String_differ
};

const void * String = & _String;
