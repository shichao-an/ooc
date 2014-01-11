#include "Xt.h"

int main (int argc, char * argv [])
{	void * shell = new(XtApplicationShell(), & argc, argv);
	void * lineOut = new(XLineOut(), shell, 0, "hello, world");

	puto(lineOut, stdout);
	mainLoop(shell);
        return 0; /* dummy */
}
