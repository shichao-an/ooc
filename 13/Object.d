#include <stdarg.h>
#include <stddef.h>
#include <stdio.h>

typedef void (* Method) ();			// for respondsTo()
void * retrieve (FILE * fp);		// object from file

%prot
#include <assert.h>

struct Method {
	const char * tag;			// for respondsTo()
	Method selector;			// returned by respondsTo()
	Method method;				// accessed by the selector
};

% Class  Object {
    unsigned long magic;			// magic number
    const Class @ class;			// object's description
%
    const Class @ classOf (const _self);	// object's class
    size_t sizeOf (const _self);		// object's size
    int isA (const _self, const Class @ class);	// object belongs to class
    int isOf (const _self, const Class @ class);// object derives from class
    void * cast (const Class @ class, const _self); // asserts isOf()
    Method respondsTo (const _self, const char * tag);
%-
    void * ctor (_self, va_list * app);		// constructor
    void * dtor (_self);					// destructor
    int differ (const _self, const Object @ b);	// true if !=
    int puto (const _self, FILE * fp);		// display
	void delete (_self);				// reclaim instance
:	void * geto (_self, FILE * fp);		// construct from file
%+
	Object @ new (const _self, ...);	// create instance
%}

		// order known in Object.c for static initialization
% Class  Class: Object {
    const char * name;				// class' name
    const Class @ super;			// class' superclass
    size_t size;					// object's memory size
%
    Object @ allocate (const _self);	// memory for instance
    const Class @ super (const _self);	// class' superclass
	const char * nameOf (const _self);	// class' name
%}
%prot
#define	MAGIC	0x0effaced	// magic number for objects
