tir (tEXT iNSERTEr)
===
Contents
===
* [What's 'tir' and 'tirc'?](#whats-tir)
* [Install](#install)
* [Files](#files)
* [Syntax](#syntax)
* [What's 'tir'?](#whats-tir)
* [Help](#help)
* [Case examples](#case-examples)
  * [1. Into a HTML file, inserting HTML files](#1-into-a-html-file-inserting-html-files)
  * [2. Multi-File & Nesting](#2-multi-file--nesting)
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

Files
===
```
+  
+example1  
|　+test_sub.html  
|　+test.html  
|　+test.html.tir  
+example2  
|　+Makefile  
|　+sub  
|　|　+hey.html  
|　|　+sub.html  
|　|　+sub.html.tir  
|　+test_sub.html  
|　+test.html  
|　+test.html.tir  
+Makefile  
+README.md  
+tir  
+tir.c  
+tirc  
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
Usage:
  -o  : Output file
  -bw : Beginning word for inserting
  -ew : Ending word for inserting
        e.g. tir infile.c -o outfile.c -bw '//[tir:begin]' -ew '[tir:end]'
  -h  : Display help
  -y  : To override without asking
  -makefile:
        Output makefile info.
Default words:
  Suffix       | Begin           | End
  ---------------------------------------------
  html htm xml | <!--[tir:begin] | [tir:end]-->
  sh rb py r   | #[tir:begin]    | [tir:end]#
  other(e.g. c)| /*[tir:begin]   | [tir:end]*/
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
		<!--[tir:begin] ref="test_sub.html" convert="urlpaese" [tir:end]-->
	</head>
	<body>
		<!--[tir:begin] 	ref=test sss.html [tir:end]-->
		<!--[tir:begin]ref=test_sss3.html[tir:end]-->
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
		<!--[tir:begin] msg="Can't open a file:test" [tir:end]-->
		<!--[tir:begin] msg="Can't open a file:test_sss3.html" [tir:end]-->
	</body>
</html>
```

Explanations:
 * The contents of specified file(test_sub.hmtl) at `ref` attribute were inserted.
 * No action for invalid attributes(e.g. above `convert`).
 * When a specified file was not exist, `tir` inserts message at `msg` attribute.
 * Output file name became a name that was removed `.tir` suffix from input file name.

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

Deference of `sed` command
===
You think that,

**"`tir` command is not necessary because the combinations of `sed` command & shell script are able to give same result?"**

I answer you that "That's right! Maybe. But I don't know the specific method.".

I designed `tir` as complicated descriptions is not necessary.

If you prefer `tir`, please use and improvement(e.g. Pull Requests).

About license
===
Under review.
