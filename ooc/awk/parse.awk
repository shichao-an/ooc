#	parse.awk -- 5.7 Sep  1 10:40:56 1993
#	Copyright (c) 1993 Axel T. Schreiner

# class description file parser

#	load(desc)			load class description file desc.d
#	setClass(c)			load class c into Replace[]	
#	setMethod(c,m)			load method m, Tag[m,c] into Replace[]
#	setDeclarator(d, cast)		load declarator d into Replace[]
# private:
#	classDeclaration(desc, line)	parse class declaration
#	structDeclarator(line)		parse class component, return index
#				parse method declaration, return method name:
#	methodDeclaration(class, line, "%")	static
#	methodDeclaration(class, line, "%-")	dynamic
#	methodDeclaration(class, line, "%+")	class
#	declarator(text)		parse declarator, return index

BEGIN {					# global database
	Pub[DESC, 0] = 0		# Pub[desc, 1 .. Pub[desc,0]]
	Prot[DESC, 0] = 0		# Prot[desc, 1 .. Prot[desc,0]]
	Dcl[DESC, 0] = 0		# Dcl[desc, 1 .. Dcl[desc,0]]
					# lines in desc.d
					# all (desc, 0) are in Dcl[]
	if (lines)
	{	Fnr[""] = 0		# FNR/FILENAME of
		Filename[""] = ""	#    class/method
		SFnr[""] = 0		#    struct
		SFilename[""] = ""
	}
}

BEGIN {					# database of classes by name
	Class[CLASS, 0] = 0		# Class[C, 1 .. Class[C, 0]]
					# C's class method names
					# all (class name, 0) are in Class[]
	Dynamic[CLASS, 0] = 0		# C's dynamic methods
	Static[CLASS, 0] = 0		# C's static methods
	Struct[CLASS, 0] = 0		# C's components' declarator indices
	Meta[CLASS] = ""		# C's meta class name
	Super[CLASS] = ""		# C's super class name
}

BEGIN {					# database of methods by name
	Method[METHOD] = 0		# index of M's declarator
					# M's parameters' declarators follow
					# all method names are in Method[]
	Owner[METHOD] = ""		# class of M's self
	Nparm[METHOD] = 0		# M's number of parameters before ...
	Var[METHOD] = 0			# true if M permits ...
	Linkage[METHOD] = ""		# M's linkage: "%" "%+" "%-"
	Tag[METHOD] = ""		# default for tag
	Tag[METHOD, CLASS] = ""		# actual tag
}

BEGIN {					# database of declarators by index
	D = 0				# next index in Type[], etc.
	Type[D] = ""			# type part inside const .* @
					# "" if defined equal to Owner[class]
	TypeP = "^[A-Za-z_0-9][A-Za-z_0-9* ]* "
	Const[D] = ""			# "const " if leading const
	As[D] = 0			# true if trailing @
	Name[D] = ""			# name part
}

function setClass (c) {
	Replace["`class"] = c
	Replace["`super"] = Super[c]
	Replace["`meta"] = Meta[c]
	Replace["`supermeta"] = Super[Meta[c]]
}

function setMethod (class, m,    d, l) {
	Replace["`method"] = m
	d = Method[m]
	Replace["`result"] = Const[d] \
		(As[d] ? "struct " Type[d] " *" : Type[d])
	Replace["`tag"] = Tag[m, class]
	Replace["`linkage"] = Linkage[m]
	if (Var[m])
	{	Replace["`,..."] = ", ..."
		l = d + Nparm[m]
		Replace["`_last"] = (As[l] && Type[l]=="" ? "_" : "") Name[l]
	}
	else
		Replace["`,..."] = ""
}

function setDeclarator (d, cast) {
	Replace["`name"] = Name[d]
	Replace["`const"] = Const[d]
	if (As[d])
	{	Replace["`type"] = "void *"
		if (Type[d] == "")
		{	Replace["`_"] = "_"
			Replace["`cast"] = cast
		}
		else
		{	Replace["`_"] = ""
			Replace["`cast"] = Type[d]
		}
	}
	else
	{	Replace["`type"] = Type[d]
		Replace["`_"] = ""
		Replace["`cast"] = ""
	}
}

# class description file:
#	...				in Pub[] collected as is
#	%prot
#	...				in Prot[] collected as is
#	% meta class:super {			collected as % class
#	% meta:supermeta class:super {		collected as % meta class
#	% MetaRoot Root {			collected as % Root % MetaRoot
#					Dcl[] only contains class lines

function load (desc,    prot, line) {
	if (! ((desc, 0) in Dcl))
	{	Dcl[desc, 0] = 0	# once only
		openFile(desc ".d")	# push

		while ((line = get()) != EOF)
			if (line !~ /^%/)
				if (prot)
					Prot[desc, ++ Prot[desc, 0]] = line
				else
					Pub[desc, ++ Pub[desc, 0]] = line
			else if (line == "%prot")
				prot = 1
			else
			{	classDeclaration(desc, line)
				prot = 0
			}
	
		closeFile()		# pop
	}
}

