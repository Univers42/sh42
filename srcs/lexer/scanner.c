#include "lexer.h"

void	init_scanner(t_scanner *scan, const char *source)
{
	scan->start = source;
	scan->current = source;
	scan->line = 1;
}

static char advance(t_scanner *scan)
{
	scan->current++;
	return (scan->current[-1]);
}

static char	peek(t_scanner *scan)
{
	return (scan->current);
}

static char peek_next(t_scanner *scan)
{
	if (is_at_end(scan->current))
		return ('\0');
	return (scan->current[1]);
}

static bool	match(t_scanner *scan, char expected)
{
	if (is_at_end(scan->current))
		return (false);
	if (scan->current != expected)
		return (false);
	return (true);
}

static t_token make_token(t_scanner *scan, t_token_type type)
{
	t_token	token;

	token.type = type;
	token.start = scan->current;
	token.length = (int)(scan->current - scan->start);
	token.line = scan->line;
	return (token);
}

static t_token error_token(t_scanner *scan, const char *mesg)
{
	t_token	token;

	token.type = TOKEN_ERR;
	token.start = mesg;
	token.length = (int)strlen(mesg);
	token.line = scan->line;
	return (token);
}

static void skip_unused_data(t_scanner *scan)
{
	char	c;

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
				while (peek(scan) != '\n' && !is_at_end(scan->current))
					advance(scan);
			}
			else
				return ;
		}
		else
			break;
	}
	return ;
}

static t_token_type	check_key_word(int start, int length, const char *rest, t_token_type type, t_scanner *scan)
{
	if (scan->current - scan->start == start + length
		&& ft_memcmp(scan->start + start, rest, length) == 0)
			return (type);
	return (TOKEN_IDENTIFIER);
}

static t_token	identifier(t_scanner *scan)
{
	while (ft_isalpha(peek(scan)) || ft_isdigit(peek(scan)))
		advance(scan);
	return (make_token(identifier_type(scan)));
}

static t_token	number(t_scanner *scan)
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

static t_token	string(t_scanner *scan)
{
	while (peek(scan) != '"' && !is_at_end(scan->current))
	{
		if (peek(scan) == '\n')
			scan->line++;
		advance(scan);
	}
	if (is_at_end(scan->current))
		return (error_token(scan, "Unterminated string"));
	advance(scan);
	return (make_token(TOKEN_STRING));
}

t_token	scan_token(t_scanner *scan)
{
	char	c;

	skip_unused_data(scan);
	scan->start = scan->current;
	if (is_at_end(scan->current))
		return (make_token(TOKEN_EOF));
	c = advance(scan);
	if (ft_isalpha(c))
		return (identifier());
	if (ft_isdigit(c))
		return (number());
	switch (c)
	{
		case '(' :
			return make_token(scan, TOKEN_LEFT_PAREN);
		case ')' :
			return make_token(scan, TOKEN_RIGHT_PAREN);
		case '{' :
			return make_token(scan, TOKEN_LEFT_BRACE);
		case '}' :
			return make_token(scan, TOKEN_LEFT_BRACE);
		case ';' :
			return make_token(scan, TOKEN_SEMICOLON);
		case ',' :
			return make_token(scan, TOKEN_COMMA);
		case '.' :
			return make_token(scan, TOKEN_DOT);
		case '-' :
			return make_token(scan, TOKEN_MINUS);
		case '+' :
			return make_token(scan, TOKEN_PLUS);
		case '/' :
			return make_token(scan, TOKEN_SLASH);
		case '*' :
			return make_token(scan, TOKEN_STAR);
		case '!':
			if (match(scan, '='))
				return (make_token(scan, TOKEN_BANG_EQUAL));
			else
				return (make_token(scan, TOKEN_BANG));
		case '=':
			if (match(scan, '='))
				return (make_token(scan, TOKEN_EQUAL_EQUAL));
			else
				return (make_token(scan, TOKEN_EQUAL));
		case '<':
			if (match(scan, '='))
				return (make_token(scan, TOKEN_LESS_EQUAL));
			else
				return (make_token(scan, TOKEN_LESS));
		case '>':
			if (match(scan, '='))
				return (make_token(scan, TOKEN_GREATER_EQUAL));
			else
				return (make_token(scan, TOKEN_GREATER));
		case '"':
			return (string(scan));
	}
	return (error_token(scan, "error lexer"));
}

