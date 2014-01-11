#include <ctype.h>

#include "Ic.h"

int main ()
{	void * calc = new(Calc());
	void * lineOut = new(LineOut());
	void * mux = new(Mux());
	static const char * const cmd [] = { "C", "Q",
		"0", "1", "2", "3", "4", "5", "6", "7", "8", "9",
		"+", "-", "*", "/", "=", 0 };
	const char * const * cpp;
	int ch;

	wire(lineOut, calc);
	for (cpp = cmd; * cpp; ++ cpp)
	{	void * button = new(Button(), * cpp);

		wire(calc, button), wire(button, mux);
	}

	while ((ch = getchar()) != EOF)
		if (! isspace(ch))
		{	static char buf [2];
			void * event;

			buf[0] = ch;
			gate(mux, event = new(Event(), 0, buf));
			delete(event);
		}
	return 0;
}
