c	= new.c String.c Atom.c
h	= new.h String.h
m	= strings.c
r	= new.r
t	= strings atoms

strings	= strings.o new.o String.o
atoms	= strings.o new.o Atom.o

all:		$t
clean:				; rm -f $t *.o

test::		strings		; strings
test::		atoms		; atoms

strings:	$(strings)	; $(CC) $(CFLAGS) -o $@ $(strings)
atoms:		$(atoms)	; $(CC) $(CFLAGS) -o $@ $(atoms)

include ../make/Makefile.$(OSTYPE)