# classDeclaration:
#	% meta class:super {			new class
#	% meta:supermeta class:super {		new meta class and new class
#	% MetaRoot Root {			very special case
#		components
#	%
#		methodDeclaration		statically bound
#		...
#	%-
#		methodDeclaration		dynamically bound
#		...
#	%+
#		methodDeclaration		class bound
#		...
#	%}

function classDeclaration (desc, line,    n, v, i, class, meta) {
	if (! sub(/[ \t]*{$/, "", line))
		fatal(line " ?invalid class header")
	sub(/^%[ \t]*/, "", line)

	n = split(line, v, "[: \t]+")	# % meta[:supermeta] class[:super] {
	if (n < 2 || n > 4)
		fatal(line " ?invalid class header")
	for (i = 1; i <= n; ++ i)
		if (v[i] !~ IdP)
			fatal(v[i] " ?invalid identifier")

	if (n == 3)			# % meta class:super {
	{	class = v[2]
		meta = ""		# no meta class defined here
		Meta[class] = v[1]
		Super[class] = v[3]
		# done with v[]

		Pub[desc, ++ Pub[desc, 0]] = "% " class
		Prot[desc, ++ Prot[desc, 0]] = "% " class
		Dcl[desc, ++ Dcl[desc, 0]] = "% " class

					# load unknown(!) superclass
		if (! ((Super[class], 0) in Class))
			load(Super[class])

		if (Meta[class] != Meta[Super[class]])
			error(class " ?meta != meta(super(class))")
	}
	else if (n == 4)		# % meta:supermeta class:super {
	{	class = v[3]
		meta = v[1]		# meta class defined here
		Meta[class] = meta
		Super[meta] = v[2]
		Super[class] = v[4]
		# done with v[]

		Pub[desc, ++ Pub[desc, 0]] = "% " meta " " class
		Prot[desc, ++ Prot[desc, 0]] = "% " meta " " class
		Dcl[desc, ++ Dcl[desc, 0]] = "% " meta " " class

					# load unknown(!) superclass
		if (! ((Super[class], 0) in Class))
			load(Super[class])

		if ((meta, 0) in Class)
			fatal(meta " ?duplicate class name")
		if (meta in Method)
			fatal(meta " ?method name")

		if (Super[meta] != Meta[Super[class]])
			error(class " ?super(meta) != meta(super(class))")

		Class[meta, 0] = 0	# declare meta class
		Meta[meta] = Replace["`metaroot"]
		if (lines)
		{	Fnr[meta] = FNR
			Filename[meta] = FILENAME
		}
	}
	else if ("`root" in Replace)	# n == 2: special case
		fatal(v[2] " ?multiple root class")
	else				# % MetaRoot Root {
	{	meta = v[1]
		class = v[2]
		# done with v[]

		Replace["`root"] = class
		Replace["`metaroot"] = Meta[class] = meta
		Super[class] = class	# exactly for root class

		Pub[desc, ++ Pub[desc, 0]] = "% " class
		Prot[desc, ++ Prot[desc, 0]] = "% " class
		Dcl[desc, ++ Dcl[desc, 0]] = "% " class

		if (meta in Method)
			fatal(meta " ?method name")
	}

	if (class == Meta[class] && class != Replace["`metaroot"])
		fatal(class " ?invalid meta class")

	if (! ((Meta[class], 0) in Class) && class != Replace["`root"] \
	    && class != Replace["`metaroot"])
		fatal(Meta[class] " ?unknown class")

	if (! ((Super[class], 0) in Class) && class != Replace["`root"])
		fatal(Super[class] " ?unknown class")

	for (i in v)
		delete v[i]
	for (i = class; i != Replace["`root"]; i = Super[i])
		if (i in v)
			fatal(i " ?loop in super class chain")
		else
			v[i] = 1

	if (meta != "")
	{	for (i in v)
			delete v[i]
		for (i = meta; i != Replace["`metaroot"]; i = Super[i])
			if (i in v)
				fatal(i " ?loop in super class chain")
			else
				v[i] = 1
	}

	if ((class, 0) in Class)
		fatal(class " ?duplicate class name")
	if (class in Method)
		fatal(class " ?method name")

	Class[class, 0] = 0		# declare class
	if (lines)
	{	Fnr[class] = FNR
		Filename[class] = FILENAME
	}

	while ((line = get()) !~ /^%/)	# collect representation
		if (line != "")
			Struct[class, ++ Struct[class, 0]] = \
				structDeclarator(line)

	if (line == "%")
		while ((line = get()) !~ /^%/)
			if (line != "")
				Static[class, ++ Static[class, 0]] = \
					methodDeclaration(class, line, "%")

	if (line == "%-" && meta != "" && class != Replace["`metaroot"])
		while ((line = get()) !~ /^%/)
			if (line != "")
				Dynamic[class, ++ Dynamic[class, 0]] = \
					methodDeclaration(class, line, "%-")

	if (line == "%+" && meta != "" && class != Replace["`metaroot"])
		while ((line = get()) !~ /^%/)
			if (line != "")
				Class[class, ++ Class[class, 0]] = \
					methodDeclaration(meta, line, "%+")

	if (line !~ /^%}/)
		fatal("?expecting %}")

	return meta
}

