#ifndef	PARSE_H
#define	PARSE_H

/*
 *	error recovery
 */

void error (const char * fmt, ...);

/*
 *	symbol transput
 */

int putsymbol (const void * sym, FILE * fp);
void * getsymbol (const void * class, FILE * fp);

#endif
