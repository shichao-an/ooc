#include "Stack.h"

int main (int argc, char ** argv)
{	void * q;
	unsigned n;

	initStack();
	q = new(Stack, 1);

	while (* ++ argv)
		switch (** argv) {
		case '+':
			add(q, *argv + 1);
			break;
		case '-':
			puts((char *) take(q));
			break;
		default:
			n = count(q);
			while (n -- > 0)
			{	const void * p = takeFirst(q);

				puts(p), addLast(q, p);
			}
		}
	return 0;
}
