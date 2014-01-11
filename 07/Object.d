#include <stdarg.h>
#include <stddef.h>
#include <stdio.h>
%prot
#include <assert.h>

% Class  Object {
    const Class @ class;			// object's description
%
    void delete (_self);				// reclaim instance
    const void * classOf (const _self);	// object's class
    size_t sizeOf (const _self);		// object's size
%-
    void * ctor (_self, va_list * app);		// constructor
    void * dtor (_self);					// destructor
    int differ (const _self, const Object @ b);	// true if !=
    int puto (const _self, FILE * fp);		// display
%}

		// order known in Object.c for static initialization
% Class  Class: Object {
    const char * name;				// class' name
    const Class @ super;			// class' superclass
    size_t size;					// object's memory size
%
    Object @ new (const _self, ...);	// create instance
    const void * super (const _self);	// class' superclass
%}
