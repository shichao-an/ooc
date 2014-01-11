#	main.awk -- 5.8 Sep 27 15:58:56 1993
#	Copyright (c) 1993 Axel T. Schreiner

# preprocessor implementation (last module)

# private:
#	preprocess(file)		preprocess a source file
#					preprocess a method header
#	methodHeader (class, method, tag)

BEGIN {					# usage is checked by shell script
	for (a = 1; a < ARGC; )
	{	if (ARGV[a] ~ /^-D/)		# -Dname[=value]
			define(ARGV[a])

		else if (ARGV[a] ~ /\.rep$/)	# load *.rep
			loadReports(ARGV[a])

						# preprocess - or *.dc
		else if (ARGV[a] ~ /\.dc$/ || ARGV[a] == "-")
			preprocess(ARGV[a])

						# generate -dc -h -r
		else if (ARGV[a] ~ /^-([hr]|dc)$/)
		{	setClass(Replace["`desc"])
			gen(ARGV[a])
		}

		else if (ARGV[a] !~ IdP)	# rest must be class
			fatal(ARGV[a] " ?invalid description name")
		else
		{	load(Replace["`desc"] = ARGV[a])
			if (makefile)		# generate -M
			{	setClass(Replace["`desc"])
				gen("-M")
			}
		}

		delete ARGV[a ++]
	}
	if (! debug)
		exit status
}

function define (arg,    name) {		# Replace[] += -Dname[=val]
	sub(/^-D/, "", arg)

	name = arg
	if (sub(/=.*$/, "", name))		# if there is a value
		sub(/^[^=]*=/, "", arg)		# leave it in arg
	else
		arg = ""			# otherwise clear arg

	if (name == "")
		fatal("-D" arg " ?invalid definition")

	Replace["`"name] = arg
}
	
function preprocess (fnm,    line, n, v, m, c) {
	openFile(fnm)			# push

	setClass(Replace["`desc"])
	gen("include")

	while ((line = get()) != EOF)
	{	if (line !~ /^%/)
		{	puts(line)
			continue
		}

		if ((n = split(line, v)) == 1)	# %casts %init
		{	if (v[1] ~ /^%(casts|init)$/)
			{	gen(substr(v[1], 2))
				continue
			}
		}
					# permitting %+ %- (anachronism)
		else if (v[1] !~ /^%[-+]?$/ || v[n] != "{")
			;
		else if (n == 3)	# % method {
		{	if (Linkage[m = v[2]] == "%")
			{	methodHeader(Owner[m], m)
				continue
			}
			c = Replace["`desc"]
			error(m " ?assuming " Linkage[m] " " c)
			methodHeader(c, m, Tag[m])
			continue;
		}
		else if (n == 4)	# % class method {
		{	c = v[2]
			if (Linkage[m = v[3]] == "%")
			{	if (c != Owner[m])
					error(m " ?declared % " c)
				methodHeader(Owner[m], m)
				continue
			}
			methodHeader(c, m, Tag[m])
			continue
		}
					# % [tag]: class method {
		else if (n == 5 && v[2] ~ /:$/)
		{	c = v[3]
			if (Linkage[m = v[4]] == "%")
			{	error(m " ?no tag, declared % " c)
				methodHeader(Owner[m], m)
				continue
			}
			sub(/:$/, "", v[2])
			if (v[2] != "" && v[2] !~ IdP)
				error(v[2] " ?invalid tag")
			methodHeader(c, m, v[2])
			continue
		}

		error(line " ?invalid request")
	}

	closeFile()			# pop
}

function methodHeader (class, method, tag,    owner) {
	if (! ((class, 0) in Class))
		fatal(class " ?unknown class")
	if (! (method in Method))
		fatal(method " ?unknown method")

	if (Linkage[method] != "%")
	{	owner = Linkage[method] == "%-" ? class : Meta[class]
		while (owner != Owner[method])
			if (owner != Replace["`root"])
				owner = Super[owner]
			else
			{	error(method " ?not defined for " class)
				break;
			}
	
		Links[class, ++ Links[class, 0]] = method
		Tag[method, class] = tag
	}

	setClass(class)
	setMethod(class, method)
	gen("methodHeader")
}
