// new(Symbol(), "name", lex)		"name" must not change
// new(Reserved(), "name", lex)
// new(Var(), "name", VAR)
// new(Const(), "name", CONST, value)
// new(Fun(), "name", FUN)
// new(Math(), "name", MATH, function-name)

% Class  Symbol: Object {
	const char * name;
	int lex;
%
	const char * name (const _self);
	int lex (const _self);
%}

% Class  Reserved: Symbol {
%}

% Class  Var: Symbol {
	double value;
%
	double value (const _self);
	double setvalue (_self, double value);
%}

% Class  Const: Var {
%}

% Class  Fun: Var {
	void * fun;
%
	void setfun (_self, Node @ fun);
	double funvalue (_self, double value);
%}

typedef double (* function) (double);

% Class  Math: Symbol {
	function fun;
%
	double mathvalue (const _self, double value);
%}
