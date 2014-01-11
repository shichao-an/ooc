// new(Exception())

#include <setjmp.h>

#define	catch(e)	setjmp(catchException(e))

void cause (int number);			// if set up, goto catch()

% Class  Exception: Object {
    int armed;							// set by a catch()
    jmp_buf label;						// used by a catch()
%
    void * catchException (_self);
%}
