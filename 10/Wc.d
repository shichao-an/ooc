// new(Wc())

% WcClass: Class  Wc: Object {
	unsigned lines;			// lines in current file
	unsigned allLines;		// lines in previous files
	unsigned chars;			// bytes in current file
	unsigned allChars;		// bytes in previous files
	unsigned files;			// files completed
%-
line:	int wc (_self, const Object @ filter,			\
				const char * fnm, char * buf);
wrap:	int printFile (_self, const Object @ filter,	\
				const char * fnm);
quit:	int printTotal (_self, const Object @ filter);
%}
