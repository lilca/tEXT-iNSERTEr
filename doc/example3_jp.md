事例
===
[目次に戻る](contents_jp.md)

##3. コンフィグ値の挿入

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

説明:
 * コンフィグファイルによって定義された値は、"#[strings]page_tile"(or "#content")と記述された場所に展開された。

[目次に戻る](contents_jp.md)
