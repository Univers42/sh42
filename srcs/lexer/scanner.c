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
			return make_token(scan, TOKEN_LESS_EQUAL);
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

void scan_all_tokens(t_scanner *scan, int debug)
{
	t_token token;

	token = (t_token){0};
	while (token.type != TOKEN_EOF && token.type != TOKEN_ERR)
	{
		token = scan_token(scan);
		if (debug)
			print_token(&token);
	}
}

void print_token(const t_token *token)
{
	static const char *token_names[] = {
		"PIPE", "PIPE_PIPE", "AMPERSAND", "AMP_AMP", "SEMICOLON", "NEWLINE",
		"REDIR_IN", "REDIR_OUT", "REDIR_APPEND", "REDIR_HEREDOC", "REDIR_HEREDOC_STRIP",
		"REDIR_FD_IN", "REDIR_FD_OUT", "REDIR_INOUT", "REDIR_CLOBBER",
		"LEFT_PAREN", "RIGHT_PAREN", "LEFT_BRACE", "RIGHT_BRACE", "GREATER",
		"IF", "THEN", "ELIF", "FI", "ELSE", "CASE", "ESAC", "FOR", "WHILE",
		"UNTIL", "CLASS", "DO", "DONE", "IN", "FUNCTION", "SELECT",
		"SQUOTE_START", "SQUOTE", "SQUOTE_END", "DQUOTE_START", "DQUOTE", "DQUOTE_END",
		"BQUOTE_START", "BQUOTE", "BQUOTE_END", "DOLLAR", "DOLLAR_LPAREN", "DOLLAR_LBRACE",
		"DOLLAR_DPAREN", "_TILDE", "GLOB_STAR", "GLOB_QUESTION", "GLOB_BRACKET_OPEN",
		"GLOB_BRACKET_COSE", "BACKSLASH", "HASH", "BANG", "AT", "QUESTION", "DASH",
		"DOLLAR_HASH", "DOLLAR_STAR", "DOLLAR_AT", "DOLLAR_QUESTION", "DOLLAR_DOLLAR",
		"DOLLAR_BANG", "DOLLAR_DASH", "DOLLAR_ZERO", "GREATER_GREATER", "COMMA", "DOT",
		"MINUS", "PLUS", "SLASH", "STAR", "BANG_EQUAL", "EQUAL", "EQUAL_EQUAL",
		"GREATER_AMPERSAND", "GREATER_EQUAL", "LESS", "LESS_LESS", "LESS_AMPERSAND",
		"LESS_EQUAL", "IDENTIFIER", "STRING", "AND", "WORD", "NUMBER", "FALSE",
		"FUN", "NIL", "OR", "PRINT", "RETURN", "SUPER", "THIS", "TRUE", "VAR",
		"WHITESPACE", "EOF", "ERR"};
	int type = token->type;
	const char *name = "UNKNOWN";
	if (type >= 0 && type < (int)(sizeof(token_names) / sizeof(token_names[0])))
		name = token_names[type];
	printf("[token] %-16s | line %d | '%.*s'\n",
		   name, token->line, token->length, token->start);
}
