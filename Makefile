# Makefile for the building "tir" on gcc
#DEBUG_OP	= -fsanitize=address
DEBUG_OP	=
VERSION		= 0.5.3

# ----------------------------------------------------------------------
all: tir

tir: src/tir.o src/base64convert.o src/tirconfig.o
	gcc $(DEBUG_OP) -o tir src/tir.o src/base64convert.o src/tirconfig.o

src/tir.o: src/tir.c src/tir.h
	gcc $(DEBUG_OP) -c src/tir.c -o src/tir.o

src/base64convert.o: src/base64convert.c src/base64convert.h
	gcc $(DEBUG_OP) -c src/base64convert.c -o src/base64convert.o

src/tirconfig.o: src/tirconfig.c src/tirconfig.h
	gcc $(DEBUG_OP) -c src/tirconfig.c -o src/tirconfig.o

clean:
	rm -f tir
	rm -f ./src/*.o

ex1:
	tir ./example1/test.html.tir -y

ex2:
	tirc ./example2
	make -f ./example2/Makefile

ex3:
	tirc ./example3
	make -f ./example3/Makefile

pkg:
	tar zcvf ./dist/macports/tir-$(VERSION).tar.gz --exclude 'dist' ./*

install: tir
	install -s tir /usr/local/bin
	cp tirc /usr/local/bin