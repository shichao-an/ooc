#include <assert.h>

#include "Exception.h"

int main ()
{	void * a = new(Exception()), * b = new(Exception());

	cause(-1); puts("caused -1");

	switch (catch(a)) {
	case 0:
		switch (catch(b)) {
		case 0:
			cause(1); assert(0);
		case 1:
			puts("caused 1");
			cause(2); assert(0);
		case 2:
			puts("caused 2");
			delete(b);
			cause(3); assert(0);
		default:
			assert(0);
		}
	case 3:
		puts("caused 3");
		delete(a);
		cause(4);
		break;
	default:
		assert(0);
	}
	puts("caused 4");
	return 0;
}
