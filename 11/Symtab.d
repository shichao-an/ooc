// new(Symtab(), minimal-dimension)

#include <stddef.h>

% Class  Symtab: Object {
    const void ** buf;		// const void * buf [dim]
    size_t dim;				// current buffer dimension
    size_t count;			// # elements in buffer
%
	void install (_self, const Symbol @ entry);
	Symbol @ screen (_self, const char * name, int lex);
%}
