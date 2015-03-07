all: ./example3/test.html ./example3/tir.cfg

# Makefile info:
./example3/test.html: ./example3/test.html.tir ./example3/tir.cfg ./example3/sub/sub.html
	tir './example3/test.html.tir' -o './example3/test.html' -bw '<!--[tir:begin]' -ew '[tir:end]-->' -y -cfg ./example3/tir.cfg

# Makefile info:
./example3/tir.cfg: ./example3/tir.cfg.tir ./example3/constant_value.cfg
	tir './example3/tir.cfg.tir' -o './example3/tir.cfg' -bw ';[tir:begin]' -ew '[tir:end];' -y -cfg ./example3/tir.cfg

