#	report.awk -- 5.6 Sep 27 15:48:01 1993
#	Copyright (c) 1993 Axel T. Schreiner

# report generator

#	loadReports(file)	get reports from file
#	gen(report)		generate report
#	puts(line)		emit line \n
# private:
#	endReport(brace)	check `{ `} balance, add `}
#	genGroup(t)		generate from Token[t] up to `}
#	genGroup(t, 1)		same and print , for `},
#					both return true if `}n ends group
#	genLineStamp(t)		process `#line item, return next position
#	fetch(t)		return (replaced) value of Token[t]
#	genLoopMethods(t, kind)	loop over methods in kind[]
#	genLoopLinks(class, t)	loop over Links[class, ]
#	genLoopParameters(t)	loop over parameters of current method
#	genLoopDescs(t, kind)	loop over lines in kind[]
#	genLoopStruct(class, t)	loop over Struct[class, ]
#	genLoopSuper(t)		loop over Super[class]
#					all 5 return true if `}n ends group
#	lf()			emit newline

BEGIN {					# interface to gen(): replacements
	Replace["`"] = ""		# ` denotes empty string
	Replace["``"] = "`"		# `` denotes one `
	Replace["`t"] = "\t"		# `t denotes tab
	Replace["`n"] = "\n"		# `n denotes newline
# private:
	newLine = 0			# 0: at left margin
					# -1: at left margin after blank line
					# 1: not at left margin
}

BEGIN {					# database of reports by name
	R = ""				# current (first) report name
	T = 0				# next index in Token[]
	Report[R] = T			# R's starting index in Token[]
					# all report names are in Report[]
	Token[T] = ""			# report tokens
}

