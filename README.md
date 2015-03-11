[日本語](README_jp.md)
What's tEXT-iNSERTEr
===

tEXT-iNSERTEr is the tools for inserting reference file as text data to the appointed place in text file.  
tEXT-iNSERTEr includes `tir` and `tirc`.  
`tir` is a tool for inserting the text data of reference file into the specified place of a input file.  
`tirc` is tool for building Makefile of specified tir-file with using `tir` command.  

Copyright
===
tEXT-iNSERTEr was originally designed and developed by Masayuki Ise (lilca) in 2014.

Concept
===
To cover various text file format.  
To in-line the description of insert instruction within comment statement.  
Failure message is included into comment statement.  

Documents
===
[Please see 'doc' directory.](doc/contents.md)

tEXT-iNSERTErとは、テキストファイルの指定した場所に、参照ファイルをテキストデータとして挿入するツールである。  
tEXT-iNSERTErは、以下のことを重視して設計された。  
　１．様々なテキストファイル形式で使えること  
　２．参照ファイルのネスティング（入れ子）構造を扱えること  
　３．正規表現を記述できなくても使えること  
これらを実装するためにOS Xで、`tir`と`tirc`を開発した。  
`tir`は、入力ファイルの指定された場所の中に、参照ファイルのテキストデータを挿入するためのC言語で書かれたツールである。  
`tirc`は、`tir`を使って、指定されたtirファイルのMakefileを作り上げるシェルスクリプトで書かれたツールである。  

Copyright
===
tEXT-iNSERTErは、Masayuki Ise(lilca)によって設計，開発された。(2014)

ドキュメント
===
['doc'ディレクトリを参照ください。](doc/contents_jp.md)
