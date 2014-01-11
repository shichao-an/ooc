#ifndef	CLASS_R
#define	CLASS_R

#include <stdarg.h>

struct Class {
	size_t size;
	void * (* ctor) (void * self, va_list * app);
	void * (* dtor) (void * self);
	void (* draw) (const void * self);
};

#endif
