tir (tEXT iNSERTEr)
===

```
tir <input file> [-o <output file>] [-bw <beginning word>] [-ew <ending word>] [-y] [-makefile] [-h]
```
What's tir?
===

"tir" is a program for inserting the data of specified file into the specified place of a input file.

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
  -y  : To override without asking  -makefile:
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
*1. In HTML file, inserting HTML file
