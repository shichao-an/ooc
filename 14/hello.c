#include "Ic.h"

int main ()
{	void * ic = new(Ic());
	void * mux = new(Mux());
	int i;
	void * lineOut = new(LineOut());

	for (i = 0; i < 2; ++ i)
		wire(new(Ic()), mux);
	wire(lineOut, mux);
	wire(lineOut, mux);
	wire(mux, ic);
	puto(ic, stdout);
	gate(ic, "hello, world");
	delete(ic);
	delete(mux);
	delete(lineOut);
	return 0;
}
