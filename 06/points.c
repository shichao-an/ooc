#include "Point.h"

int main (int argc, char ** argv)
{	void * p;

	initPoint();

	while (* ++ argv)
	{	switch (** argv) {
		case 'p':
			p = new(Point, 1, 2);
			break;
		default:
			continue;
		}
		draw(p);
		move(p, 10, 20);
		draw(p);
		delete(p);
	}
	return 0;
}