function loadReports (fnm,    line, n, v, i, b, brace) {
					# updates database
	openFile(fnm)			# push
	R = ""				# no report yet, no `{ open
	while ((line = get()) != EOF)
		if (line ~ /^%/)	# % reportname
		{	endReport(brace) # complete previous report
			sub(/^%[ \t]*/, "", line)
			if (line == "")
				fatal("?no report name")
			if (line in Report)
				fatal(line " ?duplicate report name")
			R = line	# current report name
			Report[R] = T
			continue
		}
		else if (R == "")
			fatal("?no report name")
		else			# tokens
		{	n = split(line, v, "[ 	]")	# single space, tab
			for (i = 1; i <= n; ++ i)
				if ((Token[T ++] = v[i]) !~ /^`[{}]/)
					continue

#	a token starting in `{ begins a group
#	the token `} terminates each group
#	one extra token `} terminates the report
#	in Token[]: `{xx nn ... `} and  nn is the index past `}

				else if (v[i] ~ /^`{/)
				{	Token[T] = brace
					brace = T ++
				}
				else if (! brace)
					fatal("report " R " ?missing `{")
				else
				{	b = brace; brace = Token[brace]
					Token[b] = T
				}
		}

	endReport(brace)
	closeFile()			# pop
}

function endReport (brace) {
	if (brace)
		fatal(R " ?missing `}")
	Token[T ++] = "`}"
}

function gen (r) {
	if (! (r in Report))
		fatal(r " ?cannot find report")
	genGroup(Report[r])
}

function genGroup(t, more,    T, nl) {
	for (;;)
	{	if ((T = Token[t ++]) == "")		# "" --> space
		{	if (newLine == 1)
				printf " "
			continue
		}
		if (T !~ /^`/)				# other
		{	printf "%s", T
			newLine = 1
			continue
		}
		if (T == "`n")				# `n --> newline
		{	lf()
			continue
		}
		if (T == "`#line")			# `#line item
		{	genLineStamp(t ++)
			continue
		}
		if (T !~ /^`[%{}]/)			# `other
		{	if (! (T in Replace))
				fatal(T " ?cannot replace")
			if ((T = Replace[T]) != "")
			{	printf "%s", T
				newLine = 1
			}
			continue
		}
		if (T ~ /^`%/)				# `%report
		{	gen(substr(T, 3))
			continue
		}
		if (T ~ /^`}/)				# `}
		{	if (T == "`}," && more)		# `},
			{	printf ", "
				newLine = 1
			}
			return T == "`}n"		# `}n
		}

		if (T ~ /^`{if(not)?$/)			# `{if `{ifnot
		{	if ((fetch(t+1) == fetch(t+2)) == (T == "`{if"))
				nl = genGroup(t+3)	# execute if-part
			else if (Token[t = Token[t]] == "`{else")
				nl = genGroup(++ t + 1)	# execute else-part
			else
				continue		# no else-part
		}
		else if (T == "`{else")			# `{else
			nl = 0;				# skipped (after if)
		else if (T == "`{%")			# `{%
			nl = genLoopMethods(t+1, Static)
		else if (T == "`{%+")			# `{%+
			nl = genLoopMethods(t+1, Class)
		else if (T == "`{%-")			# `{%-
			nl = genLoopMethods(t+1, Dynamic)
		else if (T == "`{()")			# `{()
			nl = genLoopParameters(t+1)
		else if (T == "`{dcl")			# `{dcl
			nl = genLoopDescs(t+1, Dcl)
		else if (T == "`{links")		# `{links class
			nl = genLoopLinks(fetch(t+1), t+2)
		else if (T == "`{prot")			# `{prot
			nl = genLoopDescs(t+1, Prot)
		else if (T == "`{pub")			# `{pub
			nl = genLoopDescs(t+1, Pub)
		else if (T == "`{struct")		# `{struct class
			nl = genLoopStruct(fetch(t+1), t+2)
		else if (T == "`{super")		# `{super
			nl = genLoopSuper(t+1)
		else
			fatal(T " ?invalid request")

		if (nl)					# `}n --> \n if any
			lf()
		t = Token[t]
	}
}

function genLineStamp (t,    filename, fnr, f) {
	if (lines)
	{	fnr = FNR
		filename = FILENAME
		if ((f = fetch(t)) != "")
			if (f in Filename)
			{	fnr = Fnr[f]
				filename = Filename[f]
			}
			else if (f in SFilename)
			{	fnr = SFnr[f]
				filename = SFilename[f]
			}
					# could now be optimized...
		printf "# line %d \"%s\"\n", fnr, filename			
		newLine = 0
	}
}

function fetch (t) {
	if (t >= T)
		fatal(t " ?no more tokens")
	if (Token[t] !~ /^`/)
		return Token[t]
	if (Token[t] in Replace)
		return Replace[Token[t]]
	fatal(Token[t] " ?invalid request")
}

function genLoopMethods (t, kind,    c, n, i, nl) {
	c = Replace["`class"]; n = kind[c, 0]
	for (i = 1; i <= n; ++ i)
	{	setMethod(c, kind[c, i])
		nl = genGroup(t)
	}
	return nl
}

function genLoopLinks (class, t,    n, i, nl) {
	n = Links[class, 0]
	for (i = 1; i <= n; ++ i)
	{	setMethod(class, Links[class, i])
		nl = genGroup(t)
	}
	return nl
}

function genLoopParameters (t,    m, p, n, cast, nl) {
	m = Replace["`method"]; p = Method[m]; n = p + Nparm[m]
	if (Linkage[m] == "%+")
		cast = Replace["`meta"]
	else if (Linkage[m] == "%-")
		cast = Replace["`class"]
	else
		cast = Owner[m]
	while (++ p <= n)
	{	setDeclarator(p, cast)
		nl = genGroup(t, p < n)
	}
	return nl
}

function genLoopDescs (t, kind,    n, i, line, nl) {
	n = kind[Replace["`desc"], 0]
	for (i = 1; i <= n; ++ i)
	{	line = kind[Replace["`desc"], i]
		if (line !~ /^%/)
		{	Replace["`class"] = ""
			Replace["`line"] = line
		}
		else
		{	Replace["`newmeta"] = line ~ /^% .* /	# two names?
			sub(/^%.* /, "", line)
			setClass(line)
		}
		nl = genGroup(t)
	}
	return nl
}

function genLoopStruct (class, t,    n, i, nl) {
	n = Struct[class, 0]
	for (i = 1; i <= n; ++ i)
	{	setDeclarator(Struct[class, i], class)
		nl = genGroup(t)
	}
	return nl
}

function genLoopSuper (t,    class, nl) {
	class = Replace["`desc"]
	for (;;)
	{	setClass(class)
		nl = genGroup(t)
		if (class == Super[class])
			return nl
		class = Super[class]
	}
}

function puts (line) {
	if (line != "")
	{	print line
		newLine = 0
	}
	else
		lf()
}

function lf () {
	if (newLine >= 0)
	{	printf "\n"
		-- newLine
	}
}
