chapter	= -8

c	= Object.c Point.c Circle.c List.c
d	= $(c:.c=)
h	= $(c:.c=.h)
m	= circles.c lists.c
r	= $(c:.c=.r)
t	= $(m:.c=)

circles	= circles.o Object.o Point.o Circle.o
lists	= lists.o Object.o List.o Point.o Circle.o

all:		$t
clean:				; rm -f $c $h $r $t *.o

test::		circles		;  circles p c
test::		lists		;- lists + +c +p . - 1234
test::		lists		;- lists axel

circles:	$(circles)	; $(CC) $(CFLAGS) -o $@ $(circles)
lists:		$(lists)	; $(CC) $(CFLAGS) -o $@ $(lists)

include ../make/Makefile.$(OSTYPE)
