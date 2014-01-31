tir (tEXT iNSERTEr)
===

```
tir <input file> [-o <output file>] [-bw <beginning word>] [-ew <ending word>] [-y] [-makefile] [-h]
```
What's tir?
===

`tir` is a program for inserting the data of specified file into the specified place of a input file.

Install
===
```
$make install
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
$tir test.html.tir -y
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
 
Deference of `sed` command
===
You think that, `tir` command is not necessary because the combinations of `sed` command & shell script are able to give same result?

I answer you that "That's right! Maybe. I don't know the specific method.".

I designed as complicate descriptions are not necessary by `tir`.

If you prefer `tir`, please use and improvement(e.g. Pull Requests).
