#include "Ic.h"
#include "Xt.h"

int main (int argc, char * argv [])
{	void * shell = new(XtApplicationShell(), & argc, argv);
	void * form = new(XawForm(), shell, "form");
	void * lineOut = new(XLineOut(), form, "lineOut",
												"........");
	void * calc = new(Calc());
	static const char * const cmd [] = {  "C", "C",
		"1", "1", "2", "2", "3", "3", "a", "+",
		"4", "4", "5", "5", "6", "6", "s", "-",
		"7", "7", "8", "8", "9", "9", "m", "*",
		"Q", "Q", "0", "0", "t", "=", "d", "/", 0 };
	const char * const * cpp;

	wire(lineOut, calc);
	for (cpp = cmd; * cpp; cpp += 2)
	{	void * button = new(XButton(), form, cpp[0], cpp[1]);

		wire(calc, button);
	}
	addAllAccelerators(form);
	mainLoop(shell);
        return 0; /* dummy */
}
