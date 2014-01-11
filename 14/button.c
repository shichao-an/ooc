#include "Ic.h"

int main ()
{	void * button, * lineOut;
	char buf [100];

	lineOut = new(LineOut());
	button = new(Button(), "a");
	wire(lineOut, button);
	puto(button, stdout);

	while (fgets(buf, sizeof buf, stdin))
	{	void * event = new(Event(), 0, buf);

		if (gate(button, event) == accept)
			break;
		delete(event);
	}

	delete(lineOut), delete(button);
	return 0;
}
