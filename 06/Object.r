#ifndef OBJECT_R
#define OBJECT_R

struct Object {
	const struct Class * class;	/* object's description */
};

struct Class {
	const struct Object _;			/* class' description */
	const char * name;				/* class' name */
	const struct Class * super;		/* class' super class */
	size_t size;					/* class' object's size */
	void * (* ctor) (void * self, va_list * app);
	void * (* dtor) (void * self);
	int (* differ) (const void * self, const void * b);
	int (* puto) (const void * self, FILE * fp);
};

void * super_ctor (const void * class, void * self, va_list * app);
void * super_dtor (const void * class, void * self);
int super_differ (const void * class, const void * self, const void * b);
int super_puto (const void * class, const void * self, FILE * fp);

#endif
