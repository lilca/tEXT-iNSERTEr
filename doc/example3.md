Case examples
===
[back to README](../README.md)

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

[back to README](../README.md)
