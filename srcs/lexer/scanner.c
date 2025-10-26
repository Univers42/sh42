#include "lexer.h"
#include <stdio.h>
#include <string.h> // for strlen, memcmp if needed

t_token scan_token(t_scanner *scan)
{
	char c;

	skip_unused_data(scan);
	scan->start = scan->current;
	if (scan_is_at_end(scan))
		return make_token(scan, TOKEN_EOF);
	c = advance(scan);
	if (ft_isalpha(c) || c == '_')
		return identifier(scan);
	if (ft_isdigit(c))
		return number(scan);
	switch (c)
	{
	case '(':
		return make_token(scan, TOKEN_LEFT_PAREN);
	case ')':
		return make_token(scan, TOKEN_RIGHT_PAREN);
	case '{':
		return make_token(scan, TOKEN_LEFT_BRACE);
	case '}':
		return make_token(scan, TOKEN_RIGHT_BRACE);
	case ';':
		return make_token(scan, TOKEN_SEMICOLON);
	case ',':
		return make_token(scan, TOKEN_COMMA);
	case '.':
		return make_token(scan, TOKEN_DOT);
	case '-':
		return make_token(scan, TOKEN_MINUS);
	case '+':
		return make_token(scan, TOKEN_PLUS);
	case '/':
		return make_token(scan, TOKEN_SLASH);
	case '*':
		return make_token(scan, TOKEN_STAR);
	case '!':
		if (match(scan, '='))
			return make_token(scan, TOKEN_BANG_EQUAL);
		else
			return make_token(scan, TOKEN_BANG);
	case '=':
		if (match(scan, '='))
			return make_token(scan, TOKEN_EQUAL_EQUAL);
		else
			return make_token(scan, TOKEN_EQUAL);
	case '<':
		if (match(scan, '='))
			return make_token(scan, TOKEN_LESS_EQUAL);
		else
			return make_token(scan, TOKEN_LESS);
	case '>':
		if (match(scan, '='))
			return make_token(scan, TOKEN_GREATER_EQUAL);
		else
			return make_token(scan, TOKEN_GREATER);
	case '"':
		return string(scan);
	}
	return error_token(scan, "error lexer");
}

void	scan_all_tokens(t_scanner *scan, int debug)
{
	t_token	token;

	token = (t_token){0};
	while (token.type != TOKEN_EOF && token.type != TOKEN_ERR)
	{
		token = scan_token(scan);
		if (debug)
			print_token(&token);
	}
}

void	print_token(const t_token *token)
{
	static const char *token_names[] = {
		"LEFT_PAREN", "RIGHT_PAREN", "LEFT_BRACE", "RIGHT_BRACE", "COMMA", "DOT",
		"MINUS", "PLUS", "SEMICOLON", "SLASH", "STAR", "PIPE", "AMPERSAND",
		"BANG", "BANG_EQUAL", "EQUAL", "EQUAL_EQUAL", "GREATER", "GREATER_EQUAL",
		"LESS", "LESS_EQUAL", "IDENTIFIER", "STRING", "NUMBER", "AND", "CLASS",
		"ELSE", "FALSE", "FOR", "FUN", "IF", "NIL", "OR", "PRINT", "RETURN",
		"SUPER", "THIS", "TRUE", "VAR", "WHILE", "ERR", "EOF"};
	int					type;

	type = token->type;
	if (type < 0 || type >= TOKEN_COUNT)
		type = TOKEN_ERR;
	printf("[token] %-16s | line %d | '%.*s'\n",
			token_names[type], token->line, token->length, token->start);
}
