#include "lexer.h"
#include <stdio.h>
#include <string.h> // for strlen, memcmp if needed

/**
 * as we use  t_scanner *scan in all variables, maybe it makes more sense to create
 * a singleton pattern to avoid generating too much confusion..
 */

static t_scanner global_scanner;

t_scanner *init_scanner(const char *source)
{
	global_scanner.start = source;
	global_scanner.current = source;
	global_scanner.line = 1;
	return (&global_scanner);
}

static char advance(t_scanner *scan)
{
	scan->current++;
	return (scan->current[-1]);
}

static char peek(t_scanner *scan)
{
	return *(scan->current);
}

static bool scan_is_at_end(t_scanner *scan)
{
	return (*scan->current == '\0');
}

static char peek_next(t_scanner *scan)
{
	if (scan_is_at_end(scan))
		return ('\0');
	return (scan->current[1]);
}

static bool match(t_scanner *scan, char expected)
{
	if (scan_is_at_end(scan))
		return (false);
	if (*(scan->current) != expected)
		return (false);
	scan->current++;
	return (true);
}

static t_token make_token(t_scanner *scan, t_token_type type)
{
	t_token token;

	token.type = type;
	token.start = scan->start;
	token.length = (int)(scan->current - scan->start);
	token.line = scan->line;
	return (token);
}

static t_token error_token(t_scanner *scan, const char *mesg)
{
	t_token token;

	token.type = TOKEN_ERR;
	token.start = mesg;
	token.length = (int)strlen(mesg);
	token.line = scan->line;
	return (token);
}

static void skip_unused_data(t_scanner *scan)
{
	char c;

	while (ST_SCANNING)
	{
		c = peek(scan);
		if (ft_isspace(c))
			advance(scan);
		if (assert_char(c, '\n'))
		{
			scan->line++;
			advance(scan);
		}
		if (assert_char(c, '/'))
		{
			if (peek_next(scan) == '/')
			{
				while (peek(scan) != '\n' && !scan_is_at_end(scan))
					advance(scan);
			}
			else
				return;
		}
		else
			break;
	}
	return;
}

t_token_type check_key_word(int start, int length, const char *rest, t_token_type type, t_scanner *scan)
{
	if (scan->current - scan->start == start + length && ft_memcmp(scan->start + start, rest, length) == 0)
		return (type);
	return (TOKEN_IDENTIFIER);
}

t_token_type identifier_type(t_scanner *scan)
{
	int length = (int)(scan->current - scan->start);
	const t_keyword_entry *kw = map_keywords();

	for (int i = 0; kw[i].keyword != NULL; i++)
	{
		if (kw[i].length == length &&
			ft_memcmp(scan->start, kw[i].keyword, length) == 0)
			return kw[i].type;
	}
	return TOKEN_IDENTIFIER;
}

static t_token identifier(t_scanner *scan)
{
	// Only consume [a-zA-Z0-9_], not punctuation
	while (ft_isalpha(peek(scan)) || ft_isdigit(peek(scan)) || peek(scan) == '_')
		advance(scan);
	return (make_token(scan, identifier_type(scan)));
}

static t_token number(t_scanner *scan)
{
	while (ft_isdigit(peek(scan)))
		advance(scan);
	if (peek(scan) == '.' && ft_isdigit(peek_next(scan)))
	{
		advance(scan);
		while (ft_isdigit(peek(scan)))
			advance(scan);
	}
	return (make_token(scan, TOKEN_NUMBER));
}

static t_token string(t_scanner *scan)
{
	while (peek(scan) != '"' && !scan_is_at_end(scan))
	{
		if (peek(scan) == '\n')
			scan->line++;
		advance(scan);
	}
	if (scan_is_at_end(scan))
		return (error_token(scan, "Unterminated string"));
	advance(scan);
	return (make_token(scan, TOKEN_STRING));
}

t_token scan_token(t_scanner *scan)
{
	char c;

	skip_unused_data(scan);

	// Set start to current before scanning a new token
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

void scan_all_tokens(t_scanner *scan, int debug)
{
	t_token token;
	do
	{
		token = scan_token(scan);
		if (debug)
			print_token(&token);
	} while (token.type != TOKEN_EOF && token.type != TOKEN_ERR);
}

void print_token(const t_token *token)
{
	static const char *token_names[] = {
		"LEFT_PAREN", "RIGHT_PAREN", "LEFT_BRACE", "RIGHT_BRACE", "COMMA", "DOT",
		"MINUS", "PLUS", "SEMICOLON", "SLASH", "STAR", "PIPE", "AMPERSAND",
		"BANG", "BANG_EQUAL", "EQUAL", "EQUAL_EQUAL", "GREATER", "GREATER_EQUAL",
		"LESS", "LESS_EQUAL", "IDENTIFIER", "STRING", "NUMBER", "AND", "CLASS",
		"ELSE", "FALSE", "FOR", "FUN", "IF", "NIL", "OR", "PRINT", "RETURN",
		"SUPER", "THIS", "TRUE", "VAR", "WHILE", "ERR", "EOF"};
	int type = token->type;
	if (type < 0 || type >= TOKEN_COUNT)
		type = TOKEN_ERR;
	printf("[token] %-16s | line %d | '%.*s'\n",
		   token_names[type], token->line, token->length, token->start);
}
