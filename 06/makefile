c	= Object.c Point.c Circle.c
h	= $(c:.c=.h)
m	= any.c points.c circles.c
r	= $(c:.c=.r)
t	= $(m:.c=)

any	= any.o Object.o
points	= points.o Object.o Point.o
circles	= circles.o Object.o Point.o Circle.o

all:		$h $r $c $m $t
clean:				; rm -f $t *.o

test::		any		; any
test::		points		; points p
test::		circles		; circles p c

any:		$(any)		; $(CC) $(CFLAGS) -o $@ $(any)
points:		$(points)	; $(CC) $(CFLAGS) -o $@ $(points)
circles:	$(circles)	; $(CC) $(CFLAGS) -o $@ $(circles)

include ../make/Makefile.$(OSTYPE)
