Case examples
===
[back to Contents](contents.md)


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
		<!--[tir:begin] ref="test_sub.html" background="urlpaese" [tir:end]-->
	</head>
	<body>
		<!--[tir:begin] 	ref=test sss.html [tir:end]-->
		<!--[tir:begin]ref=test_sss3.html[tir:end]-->
		<img src="data:image/png;base64,<!--[tir:begin] ref="../testicon.png" convert="base64" [tir:end]-->"/>
		<h2><!--[tir:begin] ref=">date" [tir:end]--></h2>
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
		<!--[tir:begin] msg="Can't open a file:./example1/test" [tir:end]-->
		<!--[tir:begin] msg="Can't open a file:./example1/test_sss3.html" [tir:end]-->
		<img src="data:image/png;base64,iVBORw0KGg.....5ErkJggg=="/>
		<h2>2015年 3月 3日 火曜日 21時24分51秒 JST
</h2>
	</body>
</html>
```

Explanations:
 * The contents of specified file(test_sub.hmtl) at `ref` attribute were inserted.
 * No action for invalid attributes(e.g. above `background`).
 * When a specified file was not exist, `tir` inserts message at `msg` attribute.
 * Output file name became a name that was removed `.tir` suffix from input file name.
 * When `base64` were specified in `convert` attribute, reference file is inserted with `base64`.
 * Current date was inserted into h2 tag by the reference of ">date"

[back to Contents](contents.md)
