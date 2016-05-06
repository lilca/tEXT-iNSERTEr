[日本語](README_jp.md)
What's tEXT-iNSERTEr
===

tEXT-iNSERTEr is the tools for inserting reference file as text data to the specified place in text file.  
tEXT-iNSERTEr was designed with strong focus on following:
  1. To work at various text file format
  2. To cover nesting structure of reference file
  3. Not to require regular expression  
tEXT-iNSERTEr has two commands (`tir` and `tirc`) to achieve above focus.  
`tir` is a tool written by C, for inserting the text data of reference file into the specified place of a input file.  
`tirc` is a tool written by shell script, for building Makefile of specified tir-file with using `tir`.  

Features
===

* base64

Copyright
===
tEXT-iNSERTEr was originally designed and developed by Masayuki Ise (lilca) in 2014.

Documents
===
[Please see 'doc' directory.](doc/contents.md)
