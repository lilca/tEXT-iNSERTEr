Case examples
===
[back to Contents](contents.md)

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
 * Defined value by the configuration file has been expanded to the location described as "#[strings]page_tile"(or "#content").

[back to Contents](contents.md)
