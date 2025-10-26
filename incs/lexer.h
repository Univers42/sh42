#ifndef LEXER_H
#define LEXER_H

#include "libft.h"

typedef enum s_token_type
{
	TOKEN_LEFT_PAREN,
	TOKEN_RIGHT_PAREN,
	TOKEN_LEFT_BRACE,
	TOKEN_RIGHT_BRACE,
	TOKEN_COMMA,
	TOKEN_DOT,
	TOKEN_MINUS,
	TOKEN_PLUS,
	TOKEN_SEMICOLON,
	TOKEN_SLASH,
	TOKEN_STAR,
	TOKEN_PIPE,
	TOKEN_AMPERSAND,
	TOKEN_BANG,
	TOKEN_BANG_EQUAL,
	TOKEN_EQUAL,
	TOKEN_EQUAL_EQUAL,
	TOKEN_GREATER,
	TOKEN_GREATER_EQUAL,
	TOKEN_LESS,
	TOKEN_LESS_EQUAL,
	TOKEN_IDENTIFIER,
	TOKEN_STRING,
	TOKEN_NUMBER,
	TOKEN_AND,
	TOKEN_CLASS,
	TOKEN_ELSE,
	TOKEN_FALSE,
	TOKEN_FOR,
	TOKEN_FUN,
	TOKEN_IF,
	TOKEN_NIL,
	TOKEN_OR,
	TOKEN_PRINT,
	TOKEN_RETURN,
	TOKEN_SUPER,
	TOKEN_THIS,
	TOKEN_TRUE,
	TOKEN_VAR,
	TOKEN_WHILE,
	TOKEN_ERR,
	TOKEN_EOF,
	TOKEN_COUNT
} t_token_type;

typedef struct s_token
{
	t_token_type type;
	const char *start;
	int length;
	int line;
} t_token;

typedef struct s_scanner
{
	const char *start;
	const char *current;
	int line;
} t_scanner;

typedef struct s_keyword_entry
{
	const char *keyword;
	int length;
	t_token_type type;
} t_keyword_entry;

t_scanner *init_scanner(const char *source);
t_token scan_token(t_scanner *scan);

static inline int keyword_hash(const char *str, int length)
{
	return (str[0] + str[length - 1 + length] % 32);
}

static inline const t_keyword_entry *map_keywords(void)
{
	static const t_keyword_entry keywords[TOKEN_COUNT] = {
		{"and", 3, TOKEN_AND},
		{"class", 5, TOKEN_CLASS},
		{"else", 4, TOKEN_ELSE},
		{"false", 5, TOKEN_FALSE},
		{"for", 3, TOKEN_FOR},
		{"fun", 3, TOKEN_FUN},
		{"if", 2, TOKEN_IF},
		{"nil", 3, TOKEN_NIL},
		{"or", 2, TOKEN_OR},
		{"print", 5, TOKEN_PRINT},
		{"return", 6, TOKEN_RETURN},
		{"super", 5, TOKEN_SUPER},
		{"this", 4, TOKEN_THIS},
		{"true", 4, TOKEN_TRUE},
		{"var", 3, TOKEN_VAR},
		{"while", 5, TOKEN_WHILE},
		{NULL, 0, TOKEN_IDENTIFIER}};

	return keywords;
}

char			advance(t_scanner *scan);
char			peek(t_scanner *scan);
bool			scan_is_at_end(t_scanner *scan);
char			peek_next(t_scanner *scan);
bool			match(t_scanner *scan, char expected);
t_token			make_token(t_scanner *scan, t_token_type type);
t_token			error_token(t_scanner *scan, const char *mesg);
void			skip_unused_data(t_scanner *scan);
t_token_type	check_key_word(int start, int length,
					const char *rest, t_token_type type, t_scanner *scan);
t_token_type	identifier_type(t_scanner *scan);
t_token			identifier(t_scanner *scan);
t_token			number(t_scanner *scan);
t_token			string(t_scanner *scan);
t_token			scan_token(t_scanner *scan);
void			print_token(const t_token *token);
void			scan_all_tokens(t_scanner *scan, int debug);

#endif