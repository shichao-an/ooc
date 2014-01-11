// new(Crt())
//	wraps a curses window, gate(crt, 0) produces events

// new(CLineOut(), y, x, len)
//	LineOut, located at y, x, with line length len

// new(CButton(), "text", y, x)
//	Button, located at y, x, sized for "text"

% IcClass  Crt: Ic {
	void * window;		// curses WINDOW
	int rows;		// size
	int cols;
%
	void makeWindow (_self, int rows, int cols, int y, int x);
	void addStr(_self, int y, int x, const char * s);
	void crtBox(_self);
%}

%prot
#define	rows(c)	(((const struct Crt *) c) -> rows)
#define	cols(c)	(((const struct Crt *) c) -> cols)

% IcClass  CLineOut: Crt {
%}

% IcClass  CButton: Crt {
	void * button;
	int y;
	int x;
%}
