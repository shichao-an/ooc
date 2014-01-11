chapter	= -11

c	= Object.c Node.c Symbol.c Symtab.c
d	= $(c:.c=)
h	= $(c:.c=.h) binary.h parse.h
m	= binary.c parse.c
r	= $(c:.c=.r)
t	= value

value	= parse.o Object.o Node.o Symbol.o Symtab.o binary.o

all:	$t
clean:		; rm -f $c $(c:.c=.h) $r $t *.o

test::	value	; echo 'let a = sin(1) + PI' | value
test::	value	; { echo 'def sqr = $$ * $$'; \
		    echo 'def one = sqr(sin($$)) + sqr(cos($$))'; \
		    echo 'one(10)'; } | value reclaim

value:	$(value); $(CC) $(CFLAGS) -o $@ $(value) $(Lm) $(debug)

include ../make/Makefile.$(OSTYPE)
