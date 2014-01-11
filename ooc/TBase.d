//	TBase.d -- 5.2 Thu Jan  3 18:47:44 EST 2002
//	Copyright (c) 1993 Axel T. Schreiner
//
line 1			class with new meta class
line 2			// comment
% TBaseClass:Class TBase:T {
	TBase @ line4;
	int line5;
%
	int sInt (int i, _self, _v, TBase @ t, const Object @ o, ...);
	const TBase @ sTBase (const _self);
%-
	void dVoid (int i, _self, _v, TBase @ t, const Object @ o, ...);
	const TBase @ dTBase (const _self);
%+
	const int cInt (int i, _self, _v, TBase @ t, const Object @ o, ...);
	void cVoid (const _self);
%}
line -2
line -1
