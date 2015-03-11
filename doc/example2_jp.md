事例
===
[目次に戻る](contents_jp.md)

##2. 複数ファイル＆ネスティング（入れ子）

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

説明:
 * 'tirc' creates Makefile for 'tir' files in the specified directory.(Caution about the overwriting Makefile)
 * A html tag '&lt;h1&gt;Hey!&lt;/h1&gt;' has achieved description of the nesting structure.

[目次に戻る](contents_jp.md)
