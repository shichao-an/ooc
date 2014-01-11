#include <stdio.h>
#include "String.h"
#include "new.h"

int main ()
{	void * a = new(String, "a"), * aa = clone(a);
	void * b = new(String, "b");

	printf("sizeOf(a) == %lu\n", (unsigned long)sizeOf(a));
	if (differ(a, b))
		puts("ok");

	if (differ(a, aa))
		puts("differ?");

	if (a == aa)
		puts("clone?");

	delete(a), delete(aa), delete(b);
	return 0;
}
