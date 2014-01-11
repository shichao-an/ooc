// new(Sort(), minimal-dimension)

% SortClass: ListClass  Sort: List {
	char rflag;
%-
	void flags (_self, Object @ filter, char flag);
	int line (_self, const Object @ filter, const char * fnm, \
												char * buf);
	int quit (_self, const Object @ filter);
%}
