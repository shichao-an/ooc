// new(Ic())
//	wire() connects me to other object
//	gate() sends something to my outlet; overwritten to receive

// new(Mux())
//	wire() connects me to many outputs
//	gate() tries to send until it finds willing receiver

// new(LineOut())
//	gate() displays input as string

// new(Button(), "text")
//	gate() takes event, possibly sends text on

// new(Event(), kind, data)
//	something to send to Button

// new(Calc())
//	gate() combines input with state, sends result on

enum react { reject, accept };

% IcClass: Class  Ic: Object {
	void * out;
%-
	void wire (Object @ to, _self);
	enum react gate (_self, const void * item);
%}

% IcClass  Mux: Ic {
	void * list;
%}

% IcClass  LineOut: Ic {
%}

% IcClass  Button: Ic {
	const char * text;
%}

%prot
#define	text(b)	(((const struct Button *) b) -> text)

% Class  Event: Object {
	int kind;				// 0 means string-data
	void * data;			// not implicitly deleted
%
	int kind (const _self);
	void * data (const _self);
%}

%prot
typedef	int values[2];		// left and right operand stack

% IcClass  Calc: Ic {
	values value;			// left and right operand
	int op;					// operator
	int state;				// FSM state
%}
