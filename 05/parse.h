#ifndef	PARSE_H
#define	PARSE_H

/*
 *	symbols
 */

enum tokens {				/* must not clash with operators */
	NUMBER = 'n',			/* literal constant */
	CONST = 'c',				/* constant name */
	MATH = 'm',				/* library function name */
	VAR = 'v',				/* variable name */

	LET = 'l'				/* LET VAR = sum */
};

/*
 *	error recovery
 */

void error (const char * fmt, ...);

#endif
