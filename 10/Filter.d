// new(Filter(), delegate)

typedef void (* flagM) (void *, void *, char);
typedef int (* nameM) (void *, const void *, const char *);
typedef int (* fileM) (void *, const void *, const char *,
													FILE *);
typedef int (* lineM) (void *, const void *, const char *,
													char *);
typedef int (* wrapM) (void *, const void *, const char *);
typedef int (* quitM) (void *, const void *);

% Class  Filter: Object {
	Object @ delegate;
	flagM flag;				// process a flag
	nameM name;				// process a filename argument
	fileM file;				// process an opened file
	lineM line;				// process a line buffer
	wrapM wrap;				// done with a file
	quitM quit;				// done with all files

	const char * progname;	// argv[0]
	char ** argv;			// current argument and byte

	char * buf;				// dynamic line buffer
	unsigned blen;			// current maximum length
%
	int mainLoop (_self, char ** argv);
	const char * argval (_self);
	const char * progname (const _self);
	int doit (_self, const char * arg);
%}
