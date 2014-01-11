//	T.d -- 5.3 Thu Jan  3 18:42:58 EST 2002
//	Copyright (c) 1993 Axel T. Schreiner
//
line 1			root and meta root class
line 2			// comment
% Class T {
	const Class @ class;
%
	void s (_self);
	void S (_self);
%-
	void * ctor (_self, va_list * app);
	void D (_self);
%+
	void c (_self);
	void C (_self);
%}

% Class Class:T {
	const char * name;
	const Class @ super;
	size_t size;
%
	void cc (_self);
	void CC (_self);
%}
line -2
line -1
