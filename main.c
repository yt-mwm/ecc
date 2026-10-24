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
#include <string.h>
#include <stdlib.h>
#include "parser.h"

int
set_flag(int i, char *argv[], const char *flag)
{
	if(argv[i] == NULL) 
		return 0;
	return (strcmp(argv[i], flag) == 0);
}

typedef struct list{
	char str[255];
	struct list *next;
} list;

list*
add_node(const char *str)
{
	list *node = (list *)malloc(sizeof(list));
	if(node == NULL) {
		exit(1);
	}
	strcpy(node->str, str);
	return node;
}

list*
create_list(list* head, int *i, int argc, char *argv[])
{	
	while((*i) < argc && !set_flag(*i, argv, "_")) {
		list *node = add_node(argv[*i]);
		node->next = head;
		head = node;
		(*i)++;
	}
	return head;
}

void
freeh(list* head)
{
	while(head != NULL) {
		list* next = head->next;
		free(head);
		head = next;
	}
}

int
main(int argc, char *argv[])
{
	if(argc < 3) 
		goto incorrect_args;

	int debug_flag = 0;
	int trash_flag = 0;
	int nostd_flag = 0;
	int notalk_flag = 0;
	char compiler[255];
	strcpy(compiler, "gcc");
	list *ihead = NULL;
	list *whead = NULL;

	int i = 3;
	for(; i < argc ;) { 
		if(set_flag(i, argv, "-clang")) {
			strcpy(compiler, "clang");
			i++;
		}
		else if(set_flag(i, argv, "-tcc")) {
			strcpy(compiler, "tcc");
			i++;
		}
		else if(set_flag(i, argv, "-debug")) {
			debug_flag = 1;
			i++;
		}
		else if(set_flag(i, argv, "-trash")) {
			trash_flag = 1;
			i++;
		}
		else if(set_flag(i, argv, "-includes=_")) {
			i++;
			ihead = create_list(ihead, &i, argc, argv);
			i++;
		}
		else if(set_flag(i, argv, "-write=_")) {
			i++;
			whead = create_list(whead, &i, argc, argv);
			i++;
		}
		else if(set_flag(i, argv, "-nostd")) {
			nostd_flag = 1;
			i++;
		}
		else if(set_flag(i, argv, "-notalk")) {
			notalk_flag = 1;
			i++;
		}
		else if(set_flag(i, argv, "-v")) {
			puts("version 0.1 ECC (Eblang Compiler to C) by mwm.");
			exit(0);
		}

		else {
			printf("your flag '%s' does not exist\n", argv[i]);
			goto incorrect_args;
		}
	}
	
	char *infile = argv[1];
	int len = strlen(infile);
	if (len < 4 || infile[len-3] != '.' || infile[len-2] != 'e' || infile[len-1] != 'b') {
		goto bad_file;
	}

	FILE *in = fopen(infile, "r");
	if(in == NULL) {
		printf("\033[31mCompilation Error!!!\033[0m Can't open your file \'%s\'! Maybe it's good if your code is shit\n", infile);
		return 1;
	}

	FILE *out = fopen("trash.c", "w");
	if(out == NULL) {
		printf("\033[31mCompilation Error!!!\033[0m Can't generate a C file!\n");
		return 1;
	}
	if(debug_flag)
		printf("parser statistics: Instrutions in \'%s\'\n", argv[1]);

	list *icurr = ihead;
	while(icurr != NULL) {
		fprintf(out, "#include %s\n", icurr->str);
		list *next = icurr->next;
		icurr = next;
	}
	list *wcurr = whead;
	while(wcurr != NULL) {
		fprintf(out, "%s ", wcurr->str);
		list *next = wcurr->next;
		wcurr = next;
	}

	if(!notalk_flag) {
		fprintf(out,
			"void talk(char *buffer) {\n"
			"	int i = 0;\n"
			"	while(buffer[i] != '\\0') {\n"
			"		__asm__ __volatile__ (\n"
			"			\"movq $1, %%%%rax\\n\"\n"
			"			\"movq $1, %%%%rdi\\n\"\n"
			"			\"movq %%[ch], %%%%rsi\\n\"\n"
			"			\"movq $1, %%%%rdx\\n\"\n"
			"			\"syscall\\n\"\n"
			"			:\n"
			"			: [ch] \"r\" (&buffer[i])\n"
			"			: \"rax\", \"rdi\", \"rdx\", \"rcx\", \"r11\" \n"
			"		);\n"
			"		i++;\n"	
			"	}\n"
			"}\n"
			);
	}
	
	fprintf(out, "int main() {\n");

	parse(in, out, debug_flag, nostd_flag);
	
	fprintf(out, "}\n");
	fclose(in);
	fclose(out);
	
	printf("Compilation...\t");
	char msg[512];

	if(nostd_flag) {
		if(!strcmp(compiler, "tcc")) 
			sprintf(msg, "%s -nostdlib trash.c -o %s -Wl,-e,main", compiler, argv[2]);
		else
			sprintf(msg, "%s -ffreestanding -nostdlib -s -Os trash.c -o %s -Wl,-e,main", compiler, argv[2]);
	}
	else
		sprintf(msg, "%s trash.c -o %s", compiler, argv[2]);

	int status = system(msg);
    if (status == 0) {
        printf("\033[32mSuccess!\n*\033[0m Compiled \'%s\' to \'%s\'\n", argv[1], argv[2]);
    } else {
        printf("\033[31mFailed!\n*\033[0m Maybe \'%s\' is not installed OR incorrect object file!\n", compiler);
    }

	if(!trash_flag)
		remove("trash.c");

	freeh(ihead);
	freeh(whead);
	return status;

	incorrect_args:
		printf("\033[31mCompilation Error!!!\033[0m Incorrect pass arguments!\n");
		return 1;

	bad_file:
		printf("\033[31mCompilation Error!!!\033[0m ECC supported only (.eb) files! For example <mycode.eb>. But your file \'%s\' is useless code\n", argv[1]);
		return 1;
}
