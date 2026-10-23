#
#  Copyright (C) 2026  yt-mwm
#
#  This program is free software: you can redistribute it and/or modify
#  it under the terms of the GNU General Public License as published by
#  the Free Software Foundation, either version 3 of the License, or
#  (at your option) any later version.
#
#  This program is distributed in the hope that it will be useful,
#  but WITHOUT ANY WARRANTY; without even the implied warranty of
#  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
#  GNU General Public License for more details.
#
#  You should have received a copy of the GNU General Public License
#  along with this program.  If not, see <https://gnu.org>.
#



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
