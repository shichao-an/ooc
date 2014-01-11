chapter	= -11

c	= Object.c List.c Exception.c Node.c Symbol.c Symtab.c
d	= $(c:.c=)
h	= $(c:.c=.h) binary.h parse.h
m	= except.c binary.c parse.c
r	= $(c:.c=.r)
t	= except value

except	= except.o Object.o List.o Exception.o
value	= parse.o binary.o $(c:.c=.o)

all:		$t
clean:		; rm -f $c $(c:.c=.h) $r $t classes.? *.stb *.sym *.o
		  rm -rf *.dSYM

test::	except	;  except
test::	value	;  echo 'let a = sin(1) + PI' | value
test::	value	;  { echo 'def sqr = $$ * $$'; \
		     echo 'def one = sqr(sin($$)) + sqr(cos($$))'; \
		     echo 'let n = one(10)'; \
		     echo save; } | value
test::	value	;  { echo load; \
		     echo 'n + one(20)'; } | value
test::	value	;  { echo let a = 1; \
		     echo def f = a + 1; \
		     echo save f; } | value
test::	value	;- { echo 'def a = 10 - $$'; \
		     echo load f; \
		     echo 'a(3)'; } | value

except:	$(except) ; $(AWK) -f ../09/munch < /dev/null > classes.c
		    $(CC) $(CFLAGS) -o $@ $(except) classes.c
value:	$(value)  ; nm -p $(value) | $(AWK) -f ../09/munch > classes.c
		    $(CC) $(CFLAGS) -o $@ $(value) classes.c $(Lm) $(debug)

include ../make/Makefile.$(OSTYPE)
