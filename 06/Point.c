#include <assert.h>

#include "Point.h"
#include "Point.r"

/*
 *	Point
 */

static void * Point_ctor (void * _self, va_list * app)
{	struct Point * self = super_ctor(Point, _self, app);

	self -> x = va_arg(* app, int);
	self -> y = va_arg(* app, int);
	return self;
}

static void Point_draw (const void * _self)
{	const struct Point * self = _self;

	printf("\".\" at %d,%d\n", self -> x, self -> y);
}

void draw (const void * _self)
{	const struct PointClass * class = classOf(_self);

	assert(class -> draw);
	class -> draw(_self);
}

void super_draw (const void * _class, const void * _self)
{	const struct PointClass * superclass = super(_class);

	assert(_self && superclass -> draw);
	superclass -> draw(_self);
}

void move (void * _self, int dx, int dy)
{	struct Point * self = _self;

	self -> x += dx, self -> y += dy;
}

/*
 *	PointClass
 */

static void * PointClass_ctor (void * _self, va_list * app)
{	struct PointClass * self
					= super_ctor(PointClass, _self, app);
	typedef void (* voidf) ();
	voidf selector;
#ifdef va_copy
	va_list ap; va_copy(ap, * app);
#else
	va_list ap = * app;
#endif

	while ((selector = va_arg(ap, voidf)))
	{	voidf method = va_arg(ap, voidf);

		if (selector == (voidf) draw)
			* (voidf *) & self -> draw = method;
	}
#ifdef va_copy
    va_end(ap);
#endif

	return self;
}

/*
 *	initialization
 */

const void * PointClass, * Point;

void initPoint (void)
{
	if (! PointClass)
		PointClass = new(Class, "PointClass",
				Class, sizeof(struct PointClass),
				ctor, PointClass_ctor,
				0);
	if (! Point)
		Point = new(PointClass, "Point",
				Object, sizeof(struct Point),
				ctor, Point_ctor,
				draw, Point_draw,
				0);
}
