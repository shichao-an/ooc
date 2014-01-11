#include <stdio.h>

#include "Circle.h"
#include "Circle.r"

static void * Circle_ctor (void * _self, va_list * app)
{	struct Circle * self = super_ctor(Circle, _self, app);

	self -> rad = va_arg(* app, int);
	return self;
}

static void Circle_draw (const void * _self)
{	const struct Circle * self = _self;

	printf("circle at %d,%d rad %d\n",
		x(self), y(self), self -> rad);
}

/*
 *	initialization
 */

const void * Circle;

void initCircle (void)
{
	if (! Circle)
	{	initPoint();
		Circle = new(PointClass, "Circle",
				Point, sizeof(struct Circle),
				ctor, Circle_ctor,
				draw, Circle_draw,
				0);
	}
}
