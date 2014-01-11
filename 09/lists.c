#include <ctype.h>
#include <stdlib.h>

#include "Circle.h"
#include "List.h"

int main (int argc, char ** argv)
{	void * q;
	unsigned n;

	q = new(List(), 1);

	while (* ++ argv)
		switch (** argv) {
		case '+':
			switch ((* argv)[1]) {
			case 'c':
				addFirst(q, new(Circle(), 1, 2, 3));
				break;
			case 'p':
				addFirst(q, new(Point(), 4, 5));
				break;
			default:
				addFirst(q, new(Object()));
			}
			break;
		case '-':
			puto(takeLast(q), stdout);
			break;
		case '.':
			n = count(q);
			while (n -- > 0)
			{	const void * p = takeFirst(q);

				if (isOf(p, Point()))
					draw(p);
				else
					 puto(p, stdout);
				addLast(q, p);
			}
			break;
		default:
			if (isdigit(** argv & 0xff))
				addFirst(q, (void *) atoi(* argv));
			else
				addFirst(q, * argv + 1);
		}
	return 0;
}
