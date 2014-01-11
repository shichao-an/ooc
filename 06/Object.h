#ifndef	OBJECT_H
#define	OBJECT_H

#include <stdarg.h>
#include <stddef.h>
#include <stdio.h>

extern const void * Object;		/* new(Object); */

void * new (const void * class, ...);
void delete (void * self);

const void * classOf (const void * self);
size_t sizeOf (const void * self);

void * ctor (void * self, va_list * app);
void * dtor (void * self);
int differ (const void * self, const void * b);
int puto (const void * self, FILE * fp);

extern const void * Class;	/* new(Class, "name", super, size
										sel, meth, ... 0); */

const void * super (const void * self);	/* class' superclass */

#endif
