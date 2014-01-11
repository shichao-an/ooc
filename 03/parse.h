#ifndef	PARSE_H
#define	PARSE_H

/*
 *	symbols
 */

enum tokens {				/* must not clash with operators */
	NUMBER = 'n'			/* literal constant */
};

/*
 *	error recovery
 */

void error (const char * fmt, ...);

#endif
