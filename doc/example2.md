Case examples
===
[back to Contents](contents.md)

##2. Multi-File & Nesting

![](../images/example2-1.png)
![](../images/example2-2.png)

Command:
```
$ tirc ./example2
$ make -f ./example2/Makefile
```
or
```
$ make ex2
```

Makefile:
```
all: ./example2/sub/sub.html ./example2/test.html

# Makefile info:
./example2/sub/sub.html: ./example2/sub/sub.html.tir ./example2/sub/hey.html
	tir './example2/sub/sub.html.tir' -o './example2/sub/sub.html' -bw '<!--[tir:begin]' -ew '[tir:end]-->' -y -cfg ./example2/tir.cfg

# Makefile info:
./example2/test.html: ./example2/test.html.tir ./example2/test_sub.html ./example2/sub/sub.html
	tir './example2/test.html.tir' -o './example2/test.html' -bw '<!--[tir:begin]' -ew '[tir:end]-->' -y -cfg ./example2/tir.cfg
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
 * 'tirc' built Makefile for 'tir' files in the specified directory.(Caution about the overwriting Makefile)
 * A html tag '&lt;h1&gt;Hey!&lt;/h1&gt;' has achieved description of the nesting structure.

[back to Contents](contents.md)
