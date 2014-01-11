#ifndef	CLASS_R
#define	CLASS_R

#include <stdarg.h>
#include <stdio.h>

struct Class {
	size_t size;
	void * (* ctor) (void * self, va_list * app);
	void * (* dtor) (void * self);
	void * (* clone) (const void * self);
	int (* differ) (const void * self, const void * b);
};

#endif
