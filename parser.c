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
#include <stdlib.h>
#include <string.h>
#include "lexer.h"
#include "parser.h"

Token token;
int forgc = 0;

void
match(unsigned int type, FILE *in, int debug_flag)
{
	forgc++;
	if(token.type == type) {
		token = get_next_token(in);
		if(forgc == 10) {
			forgc = 0;
			printf("Sorry, I forgot what the '%s' token means.\n", token.text);
			token = get_next_token(in);
		}
		if(debug_flag)
			printf("token:\t%d\t\'%s\'\n", token.type, token.text);
		return;
	}
		
	printf("\033[31mSyntax Error!!!\033[0m Your stupid head pass incorrect token \'%s\' when ECC wait a %d\n", token.text, type);
}

char*
data_types(int type)
{
	if(type == TOKEN_INT)
		return "int";
	
	else if(type == TOKEN_WORD)
		return "char";
	return "";
}

char*
get_sign(Token *token)
{
	if(token->type >= 4 && token->type <= 8)
		return token->text;
	return "";
}

char*
get_cmp(Token *token)
{
	if(token->type == 9 || token->type <= 10)
		return token->text;
	return "";

}

char*
var_val(Token *token)
{
	if(token->type == 13 || token->type == 14)
		return token->text;
	return "";
}


void
parse(FILE *in, FILE *out, int debug_flag, int nostd_flag)
{
	token = get_next_token(in);
	int ccount = 0;
	char buffer[255];
	char buffer2[255];
	char buffer3[255];
	char sign[3];
	char sign2[3];
	
	while(token.type != TOKEN_EOF) {
		if(debug_flag)
			printf("\n`%s`\n", token.text);	
		
		if(ccount > 10) {
			printf("You wrote to much C code.\n");
			exit(1);
		}

		if(token.type == 11 || token.type == 12) {			
			char type[5];
			strcpy(type, data_types(token.type));
			if(type[0] == '\0')
				goto SyntaxError;
			match(token.type, in, debug_flag);
			strcpy(buffer, token.text);
			match(TOKEN_VAR, in, debug_flag);
			
			if(token.type == TOKEN_EQU) {
				match(TOKEN_EQU, in, debug_flag);
				strcpy(buffer2, token.text);
				match(token.type, in, debug_flag);
				fprintf(out, "%s %s = %s;\n", type, buffer, buffer2);
				continue;
			}
			fprintf(out, "%s %s;\n", type, buffer);
		}

		else if(token.type == TOKEN_VAR) {
			strcpy(buffer, token.text);
			match(TOKEN_VAR, in, debug_flag);
			
			strcpy(sign, get_sign(&token)); 
			if(sign[0] == '\0')
				goto SyntaxError;

			match(token.type, in, debug_flag);
			strcpy(buffer2, var_val(&token));
			if(buffer2[0] == '\0')
				goto SyntaxError;

			match(token.type, in, debug_flag);
			fprintf(out, "%s %s %s", buffer, sign, buffer2);
				
			strcpy(sign2, get_sign(&token));
			if(sign2[0] == '\0') {
				fprintf(out, ";\n");
				continue;
			}

			match(token.type, in, debug_flag);
			strcpy(buffer3, var_val(&token));
			if(buffer3[0] != '\0') {
				match(token.type, in, debug_flag);
				fprintf(out, " %s %s;\n", sign2, buffer3);
				continue;
			}
		}

		else if(token.type == TOKEN_IF) {
			match(TOKEN_IF, in, debug_flag);
			match(TOKEN_COLON, in, debug_flag);

			strcpy(buffer, var_val(&token));
			if(buffer[0] == '\0') 
				goto SyntaxError;
			
			match(token.type, in, debug_flag);
			if(token.type == TOKEN_COLON) {
				match(TOKEN_COLON, in, debug_flag);
				fprintf(out, "if(%s)\n", buffer);
				continue;
			}

			strcpy(sign, get_cmp(&token));
			if(sign[0] == '\0')
				goto SyntaxError;
			
			match(token.type, in, debug_flag);
			strcpy(buffer2, var_val(&token));
			if(buffer2[0] == '\0')
				goto SyntaxError;
			
			match(token.type, in, debug_flag);
			match(TOKEN_COLON, in, debug_flag);
			fprintf(out, "if(%s %s %s)\n", buffer, sign, buffer2);
		}

		else if(token.type == TOKEN_WORKIF) {
			match(TOKEN_WORKIF, in, debug_flag);
			match(TOKEN_COLON, in, debug_flag);
		
			strcpy(buffer, var_val(&token));
			if(buffer[0] == '\0')
				goto SyntaxError;
			
			match(token.type, in, debug_flag);
			if(token.type == TOKEN_COLON) {
				match(TOKEN_COLON, in, debug_flag);
				fprintf(out, "while(%s)\n", buffer);
				continue;
			}
			
			strcpy(sign, get_cmp(&token));
			if(sign[0] == '\0')
				goto SyntaxError;

			match(token.type, in, debug_flag);
			strcpy(buffer2, var_val(&token));
			if(buffer2[0] == '\0')
				goto SyntaxError;
			
			match(token.type, in, debug_flag);
			match(TOKEN_COLON, in, debug_flag);
			fprintf(out, "while(%s %s %s)\n", buffer, sign, buffer2);
		}

		else if(token.type == TOKEN_OPAR) {
			match(TOKEN_OPAR, in, debug_flag);
			fprintf(out, "{\n");
		}
		else if(token.type == TOKEN_CPAR) {
			match(TOKEN_CPAR, in, debug_flag);
			fprintf(out, "}\n");
		}
	
		else if(token.type == TOKEN_C) {
			ccount++;	
			match(TOKEN_C, in, debug_flag);
			match(TOKEN_OPAR, in, debug_flag);
			
			int linec = 0;
			while(token.type != TOKEN_CPAR) {
				linec++;	
				if(linec > 5) {
					printf("Oops! I don't know C syntax.\n");
					exit(1);
				}
				strcpy(buffer, token.text);
				match(TOKEN_IGNORE, in, debug_flag);
				fprintf(out, "%s\n", buffer);
			}
			match(TOKEN_CPAR, in, debug_flag);
		}

		else if(token.type == TOKEN_RETURN) {
			match(TOKEN_RETURN, in, debug_flag);
			strcpy(buffer, var_val(&token));
			if(buffer[0] == '\0')
				goto SyntaxError;

			match(token.type, in, debug_flag);
			match(TOKEN_COLON, in, debug_flag);	
			if(nostd_flag) {
				fprintf(out,
					"__asm__ __volatile__(\n"
					"	\"movl $60, %%%%eax\\n\"\n"
					"	\"movl %%[ret], %%%%edi\\n\"\n"
					"	\"syscall\\n\"\n"
					"	:\n"
					"	: [ret] \"r\" ((int)%s)\n"
					"	: \"eax\", \"edi\", \"rcx\", \"r11\" \n"
					");\n",
					buffer
				);
				continue;
			}
			fprintf(out, "return %s;\n", buffer);
		}

		else if(token.type == TOKEN_TALK) {
			match(TOKEN_TALK, in, debug_flag);
			match(TOKEN_OPAR, in, debug_flag);
			strcpy(buffer, token.text);
			match(TOKEN_STR, in, debug_flag);
			match(TOKEN_CPAR, in, debug_flag);

			fprintf(out, "talk(\"%s\");\n", buffer);
		}		

		else {
			goto SyntaxError;
		}
	}
	return;

	SyntaxError:
		printf("\033[31mSyntax Error!!!\033[0m WTF??? %d %s\n", token.type, token.text);
		exit(1);
}
