#include <assert.h>
#include <stdlib.h>
#include <string.h>

#include "Object.h"
#include "Object.r"

/*
 *	Object
 */

static void * Object_ctor (void * _self, va_list * app)
{
	return _self;
}

static void * Object_dtor (void * _self)
{
	return _self;
}

static int Object_differ (const void * _self, const void * b)
{
	return _self != b;
}

static int Object_puto (const void * _self, FILE * fp)
{	const struct Class * class = classOf(_self);

	return fprintf(fp, "%s at %p\n", class -> name, _self);
}

const void * classOf (const void * _self)
{	const struct Object * self = _self;

	assert(self && self -> class);
	return self -> class;
}

size_t sizeOf (const void * _self)
{	const struct Class * class = classOf(_self);

	return class -> size;
}

/*
 *	Class
 */

static void * Class_ctor (void * _self, va_list * app)
{	struct Class * self = _self;
	const size_t offset = offsetof(struct Class, ctor);

	self -> name = va_arg(* app, char *);
	self -> super = va_arg(* app, struct Class *);
	self -> size = va_arg(* app, size_t);

	assert(self -> super);

	memcpy((char *) self + offset, (char *) self -> super
					+ offset, sizeOf(self -> super) - offset);
{
	typedef void (* voidf) ();	/* generic function pointer */
	voidf selector;
#ifdef va_copy
	va_list ap; va_copy(ap, * app);
#else
	va_list ap = * app;
#endif
    
	while ((selector = va_arg(ap, voidf)))
	{	voidf method = va_arg(ap, voidf);

		if (selector == (voidf) ctor)
			* (voidf *) & self -> ctor = method;
		else if (selector == (voidf) dtor)
			* (voidf *) & self -> dtor = method;
		else if (selector == (voidf) differ)
			* (voidf *) & self -> differ = method;
		else if (selector == (voidf) puto)
			* (voidf *) & self -> puto = method;
	}
#ifdef va_copy
    va_end(ap);
#endif

	return self;
}}

static void * Class_dtor (void * _self)
{	struct Class * self = _self;

	fprintf(stderr, "%s: cannot destroy class\n", self->name);
	return 0;
}

const void * super (const void * _self)
{	const struct Class * self = _self;

	assert(self && self -> super);
	return self -> super;
}

/*
 *	initialization
 */

static const struct Class object [] = {
	{ { object + 1 },
	  "Object", object, sizeof(struct Object),
	  Object_ctor, Object_dtor, Object_differ, Object_puto
	},
	{ { object + 1 },
	  "Class", object, sizeof(struct Class),
	  Class_ctor, Class_dtor, Object_differ, Object_puto
	}
};

const void * Object = object;
const void * Class = object + 1;

/*
 *	object management and selectors
 */

void * new (const void * _class, ...)
{	const struct Class * class = _class;
	struct Object * object;
	va_list ap;

	assert(class && class -> size);
	object = calloc(1, class -> size);
	assert(object);
	object -> class = class;
	va_start(ap, _class);
	object = ctor(object, & ap);
	va_end(ap);
	return object;
}

void delete (void * _self)
{
	if (_self)
		free(dtor(_self));
}

void * ctor (void * _self, va_list * app)
{	const struct Class * class = classOf(_self);

	assert(class -> ctor);
	return class -> ctor(_self, app);
}

void * super_ctor (const void * _class,
				void * _self, va_list * app)
{	const struct Class * superclass = super(_class);

	assert(_self && superclass -> ctor);
	return superclass -> ctor(_self, app);
}

void * dtor (void * _self)
{	const struct Class * class = classOf(_self);

	assert(class -> dtor);
	return class -> dtor(_self);
}

void * super_dtor (const void * _class, void * _self)
{	const struct Class * superclass = super(_class);

	assert(_self && superclass -> dtor);
	return superclass -> dtor(_self);
}

int differ (const void * _self, const void * b)
{	const struct Class * class = classOf(_self);

	assert(class -> differ);
	return class -> differ(_self, b);
}

int super_differ (const void * _class, const void * _self, const void * b)
{	const struct Class * superclass = super(_class);

	assert(_self && superclass -> differ);
	return superclass -> differ(_self, b);
}

int puto (const void * _self, FILE * fp)
{	const struct Class * class = classOf(_self);

	assert(class -> puto);
	return class -> puto(_self, fp);
}

int super_puto (const void * _class, const void * _self, FILE * fp)
{	const struct Class * superclass = super(_class);

	assert(_self && superclass -> puto);
	return superclass -> puto(_self, fp);
}
