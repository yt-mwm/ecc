CC = gcc
CFLAGS = -Wall -Wextra -O2

target = ecc

OBJS = main.o lexer.o parser.o

all: $(target)

$(target): $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) -o $(target)

main.o: main.c parser.h lexer.h
	$(CC) $(CFLAGS) -c main.c

parser.o: parser.c parser.h lexer.h
	$(CC) $(CFLAGS) -c parser.c

lexer.o: lexer.c lexer.h
	$(CC) $(CFLAGS) -c lexer.c

clean:
	rm -f $(OBJS) $(target)

install:
	cp ecc /usr/local/bin/

uninstall:
	rm /usr/local/bin/ecc

.PHONY: all clean install uninstall
