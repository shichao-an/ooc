p	= 01 02 03 04 05 06 07 08 09 10 11 12 13 14

all clean test::
	@ for p in $p; do \
	    if [ -d $$p -a -r $$p/makefile ]; then \
	      echo; echo make $@ in $$p; echo; \
	      (cd $$p && $(MAKE) $(MFLAGS) $@) \
	    fi; \
	  done

include make/Makefile.$(OSTYPE)
