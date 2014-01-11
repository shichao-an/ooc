#ifndef	NAME_H
#define	NAME_H

/*
 *	install(& struct Name)		arrange for object to be found later
 *
 *	screen(name)
 *		find a name in a table
 *		if nothing found, enter new(Var, name) into table
 *	return associated token and pointer to structure
 */

extern void * symbol;	/* -> last Name found by screen() */

void install (const void * symbol);
int screen (const char * name);

#endif
