// new(Xt())
//	wrapper for Xt/... widget
//	Object is part of Xt, i.e., our 'Object' has to be mangled

// new(XtApplicationShell(), & argc, argv)
//	wrapper for Xt application shell

// new(XawLabel(), parent-xt, "node-name")
//	wrapper for Xaw/Label widget

// new(XawCommand(), parent-xt, "node-name")
//	wrapper for Xaw/Command widget

// new(XawBox(), parent-xt, "node-name")
//	wrapper for Xaw/Box widget

// new(XawForm(), parent-xt, "node-name")
//	wrapper for Xaw/Form widget

// new(XLineOut(), parent-xt, "node-name", "text")
//	implements LineOut with a XawLabel

// new(XButton(), parent-xt, "node-name", "text")
//	implements Button with a XawCommand

#include <X11/Intrinsic.h>

% Class  Xt: Object {
	Widget widget;
%
	void * makeWidget (_self, WidgetClass wc, va_list * app);
	void addAllAccelerators (_self);
	void setLabel (_self, const char * text);
	void addCallback (_self, XtCallbackProc fun, XtPointer data);
%}
%prot
#define	widget(xt)	(((struct Xt *) xt) -> widget)

% Class  XtApplicationShell: Xt {
	XtAppContext appContext;
%
	void mainLoop (_self);
%}

% Class  XawLabel: Xt {
%}

% Class  XawCommand: Xt {
%}

% Class  XawBox: Xt {
%}

% Class  XawForm: Xt {
%}

% Class  XLineOut: XawLabel {
%}

% Class  XButton: XawCommand {
	void * button;
%}
