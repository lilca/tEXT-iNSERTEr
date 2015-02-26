all: ./example2/sub/sub.html ./example2/test.html

# Makefile info:
./example2/sub/sub.html: ./example2/sub/sub.html.tir ./example2/sub/hey.html
	tir './example2/sub/sub.html.tir' -o './example2/sub/sub.html' -bw '<!--[tir:begin]' -ew '[tir:end]-->' -y -cfg ./example2/tir.cfg

# Makefile info:
./example2/test.html: ./example2/test.html.tir ./example2/test_sub.html ./example2/sub/sub.html
	tir './example2/test.html.tir' -o './example2/test.html' -bw '<!--[tir:begin]' -ew '[tir:end]-->' -y -cfg ./example2/tir.cfg

