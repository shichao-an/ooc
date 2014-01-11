#include "Crt.h"

int main ()
{	void * crt = new(Crt());
	void * lineOut = new(CLineOut(), 5, 10, 40);
	void * button = new(CButton(), "a", 10, 40);

	makeWindow(crt, 0, 0, 0, 0);	/* total screen */
	gate(lineOut, "hello, world");

	wire(lineOut, button), wire(button, crt);
	gate(crt, 0);					/* main loop */

	return 0;
}
