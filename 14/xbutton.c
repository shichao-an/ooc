#include "Ic.h"
#include "Xt.h"

int main (int argc, char * argv [])
{	void * shell = new(XtApplicationShell(), & argc, argv);
	void * box = new(XawBox(), shell, 0);
	void * composite = new(XawBox(), box, 0);
	void * lineOut = new(XLineOut(), composite, 0, "-long-");
	void * button = new(XButton(), composite, 0, "a");

	wire(lineOut, button);
	puto(button, stdout);	/* Box will move its children */

	composite = new(XawForm(), box, "form");
	lineOut = new(XLineOut(), composite,"lineOut", "-long-");
	button = new(XButton(), composite, "button", "b");

	wire(lineOut, button);
	puto(button, stdout);	/* Form won't move its children */

	mainLoop(shell);
        return 0; /* dummy */
}
