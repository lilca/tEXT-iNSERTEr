# Makefile for the building "tir" on gcc

all: tir

tir: tir.o base64convert.o
	gcc -o tir tir.o base64convert.o

tir.o: tir.c tir.h
	gcc -c tir.c

base64convert.o: base64convert.c base64convert.h
	gcc -c base64convert.c

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

