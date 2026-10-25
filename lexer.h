/*
 *  Copyright (C) 2026  yt-mwm
 *
 *  This program is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program.  If not, see <https://gnu.org>.
 */

typedef enum {
                        //                              num
	TOKEN_COLON,        // ':'                          0
	TOKEN_STR,          // '"'                          1
	TOKEN_OPAR,         // '('                          2
	TOKEN_CPAR,         // ')'                          3
	TOKEN_EQU,          // '='                          4
	TOKEN_PLUS,         // '+'                          5
	TOKEN_MINUS,        // '-'                          6
	TOKEN_RSHT,         // ">>"                         7 
	TOKEN_LSHT,         // "<<"                         8 
	TOKEN_CMP,          // "=="                         9 
	TOKEN_NCMP,         // "!="                         10
	TOKEN_INT,          // int type                     11
	TOKEN_WORD,         // char type                    12
	TOKEN_VAR,          // variable name                13
	TOKEN_VAL,          // value                        14
	TOKEN_IF,           // if operator                  15
	TOKEN_WORKIF,       // while loop                   16
	TOKEN_RETURN,       // return                       17
	TOKEN_EOF,          // end of file                  18
	TOKEN_ERR,          // incorrect token              19
	TOKEN_C,            // can write C in Eblang :)     20
	TOKEN_IGNORE,       // ignore a string in parser    21 
	TOKEN_TALK          // just print characters        22

} Tokenlist;

typedef struct {
	Tokenlist type;
	char text [256];
} Token;

int
check_space(char ch);

int
check_word(char name[], FILE *file);

int
special_char(char ch);

Token
write_token(Token token, int type, const char *str);

void
error(Token token);

Token
get_next_token(FILE *file);
