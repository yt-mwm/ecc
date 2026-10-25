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

#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <stdlib.h>
#include "lexer.h"

int
check_space(char ch)
{
	if(ch == ' ' || ch == '\t' || ch == '\n')
		return 1;
	
	return 0;
}

int
check_word(char name[], FILE *file)
{
	char buffer[256];
	int c;
	int i = 0;
	int size = strlen(name)-1;

	for(; i <= size;) {
		c = fgetc(file);
		if(c == EOF) {
			for(int j=i-1; j >= 0; j--)
				ungetc(buffer[j], file);
			return 0;
		}
		
		buffer[i] = c;

		if(c != name[i]) {
			for(int j=i; j >= 0; j--)
				ungetc(buffer[j], file);
			return 0;
		} i++;
	} 
	return 1;
}

int
special_char(char ch)
{
	if(ch == '(' || ch == ')' || ch == '"' || ch == '=' || ch == '.' || ch == '$' || ch == '#' || ch == ':')
		return 1;
	
	return 0;
}

Token
write_token(Token token, int type, const char *str)
{
	token.type = type;
	strcpy(token.text, str);
	return token;
}

void
error(Token token)
{
	token.type = TOKEN_ERR;
	strcpy(token.text, "err");
}

Token
get_next_token(FILE *file)
{
	Token token;
	token.text[0] = '\0';
	int c;
	
	while((c = fgetc(file)) != EOF && check_space(c));


	if(c == EOF)
		return write_token(token, TOKEN_EOF, "EOF");
	
	if(c == ':')
		return write_token(token, TOKEN_COLON, ":");

	if(c == '(') 
		return write_token(token, TOKEN_OPAR, "(");

	if(c == ')')
		return write_token(token, TOKEN_CPAR, ")");

	if(c == '+') 
		return write_token(token, TOKEN_PLUS, "+");

	if(c == '-') 
		return write_token(token, TOKEN_MINUS, "-");

	if(c == '>') { 
		ungetc(c, file);
		if(check_word(">>", file)) 
			return write_token(token, TOKEN_RSHT, ">>");

		c = fgetc(file);
		error(token);
		return token;
	}

	if(c == '<') { 
		ungetc(c, file);
		if(check_word("<<", file)) 
			return write_token(token, TOKEN_LSHT, "<<");
	
		c = fgetc(file);
		error(token);
		return token;
	}

	if(c == '_') {
		ungetc(c, file);
		if(check_word("_c_", file)) 
			return write_token(token, TOKEN_C, "_c_");
		
		c = fgetc(file);
		error(token);
		return token;
	}



	if(c == 't') {
		ungetc(c, file);
		if(check_word("talk", file)) 
			return write_token(token, TOKEN_TALK, "talk");

		c = fgetc(file);
		error(token);
		return token; 
	}

	if(c == 'e') {
		ungetc(c, file);
		if(check_word("eof", file)) 
			return write_token(token, TOKEN_EOF, "eof");

		c = fgetc(file);
		error(token);
		return token;
	}


	if(c == 'i') {
		ungetc(c, file);
		if(check_word("int", file)) 
			return write_token(token, TOKEN_INT, "int");

		if(check_word("if", file)) 
			return write_token(token, TOKEN_IF, "if");

		c = fgetc(file);
		error(token);
		return token;
	}

	if(c == 'w') {
		ungetc(c, file);
		if(check_word("word", file)) 
			return write_token(token, TOKEN_WORD, "word");

		if(check_word("workif", file)) 
			return write_token(token, TOKEN_WORKIF, "workif");

		c =fgetc(file);
		error(token);
		return token;
	}
	
	if(c == '#') {
		ungetc(c, file);
		if(check_word("#?", file)) {
			while((c = fgetc(file)) != '\n' && c != EOF) { }
			return get_next_token(file);
		}
		if(check_word("#!", file)) 
			return write_token(token, TOKEN_RETURN, "#!");

		c = fgetc(file);
		error(token);
		return token;
	}	

	if(c == '=') {
		ungetc(c, file);
		if(check_word("==", file)) 
			return write_token(token, TOKEN_CMP, "==");

		if(check_word("=!", file)) 
			return write_token(token, TOKEN_NCMP, "!=");

		c = fgetc(file);
		return write_token(token, TOKEN_EQU, "=");
	}


	if(c == ';') { 
		token.type = TOKEN_IGNORE;

		int i = 0;
		for(; i<254; i++) {
			c = fgetc(file);
			if(c == EOF || c == '\n') 
				break;
			token.text[i] = c;
		} token.text[i] = '\0';
	
		return token;
	}

	if(c == '"') { 
		token.type = TOKEN_STR;

		int i = 0;
		for(; i<254; i++) {
			c = fgetc(file);
			if(c == EOF || c == '"') 
				break;
			if(c == '`')
				c = '"';
			token.text[i] = c;
		} token.text[i] = '\0';
	
		return token;
	}

	if(c == '.') {
		token.type = TOKEN_VAR;

		int i = 0;
		for(; i<254; i++) {
			c = fgetc(file);
			if(c == EOF || check_space(c) || special_char(c)) {
				ungetc(c, file);
				break;
			}
			token.text[i] = c;
		} token.text[i] = '\0';
	
		return token;
	}

	if(c == '$') {
		token.type = TOKEN_VAL;

		int i = 0;
		for(; i<254; i++) {
			c = fgetc(file);
			if(c == EOF || check_space(c) || special_char(c)) {
				ungetc(c, file);
				break;
			}
			token.text[i] = c;
		} token.text[i] = '\0';
	
		return token;
	}

	error(token);
	return token;
}