# structDeclarator:
#	declarator;

function structDeclarator (line,    n, d) {
	gsub(/[@;]/, " & ", line)	# tokenize delimeters
	gsub(/	+/, " ", line)		# tabs -> blank
	gsub(/  +/, " ", line)		# many blanks -> blank
	sub(/^ /, "", line)		# no leading blank

	if (line !~ /^[^;]+ ; $/)
		fatal(line " ?not a component")

	sub(/ ; /, "", line)		# line = declarator

	n = Name[d = declarator(line)]
	if (lines)
	{	SFnr[n] = FNR
		SFilename[n] = FILENAME
	}

	if (n in Method && Linkage[n] != "%")
		fatal(n " ?linked method name")
	if ((n, 0) in Class)
		fatal(n " ?class name")

	return d			# result is declarator
}

# methodDeclaration:
#	[tag:] declarator ( declarator { , declarator } [ , ... ] );

function methodDeclaration (class, line, linkage,    n, i, v, m, d, self) {
	gsub(/[:(,)*@]/, " & ", line)	# tokenize delimeters
	gsub(/	+/, " ", line)		# tabs -> blank
	gsub(/  +/, " ", line)		# many blanks -> blank
	sub(/^ /, "", line)		# no leading blank

	if (n = index(line, ": "))
	{	i = substr(line, 1, n)	# i is tag, ends in [ ]?:
		sub(/^[^:]*: /, "", line)
		if (linkage != "%-")
			error(i " ?tag only for %-")
	}
	else
		i = ""

	sub(/ $/, "", line)		# no trailing blank

	if (line !~ /^[^()]+ \( [^()]+ \) ;$/)
		fatal(line " ?not a method header")

	sub(/\(/, ",", line)		# v = declarators
	sub(/ \) ;/, "", line)
	n = split(line, v, " , ")

	m = Name[d = declarator(v[1])]
	if (Type[d] == "")
		fatal(m " ?no result type")
	if (m in Method)
		fatal(m " ?duplicate method name")
	if ((m, 0) in Class)
		fatal(m " ?class name")

	if ((Linkage[m] = linkage) == "%-")
		if (i == "")		# default tag is name
			Tag[m] = Name[d]
		else
		{	sub(/ ?:$/, "", i)
			if (i != "" && i !~ IdP)
				error(i " ?invalid default tag")
			Tag[m] = i
		}
		
	Method[m] = d
	if (lines)
	{	Fnr[m] = FNR
		Filename[m] = FILENAME
	}
	Owner[m] = class
	Nparm[m] = n-1
	if (Var[m] = v[n] == "...")	# if m permits ...
	{	n --			# need to clip it from list
		Nparm[m] --
	}

	self = 0			# m's parameters' declarators
	for (i = 2; i <= n; ++ i) 
		if (Name[d = declarator(v[i])] == "self")
		{	if (self)
				error(m " ?duplicate self")
			if (! As[d])
				error(m " ?self without @")
			if (Type[d] != "")
				error(m " ?self with foreign class")
			self = i
		}
	if (! self)
		error(m " ?no self")

	return m			# result is method name
}

# declarator:	[const] _IdP		object in current class
#		[const] IdP @ IdP	other object
#		[const] TypeP IdP	other data item

function declarator (line,    i) {
	Const[D] = sub(/^const /, "", line) ? "const " : ""

	if (line ~ /^_/)
	{	Type[D] = ""		# "" marks Owner[]
		As[D] = 1		# != 0 marks object
		Name[D] = substr(line, 2)
		if (Name[D] ~ IdP)
			return D ++	# result is declarator index
	}
	else if ((i = index(line, " @ ")) > 1)
	{	Type[D] = substr(line, 1, i-1)
		As[D] = 1
		Name[D] = substr(line, i+3)
		if (Type[D] ~ IdP && Name[D] ~ IdP)
			return D ++	# result is declarator index
	}
	else if (match(line, TypeP))	# find last blank
	{	Type[D] = substr(line, 1, RLENGTH-1)
		As[D] = 0
		Name[D] = substr(line, RLENGTH+1)
		if (Name[D] ~ IdP)
			return D ++	# result is declarator index
	}

	error(line " ?invalid declarator")
	Type[D] = "BAD"
	As[D] = 0
	Name[D] = "bad"
	return D ++			# result is (fake) declarator index
}
