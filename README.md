tir (tEXT iNSERTEr)
===
Contents
===
* [What's 'tir' and 'tirc'?](#whats-tir)
* [Install](#install)
* [Syntax](#syntax)
* [What's 'tir'?](#whats-tir)
* [Help](#help)
* [Case examples](#case-examples)
  * [1. Into a HTML file, inserting HTML files](#1-into-a-html-file-inserting-html-files)
  * [2. Multi-File & Nesting](#2-multi-file--nesting)
  * [3. Inserting config values](#3-inserting-config-values)
* [Deference of `sed` command](#deference-of-sed-command)
* [About license](#about-license)
 
What's 'tir' and 'tirc'?
===

`tir` is a program for inserting the data of specified file into the specified place of a input file.  
`tirc` is a shell script for building Makefile of specified .tir file with using `tir` command.  

Install
===
```
$ sudo make install
```

Syntax
===
```
$ tir <input file> [-o <output file>] [-bw <beginning word>] [-ew <ending word>] [-y] [-makefile] [-h]
```
```
$ tirc <target directory>
```

Help
===

```
$ tir
```
or
```
$ tir -h
```

Case examples
===
##1. Into a HTML file, inserting HTML files

Command:
```
$ tir ./example1/test.html.tir -y
```
or
```
$ make ex1
```

test.html.tir:
```
<!DOCTYPE html>
<html>
	<head>
		<!--[tir:begin] ref="test_sub.html" background="urlpaese" [tir:end]-->
	</head>
	<body>
		<!--[tir:begin] 	ref=test sss.html [tir:end]-->
		<!--[tir:begin]ref=test_sss3.html[tir:end]-->
		<img src="data:image/png;base64,<!--[tir:begin] ref="../testicon.png" convert="base64" [tir:end]-->"/>
	</body>
</html>
```

test_sub.html:
```
<h1>Hello!!</h1>
```

result(test.html)
```
<!DOCTYPE html>
<html>
	<head>
		<h1>Hello!!</h1>
	</head>
	<body>
		<!--[tir:begin] msg="Can't open a file:./example1/test" [tir:end]-->
		<!--[tir:begin] msg="Can't open a file:./example1/test_sss3.html" [tir:end]-->
		<img src="data:image/png;base64,iVBORw0KGg.....5ErkJggg=="/>
	</body>
</html>
```

Explanations:
 * The contents of specified file(test_sub.hmtl) at `ref` attribute were inserted.
 * No action for invalid attributes(e.g. above `background`).
 * When a specified file was not exist, `tir` inserts message at `msg` attribute.
 * Output file name became a name that was removed `.tir` suffix from input file name.
 * When `base64` were specified in `convert` attribute, reference file is inserted with `base64`.

##2. Multi-File & Nesting

Command:
```
$ tirc ./example2
$ make -f ./example2/Makefile
```
or
```
$ make ex2
```
test.html.tir:
```
<!DOCTYPE html>
<html>
	<head>
		<!--[tir:begin] ref="test_sub.html" convert="urlpaese" [tir:end]-->
	</head>
	<body>
		<!--[tir:begin]  ref="./sub/sub.html" [tir:end]-->
	</body>
</html>
```

test_sub.html:
```
<h1>Hello!!</h1>
```
sub/sub.html.tir
```
<!--[tir:begin] ref="hey.html" [tir:end]-->
```
sub/hey.html
```
<h1>Hey!</h1>
```
result(test.html)
```
<!DOCTYPE html>
<html>
	<head>
		<h1>Hello!!</h1>

	</head>
	<body>
		<h1>Hey!</h1>
	</body>
</html>
```

Explanations:
 * 'tirc' creates Makefile for 'tir' files in the specified directory.(Caution about the overwriting Makefile)
 * A html tag '&lt;h1&gt;Hey!&lt;/h1&gt;' has achieved description of the nesting structure.

##3. Inserting config values

Command:
```
$ tirc ./example3
$ make -f ./example3/Makefile
```
or
```
$ make ex3
```
test.html.tir:
```
<!DOCTYPE html>
<html>
	<head>
		<title><!--[tir:begin] ref="#[strings]page_tile" [tir:end]--></title>
	</head>
	<body>
		<!--[tir:begin]  ref="#content" [tir:end]-->
		<!--[tir:begin] ref="sub/sub.html" [tir:end]-->
	</body>
</html>
```

tir.cfg
```
; No section
content=<p>Some content</p>
; Strings section
[strings]
page_tile=Example3
;
;[tir:begin] ref="constant_value.cfg" [tir:end]*/
```
sub/sub.html:
```
<h1>Hey!</h1>
```
result(test.html)
```
<!DOCTYPE html>
<html>
	<head>
		<title>Example3</title>
	</head>
	<body>
		<p>Some content</p>
		<h1>Hey!</h1>
	</body>
</html>
```

Explanations:
 * Value defined by the configuration file to the location described as "#[strings]page_tile"(or "#content") has been expanded.

Deference of `sed` command
===
I designed `tir` as complicated descriptions is not necessary.

If you prefer `tir`, please use and improvement(e.g. Pull Requests).

About license
===
Under review.
