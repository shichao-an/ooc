#include "Crt.h"

int main ()
{	void * calc = new(Calc());
	void * crt = new(Crt());
	void * lineOut = new(CLineOut(), 1, 1, 12);
	void * mux = new(Mux());
	static const struct tbl { const char * nm; int y, x; }
	  tbl [] = {					  { "C", 0, 15 },
	    { "1", 3, 0 }, { "2", 3, 5 }, { "3", 3, 10 }, { "+", 3, 15 },
	    { "4", 6, 0 }, { "5", 6, 5 }, { "6", 6, 10 }, { "-", 6, 15 },
	    { "7", 9, 0 }, { "8", 9, 5 }, { "9", 9, 10 }, { "*", 9, 15 },
	    { "Q", 12, 0 },{ "0", 12, 5 },{ "=", 12, 10 },{ "/", 12, 15 },
	    { 0 }};
	const struct tbl * tp;

	makeWindow(crt, 0, 0, 0, 0);
	wire(lineOut, calc);
	wire(mux, crt);

	for (tp = tbl; tp -> nm; ++ tp)
	{	void * o = new(CButton(), tp -> nm, tp -> y, tp -> x);

		wire(calc, o), wire(o, mux);
	}

	gate(crt, 0);
	return 0;
}
