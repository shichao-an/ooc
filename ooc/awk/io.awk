#	io.awk -- 5.4 Jun 18 15:35:16 1993
#	Copyright (c) 1993 Axel T. Schreiner

# input/output file management functions

#	error(message)		message -> stderr, set exit status 1
#	fatal(message)		message -> stderr, exit 1
#	openFile(filename)	locate filename and push into FILENAME
#	get()			next full, uncommented line from FILENAME
#	closeFile()		pop FILENAME

BEGIN {	stderr = "/dev/stderr"		# diagnostic stream
	status = 0			# exit value

	EOF = "%%"			# returned by get() on end of file
					# must start with %
	IdP = "^[a-zA-Z][a-zA-Z0-9]*$"	# legal identifier (no underscore)

	FILENAME = "-"; FNR = 0		# current file, not yet initialized

					# openFile() gets files on OOCPATH[]
	if (OOCPATH[0] = split(ENVIRON["OOCPATH"], OOCPATH, ":"))
	{	for (i = 1; i <= OOCPATH[0]; ++ i)
			if (OOCPATH[i] == "")
				OOCPATH[i] = "./"
			else if (OOCPATH[i] !~ /\/$/)
				OOCPATH[i] = OOCPATH[i] "/"
	}
	else
		OOCPATH[OOCPATH[0] = 1] = "./"

	FileStack[0] = 0		# >0: FILENAME/FNR was pushed
}

function error (msg,    s) {
	for (s = 1; s < FileStack[0]; s += 2)
		if (FileStack[s] !~ /^-?$/)
			if (FileStack[s+1])
				printf "%s(%d): ", \
					FileStack[s], FileStack[s+1] > stderr
			else
				printf "%s: ", FileStack[s] > stderr

	printf "%s%s: %s\n", FILENAME, FNR ? "(" FNR ")" : "", msg > stderr
	status = 1
}

function fatal (msg) {
	error(msg)
	exit status = 1
}

function openFile (fnm,    i) {
	if (fnm ~ /\//)			# path with / must be found as such
	{	if (system("test -r " fnm) != 0)
			fatal(fnm " ?cannot open")
	}
	else if (fnm == "-")		# - is /dev/stdin
		fnm = "/dev/stdin"
	else				# a name is searched on OOCPATH
	{	for (i = 1; i <= OOCPATH[0]; ++ i)
			if (system("test -r " OOCPATH[i] fnm) == 0)
				break;
		if (i > OOCPATH[0])
			fatal(fnm " ?cannot find")
		fnm = OOCPATH[i] fnm
	}
	FileStack[++ FileStack[0]] = FILENAME
	FileStack[++ FileStack[0]] = FNR
	FILENAME = fnm ~ /^\.\// ? substr(fnm, 3) : fnm
	FNR = 0
}

function get (    input, i, line) {
	input = ""			# collates pending line
	while ((i = getline line < FILENAME) == 1)
	{	++ FNR

		if (line ~ /\/\//)	# comment: white* // any*
		{	sub(/[ \t]*\/\/.*$/, "", line)
			if (input line == "")
				continue # ignore comment lines
		}

		if (input != "")	# don't glue leading white
			sub(/^[ \t]+/, "", line)
		input = input line

		if (input ~ /\\$/)
		{	sub(/[ \t]*\\$/, " ", input)
			continue	# glue using one blank...
		}			# ...for white* backslash

		sub(/[ \t]+$/, "", input) # clip trailing white
		return input		# next line -- may be empty
	}
	if (i == -1)
		fatal("?cannot read")
	return EOF
}

function closeFile () {
	close(FILENAME)
	FNR = FileStack[FileStack[0] --]
	FILENAME = FileStack[FileStack[0] --]
}
