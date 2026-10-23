typedef enum {
						          //                              num
	TOKEN_COLON,        // ':'                          0
	TOKEN_STR,          // '"'                          1
	TOKEN_OPAR,         // '('                          2
	TOKEN_CPAR,         // ')'                          3
	TOKEN_OPEN,         // '{'                          4
	TOKEN_CLOSE,        // '}'                          5
	TOKEN_EQU,          // '='                          6
	TOKEN_PLUS,         // '+'                          7
	TOKEN_MINUS,        // '-'                          8
	TOKEN_MUL,          // '*'                          9 
	TOKEN_DIV,          // '/'                          10
	TOKEN_MOD,          // '%'                          11
	TOKEN_XOR,          // '^'                          12
	TOKEN_RSHT,         // ">>"                         13
	TOKEN_LSHT,         // "<<"                         14
	TOKEN_CMP,          // "=="                         15
	TOKEN_NCMP,         // "!="                         16
	TOKEN_GREAT,        // '>'                          17
	TOKEN_LESS,         // '<'                          18
	TOKEN_INT,          // int type                     19
	TOKEN_WORD,         // char type                    20
	TOKEN_LONG,         // long int type                21
	TOKEN_VAR,          // variable name                22
	TOKEN_VAL,          // value                        23
	TOKEN_IF,           // if operator                  24
	TOKEN_ELSE,         // else operator                25
	TOKEN_WORKIF,       // while loop                   26
	TOKEN_RETURN,       // return                       27
	TOKEN_EOF,          // end of file                  28
	TOKEN_ERR,          // incorrect token              29
	TOKEN_C,            // can write C in Eblang :)     30
	TOKEN_IGNORE,       // ignore a string in parser    31 

	TOKEN_TALK          // just print characters        32

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
