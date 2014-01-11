#include "Queue.h"

int main (int argc, char ** argv)
{	void * q;
	unsigned n;

	initQueue();
	q = new(Queue, 1);

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
			{	const void * p = take(q);

				puts(p), add(q, p);
			}
		}
	return 0;
}
