# Makefile for the building "tir" on gcc

all: tir

tir: tir.c
	gcc -o tir tir.c

clean:
	rm -f tir

ex1:
	tir ./example1/test.html.tir -y

ex2:
	tirc ./example2
	make -f ./example2/Makefile

install: tir
	install -s tir /usr/local/bin
	cp tirc /usr/local/bin

