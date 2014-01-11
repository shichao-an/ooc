c	= new.c Point.c Circle.c
h	= $(c:.c=.h)
m	= points.c circles.c
r	= $(c:.c=.r)
t	= $(m:.c=)

points	= points.o new.o Point.o
circles	= circles.o new.o Point.o Circle.o

all:		$t
clean:				; rm -f $t *.o

test::		points		; points p
test::		circles		; circles p c

points:		$(points)	; $(CC) $(CFLAGS) -o $@ $(points)
circles:	$(circles)	; $(CC) $(CFLAGS) -o $@ $(circles)

include ../make/Makefile.$(OSTYPE)
