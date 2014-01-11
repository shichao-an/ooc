chapter	= -7

c	= Object.c Point.c Circle.c List.c Queue.c Stack.c
d	= $(c:.c=)
h	= $(c:.c=.h)
m	= points.c circles.c queues.c stacks.c
r	= $(c:.c=.r)
t	= $(m:.c=)

points	= points.o Object.o Point.o
circles	= circles.o Object.o Point.o Circle.o
queues	= queues.o Object.o List.o Queue.o
stacks	= stacks.o Object.o List.o Stack.o

all:		$t
clean:				; rm -f $c $h $r $t *.o

test::		points		; points p
test::		circles		; circles p c
test::		queues		; queues +axel - +is +here . - . - .
test::		stacks		; stacks +axel - +is +here . - . - .

points:		$(points)	; $(CC) $(CFLAGS) -o $@ $(points)
circles:	$(circles)	; $(CC) $(CFLAGS) -o $@ $(circles)
queues:		$(queues)	; $(CC) $(CFLAGS) -o $@ $(queues)
stacks:		$(stacks)	; $(CC) $(CFLAGS) -o $@ $(stacks)

include ../make/Makefile.$(OSTYPE)
