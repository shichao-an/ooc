// abstract base classes for expression tree

% NodeClass: Class  Node: Object {
	void * next;
%
	void sunder (_self);
%-
	double exec (const _self);
%+
	void reclaim (const _self, Method how);
%}

% NodeClass  Monad: Node {
	void * down;
%}
%prot
#define	down(x)		(((struct Monad *)(x)) -> down)

% NodeClass  Dyad: Node {
	void * left;
	void * right;
%}
%prot
#define	left(x)		(((struct Dyad *)(x)) -> left)
#define	right(x)	(((struct Dyad *)(x)) -> right)

// literals

// new(Number(), value)

% NodeClass  Number: Node {
	double value;
%}

// symbol value references

// new(Global(), constant-or-variable)
// new(Parm(), function)

% NodeClass  Val: Monad {
%}

% NodeClass  Global: Val {
%}

% NodeClass  Parm: Val {
%}

// unary operations

// new(Minus(), subtree)

% NodeClass  Unary: Monad {
%}

% NodeClass  Minus: Unary {
%}

// symbol access

// new(Assign(), var, right-subtree)
// new(Builtin(), math, arg-subtree)
// new(User(), fun, arg-subtree)

% NodeClass  Ref: Dyad {
%}

% NodeClass  Assign: Ref {
%}

% NodeClass  Builtin: Ref {
%}

% NodeClass  User: Ref {
%}

// binary operations

// new(Add(), left-subtree, right-subtree)
// new(Sub(), left-subtree, right-subtree)
// new(Mult(), left-subtree, right-subtree)
// new(Div(), left-subtree, right-subtree)

% NodeClass  Binary: Dyad {
%}

% NodeClass  Add: Binary {
%}

% NodeClass  Sub: Binary {
%}

% NodeClass  Mult: Binary {
%}

% NodeClass  Div: Binary {
%}
