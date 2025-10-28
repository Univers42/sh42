#include "lexer.h"


t_token_type check_key_word(int start, int length, const char *rest, t_token_type type, t_scanner *scan);

t_token handle_pipe(t_scanner *scan)
{
	advance(scan);
	if (match(scan, '|'))
		return (make_token(scan, TOKEN_PIPE_PIPE));
	else if (match(scan, '='))
		return (make_token(scan, TOKEN_ASS_OR));
	else
		return (make_token(scan, TOKEN_PIPE));
}

t_token handle_ampersand(t_scanner *scan)
{
	advance(scan);
	if (match(scan, '&'))
		return (make_token(scan, TOKEN_AMP_AMP));
	else
		return (make_token(scan, TOKEN_AMPERSAND));
}

t_token handle_semicolon(t_scanner *scan)
{
	advance(scan);
	if (match(scan, ';'))
		return (make_token(scan, TOKEN_SEMICOLON));
	else if (match(scan, '\n'))
		return (make_token(scan, TOKEN_NEWLINE));
	/* Single semicolon -> return semicolon token */
	return (make_token(scan, TOKEN_SEMICOLON));
}

t_token	handle_glob(t_scanner *scan)
{
	char terminator;

	terminator = peek(scan);
	advance(scan);
	if (terminator == '?')
		return (make_token(scan, TOKEN_QUESTION));
	if (terminator == '[')
		return (make_token(scan, TOKEN_GLOB_BRACKET_OPEN));
	else if (terminator == ']')
		return (make_token(scan, TOKEN_GLOB_BRACKET_CLOSE));
	return (error_token(scan, "Error glob token"));
}

t_token	handle_minus(t_scanner *scan)
{
	/* consume initial '-' */
	advance(scan);

	/* Double-dash long flag: consume following alnum or '-' */
	if (match(scan, '-'))
	{
		while (ft_isalnum(peek(scan)) || peek(scan) == '-')
			advance(scan);
		return (make_token(scan, TOKEN_FLAG));
	}

	/* Single-dash flag: consume all following alnum/hyphen characters */
	if (ft_isalnum(peek(scan)))
	{
		while (ft_isalnum(peek(scan)) || peek(scan) == '-')
			advance(scan);
		return (make_token(scan, TOKEN_FLAG));
	}

	/* Fallback: minus operator */
	return (make_token(scan, TOKEN_OP_MINUS));
}

t_token	handle_greater(t_scanner *scan)
{
	advance(scan);
	if (match(scan, '('))
		return (make_token(scan, TOKEN_SUBST_OUT));
	else if (match(scan, '>'))
		return (make_token(scan, TOKEN_OP_SHIFT_RIGHT));
	else if (match(scan, '&'))
		return (make_token(scan, TOKEN_REDIR_FD_IN));
	else if (match(scan, '|'))
		return (make_token(scan, TOKEN_REDIR_CLOBBER));
	else if (match(scan, '='))
		return (make_token(scan, TOKEN_COMP_GTEQ));
	else if (check_key_word(1, 2, ">=", TOKEN_ASS_SHIFT, scan))
		return (make_token(scan, TOKEN_ASS_SHIFT));
	else
		return (make_token(scan, TOKEN_REDIR_OUT));
}

t_token	handle_less(t_scanner *scan)
{
	advance(scan);
	if (match(scan, '('))
		return (make_token(scan, TOKEN_SUBST_IN));
	else if (match(scan, '<'))
		return (make_token(scan, TOKEN_REDIR_IN));
	else if (match(scan, '&'))
		return (make_token(scan, TOKEN_REDIR_FD_IN));
	else if (match(scan, '>'))
		return (make_token(scan, TOKEN_REDIR_INOUT));
	else if (match(scan, '='))
		return (make_token(scan, TOKEN_COMP_GTEQ));
	else if (check_key_word(1, 2, "<-", TOKEN_REDIR_HEREDOC_STRIP, scan))
		return (make_token(scan, TOKEN_ASS_SHIFT));
	else if (check_key_word(1, 2, "<=", TOKEN_ASS_UNSHIFT, scan))
		return (make_token(scan, TOKEN_ASS_UNSHIFT));
	else
		return (make_token(scan, TOKEN_REDIR_IN));
}

t_token	handle_lparen(t_scanner *scan)
{
	advance(scan);
	return (make_token(scan, TOKEN_LEFT_PAREN));
}

t_token handle_rparen(t_scanner *scan)
{
	advance(scan);
	return (make_token(scan, TOKEN_RIGHT_PAREN));
}

t_token handle_lbrace(t_scanner *scan)
{
	advance(scan);
	return (make_token(scan, TOKEN_LEFT_BRACE));
}

t_token handle_rbrace(t_scanner *scan)
{
	advance(scan);
	return (make_token(scan, TOKEN_RIGHT_BRACE));
}

t_token handle_newline(t_scanner *scan)
{
	advance(scan);
	return (make_token(scan, TOKEN_NEWLINE));
}

//<<< Category strings
//----------------------------------------------
t_token handle_squote(t_scanner *scan)
{
	return (string(scan));
}

t_token handle_dquote(t_scanner *scan)
{
	return (string(scan));
}

t_token	handle_bquote(t_scanner *scan)
{
	return (string(scan));
}
//-----------------------------------------------
t_token	handle_sys_op(t_scanner *scan)
{
	return (make_token(scan, TOKEN_EOF));
}

