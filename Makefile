# Makefile for the building "tir" on gcc
#DEBUG_OP	= -fsanitize=address
DEBUG_OP	=
CC				= gcc
CFLAGS		=
LDFLAGS		=
CXX				=
CXXFLAGS	=
CPP				=
CPPFLAGS	=
# ----------------------------------------------------------------------
all: bin/tir

bin/tir: src/tir.o src/base64convert.o src/tirconfig.o src/opt_strcat.o
	$(CC) $(DEBUG_OP) $(LDFLAGS) -o bin/tir src/tir.o src/base64convert.o src/tirconfig.o src/opt_strcat.o

src/tir.o: src/tir.c src/tir.h
	$(CC) $(DEBUG_OP) $(CFLAGS) -c src/tir.c -o src/tir.o

src/base64convert.o: src/base64convert.c src/base64convert.h
	$(CC) $(DEBUG_OP) $(CFLAGS) -c src/base64convert.c -o src/base64convert.o

src/tirconfig.o: src/tirconfig.c src/tirconfig.h
	$(CC) $(DEBUG_OP) $(CFLAGS) -c src/tirconfig.c -o src/tirconfig.o

src/opt_strcat.o: src/opt_strcat.c src/opt_strcat.h
	$(CC) $(DEBUG_OP) $(CFLAGS) -c src/opt_strcat.c -o src/opt_strcat.o

tirc: src/tirc.test
	cp src/tirc.test bin/tirc

ex1:
	tir ./example1/test.html.tir -y

ex2:
	tirc ./example2
	make -f ./example2/Makefile

ex3:
	tirc ./example3
	make -f ./example3/Makefile

clean:
	rm -f src/*.o

install:
ifdef DESTDIR
	install -s bin/tir $(DESTDIR)$(PREFIX)/bin
	install bin/tirc $(DESTDIR)$(PREFIX)/bin
else
	install -s bin/tir /usr/local/bin
	install bin/tirc /usr/local/bin
endif
