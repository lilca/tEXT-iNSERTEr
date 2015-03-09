事例
===
[目次に戻る](contents_jp.md)


##1. HTMLファイルの中に、HTMLファイルを挿入する

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

説明:
 * `ref`属性で指定されたファイル(test_sub.hmtl)の内容が挿入された。
 * 無効な属性では何もしない（例えば、上記の`background`）。
 * 指定されたファイルが存在しなかったとき、`tir`は`msg`属性でメッセージを挿入する。
 * 出力ファイル名は、入力ファイル名から`.tir`拡張子が取り除かれた名称になる。
 * `base64`が`convert`属性で指定されたとき、参照ファイルは`base64`で挿入される。
 * 現在の日時が、`>date`の参照によって、h2タグの中に挿入された。

[目次に戻る](contents_jp.md)
