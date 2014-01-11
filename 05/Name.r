#ifndef	NAME_R
#define	NAME_R

/*
 *	Name
 *	maintain a table of Name structures
 */

struct Name {			/* base structure */
	const void * type;	/* for dynamic linkage */
	const char * name;	/* may be malloc-ed */
	int token;
};

#endif
