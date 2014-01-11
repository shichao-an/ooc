// new(List(), minimal-dimension)

% ListClass: Class  List: Object {
    const void ** buf;	// const void * buf [dim]
    unsigned dim;		// current buffer dimension
    unsigned count;		// # elements in buffer
    unsigned begin;		// index of takeFirst slot, 0..dim
    unsigned end;		// index of addLast slot, 0..dim
%
    Object @ addFirst (_self, const Object @ element);
    Object @ addLast (_self, const Object @ element);
    unsigned count (const _self);
    Object @ lookAt (const _self, unsigned n);
    Object @ takeFirst (_self);
    Object @ takeLast (_self);
    void sort (_self);
%-						// abstract, for Queue/Stack
    Object @ add (_self, const Object @ element);
    Object @ take (_self);
%}
