# Makefile for the building "tir" on gcc

all: tir

tir: tir.c
	gcc -o tir tir.c

clean:
	rm -f tir

test:
	tir test.html.tir -y

#install: tir
#	install -s tir /usr/local/bin