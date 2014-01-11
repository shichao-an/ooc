c	= binary.c Name.c value.c Var.c mathlib.c parse.c
h	= $(c:.c=.h)
r	= Name.r value.r
t	= value

value	= $(c:.c=.o)

all:		$t
clean:				; rm -f $t *.o

test::		value		; echo 'let a = sin(1) + PI' | value

value:		$(value)	; $(CC) $(CFLAGS) -o $@ $(value) $(Lm)

include ../make/Makefile.$(OSTYPE)