t_token handle_dollar(t_scanner *scan)
{
	advance(scan);
	if (match(scan, '*'))
		return (make_token(scan, TOKEN_DOLLAR_STAR));
	if (match(scan, '#'))
		return (make_token(scan, TOKEN_DOLLAR_HASH));
	else if (match(scan, '@'))
		return (make_token(scan, TOKEN_DOLLAR_AT));
	else if (match(scan, '?'))
		return (make_token(scan, TOKEN_DOLLAR_QUESTION));
	else if (match(scan, '$'))
		return (make_token(scan, TOKEN_DOLLAR_DOLLAR));
	else if (match(scan, '!'))
		return (make_token(scan, TOKEN_DOLLAR_BANG));
	else if (match(scan, '-'))
		return (make_token(scan, TOKEN_DOLLAR_DASH));
	else if (match(scan, '0'))
		return (make_token(scan, TOKEN_DOLLAR_ZERO));
	else if (match(scan, '{'))
		return (make_token(scan, TOKEN_PAR_EXP_START));
	else if (match(scan, '('))
		return (make_token(scan, TOKEN_CMD_SUBST_START));
	else if (check_key_word(1, 3, "((", TOKEN_ARITH_EXP_START, scan))
		return (make_token(scan, TOKEN_ARITH_EXP_START));
	else
		return (make_token(scan, TOKEN_DOLLAR));
}

t_token	handle_backslash(t_scanner *scan)
{
	advance(scan);
	return (make_token(scan, TOKEN_BACKSLASH));
}

/**
 * here different possible intepretation
 * TOKEN_HASH_OP
 * TOKEN_POUND
 * TOKEN_POUND_POUND
 * TOKEN_HASH
 */
t_token	handle_hash(t_scanner *scan)
{
	advance(scan);
	if (match(scan, '#'))
		return (make_token(scan, TOKEN_POUND_POUND));
	return (make_token(scan, TOKEN_POUND));
}

t_token	handle_tilde(t_scanner *scan)
{
	advance(scan);
	return (make_token(scan, TOKEN_TILDE));
}

t_token	handle_star(t_scanner *scan)
{
	advance(scan);
	if (match(scan, '*'))
		return (make_token(scan, TOKEN_GLOB_STAR));
	else if (match(scan, '='))
		return (make_token(scan, TOKEN_ASS_MULT));
	return (make_token(scan, TOKEN_GLOB_STAR));
}

t_token	handle_plus(t_scanner *scan)
{
	advance(scan);
	if (match(scan, '+'))
		return (make_token(scan, TOKEN_OP_INCREMENT));
	else if (match(scan, '='))
		return (make_token(scan, TOKEN_ASS_PLUS));
	return (make_token(scan, TOKEN_OP_ADD));
}

t_token	handle_div(t_scanner *scan)
{
	advance(scan);
	if (match(scan, '='))
		return (make_token(scan, TOKEN_ASS_DIV));
	return (make_token(scan, TOKEN_OP_DIV));
}

t_token	handle_percent(t_scanner *scan)
{
	advance(scan);
	if (match(scan, '%'))
		return (make_token(scan, TOKEN_PERCENT_PERCENT));
	return (make_token(scan, TOKEN_PERCENT));
}

t_token	handle_eq(t_scanner *scan)
{
	advance(scan);
	/* Handle '==' as comparison, '='+assignment, '+=' handled separately by handlers on '+' */
	if (match(scan, '='))
		return (make_token(scan, TOKEN_COMP_EQ));
	else if (match(scan, '+'))
		return (make_token(scan, TOKEN_ASS_PLUS));
	/* Single '=' assignment */
	return (make_token(scan, TOKEN_ASS_EQUAL));
}

t_token handle_question(t_scanner *scan)
{
	advance(scan);
	if (match(scan, '-'))
		return (make_token(scan, TOKEN_COLON_DASH));
	else if (match(scan, '='))
		return (make_token(scan, TOKEN_COLON_EQUAL));
	else if (match(scan, '+'))
		return (make_token(scan, TOKEN_COLON_QUESTION));
	return (make_token(scan, TOKEN_ERR));
}

t_token	handle_bang(t_scanner *scan)
{
	advance(scan);
	if (match(scan, '!'))
		return (make_token(scan, TOKEN_BANG_BANG));
	return (make_token(scan, TOKEN_BANG));
}

t_token handle_lbracket(t_scanner *scan)
{
	advance(scan);
	return (make_token(scan, TOKEN_GLOB_BRACKET_OPEN));
}

t_token handle_rbracket(t_scanner *scan)
{
	advance(scan);
	return (make_token(scan, TOKEN_GLOB_BRACKET_CLOSE));
}

/* dot handler */
t_token handle_dot(t_scanner *scan)
{
	/* If dot starts a fractional number like .5, consume dot and digits */
	advance(scan);
	if (ft_isdigit(peek(scan)))
	{
		/* fractional number */
		while (ft_isdigit(peek(scan)))
			advance(scan);
		return (make_token(scan, TOKEN_NUMBER));
	}
	/* Otherwise, dot may be part of a word (handled by identifier when appropriate),
	   or a standalone DOT token */
	return (make_token(scan, TOKEN_DOT));
}

t_token handle_word(t_scanner *scan)
{
	return (identifier(scan));
}

t_token	handle_whitespace(t_scanner *scan)
{
	while (ft_isspace(peek(scan)))
		advance(scan);
	return (make_token(scan, TOKEN_WHITESPACE));
}

t_token handle_eof(t_scanner *scan)
{
	return (make_token(scan, TOKEN_EOF));
}

t_token handle_literal(t_scanner *scan)
{
	char c;
	c = peek(scan);
	/* If it looks like an identifier, scan it */
	if (ft_isalpha(c) || c == '_')
		return (identifier(scan));
	/* If it looks like a number, scan it */
	if (ft_isdigit(c))
		return (number(scan));
	/* Otherwise consume a single character as a word/literal to avoid zero-length tokens */
	advance(scan);
	return (make_token(scan, TOKEN_WORD));
}

