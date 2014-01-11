//	Test.d -- 5.2 Jun  9 08:59:52 1993
//	Copyright (c) 1993 Axel T. Schreiner
//
line 1			class without new meta class
line 2			// comment
% TBaseClass Test:TBase {
	int line4;
	int line5;
%
	int tInt (int i, _self, _v, const Object @ o, ...);
	const Test @ tTest (const _self);
%}
line -2
line -1
