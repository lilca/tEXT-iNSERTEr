# Makefile for the building "tir" on gcc

all: tir

tir: src/tir.o src/base64convert.o src/tirconfig.o
	gcc -o tir src/tir.o src/base64convert.o src/tirconfig.o

src/tir.o: src/tir.c src/tir.h
	gcc -c src/tir.c

src/base64convert.o: src/base64convert.c src/base64convert.h
	gcc -c src/base64convert.c

src/tirconfig.o: src/tirconfig.c src/tirconfig.h
	gcc -c src/tirconfig.c

clean:
	rm -f tir

ex1:
	tir ./example1/test.html.tir -y

ex2:
	tirc ./example2
	make -f ./example2/Makefile

ex3:
	tirc ./example3
	make -f ./example3/Makefile

install: tir
	install -s tir /usr/local/bin
	cp tirc /usr/local/bin