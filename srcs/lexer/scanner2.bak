/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   scanner2.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dlesieur <dlesieur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/26 19:00:07 by dlesieur          #+#    #+#             */
/*   Updated: 2025/10/27 00:11:49 by dlesieur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "lexer.h"
#include <ctype.h>
#include <string.h>
#include <stdlib.h>

/* ============================================================================
 * STRUCTURES (using your definitions)
 * ============================================================================ */

typedef struct s_token
{
	t_token_type	type;
	const char		*start;
	int				length;
	int				line;
}	t_token;

typedef struct s_scanner
{
	const char		*start;
	const char		*current;
	int				line;
}	t_scanner;

typedef struct s_keyword_entry
{
	const char		*keyword;
	int				length;
	t_token_type	type;
}	t_keyword_entry;

/* ============================================================================
 * FUNCTION POINTER TYPES
 * ============================================================================ */

typedef t_token (*t_char_handler)(t_scanner *scanner);

/* Forward declarations */
static t_token handle_pipe(t_scanner *scanner);
static t_token handle_ampersand(t_scanner *scanner);
static t_token handle_semicolon(t_scanner *scanner);
static t_token handle_greater(t_scanner *scanner);
static t_token handle_less(t_scanner *scanner);
static t_token handle_lparen(t_scanner *scanner);
static t_token handle_rparen(t_scanner *scanner);
static t_token handle_lbrace(t_scanner *scanner);
static t_token handle_rbrace(t_scanner *scanner);
static t_token handle_newline(t_scanner *scanner);
static t_token handle_squote(t_scanner *scanner);
static t_token handle_dquote(t_scanner *scanner);
static t_token handle_dollar(t_scanner *scanner);
static t_token handle_bquote(t_scanner *scanner);
static t_token handle_backslash(t_scanner *scanner);
static t_token handle_hash(t_scanner *scanner);
static t_token handle_tilde(t_scanner *scanner);
static t_token handle_star(t_scanner *scanner);
static t_token handle_question(t_scanner *scanner);
static t_token handle_lbracket(t_scanner *scanner);
static t_token handle_rbracket(t_scanner *scanner);
static t_token handle_word(t_scanner *scanner);
static t_token handle_whitespace(t_scanner *scanner);
static t_token handle_eof(t_scanner *scanner);
static t_token handle_error(t_scanner *scanner);

/* ============================================================================
 * JUMP TABLE: O(1) character-to-handler mapping
 * ============================================================================ */

static t_char_handler g_jump_table[256];
static int g_jump_table_initialized = 0;

/* Initialize the jump table (call once at startup) */
void scanner_init_jump_table(void)
{
	int i;
	
	if (g_jump_table_initialized)
		return;
	
	/* Default: all characters go to word handler */
	for (i = 0; i < 256; i++)
		g_jump_table[i] = handle_word;
	
	/* Register special character handlers */
	g_jump_table['|'] = handle_pipe;
	g_jump_table['&'] = handle_ampersand;
	g_jump_table[';'] = handle_semicolon;
	g_jump_table['>'] = handle_greater;
	g_jump_table['<'] = handle_less;
	g_jump_table['('] = handle_lparen;
	g_jump_table[')'] = handle_rparen;
	g_jump_table['{'] = handle_lbrace;
	g_jump_table['}'] = handle_rbrace;
	g_jump_table['\n'] = handle_newline;
	g_jump_table['\''] = handle_squote;
	g_jump_table['"'] = handle_dquote;
	g_jump_table['$'] = handle_dollar;
	g_jump_table['`'] = handle_bquote;
	g_jump_table['\\'] = handle_backslash;
	g_jump_table['#'] = handle_hash;
	g_jump_table['~'] = handle_tilde;
	g_jump_table['*'] = handle_star;
	g_jump_table['?'] = handle_question;
	g_jump_table['['] = handle_lbracket;
	g_jump_table[']'] = handle_rbracket;
	g_jump_table[' '] = handle_whitespace;
	g_jump_table['\t'] = handle_whitespace;
	g_jump_table['\0'] = handle_eof;
	
	g_jump_table_initialized = 1;
}

/* ============================================================================
 * PERFECT HASH: O(1) keyword lookup
 * ============================================================================ */

/* Keyword table (sorted by hash for better cache locality) */
static const t_keyword_entry g_keywords[] = {
	{"if", 2, TOKEN_IF},
	{"fi", 2, TOKEN_FI},
	{"in", 2, TOKEN_IN},
	{"do", 2, TOKEN_DO},
	{"then", 4, TOKEN_THEN},
	{"else", 4, TOKEN_ELSE},
	{"elif", 4, TOKEN_ELIF},
	{"case", 4, TOKEN_CASE},
	{"esac", 4, TOKEN_ESAC},
	{"done", 4, TOKEN_DONE},
	{"for", 3, TOKEN_FOR},
	{"while", 5, TOKEN_WHILE},
	{"until", 5, TOKEN_UNTIL},
	{"function", 8, TOKEN_FUNCTION},
	{"select", 6, TOKEN_SELECT},
	{NULL, 0, TOKEN_WORD} /* Sentinel */
};

/* Perfect hash function for shell keywords */
static inline unsigned int keyword_hash(const char *str, int len)
{
	if (len == 0 || len > 8)
		return 0;
	/* Simple but effective: first_char + length + last_char */
	return ((unsigned char)str[0] + len + (unsigned char)str[len - 1]) % 32;
}

/* O(1) keyword lookup with perfect hash */
static t_token_type lookup_keyword(const char *str, int len)
{
	unsigned int hash;
	int i;
	
	if (len > 8 || len == 0)
		return TOKEN_WORD;
	
	hash = keyword_hash(str, len);
	
	/* Check keywords with matching hash (usually just 1-2) */
	for (i = 0; g_keywords[i].keyword != NULL; i++)
	{
		if (g_keywords[i].length == len && 
			keyword_hash(g_keywords[i].keyword, len) == hash &&
			memcmp(g_keywords[i].keyword, str, len) == 0)
		{
			return g_keywords[i].type;
		}
	}
	
	return TOKEN_WORD;
}

/* ============================================================================
 * HELPER FUNCTIONS
 * ============================================================================ */

/* Create a token */
static inline t_token make_token(t_scanner *scanner, t_token_type type)
{
	t_token token;
	
	token.type = type;
	token.start = scanner->start;
	token.length = (int)(scanner->current - scanner->start);
	token.line = scanner->line;
	
	return token;
}

/* Create an error token */
static inline t_token error_token(t_scanner *scanner, const char *message)
{
	t_token token;
	
	token.type = TOKEN_ERR;
	token.start = message;
	token.length = (int)strlen(message);
	token.line = scanner->line;
	
	return token;
}

/* Peek at current character without consuming */
static inline char peek(t_scanner *scanner)
{
	return *scanner->current;
}

/* Peek ahead n characters */
static inline char peek_next(t_scanner *scanner)
{
	if (*scanner->current == '\0')
		return '\0';
	return scanner->current[1];
}

/* Check if at end of input */
static inline int is_at_end(t_scanner *scanner)
{
	return *scanner->current == '\0';
}

/* Consume and return current character */
static inline char advance(t_scanner *scanner)
{
	scanner->current++;
	return scanner->current[-1];
}

/* Check if next character matches, and consume if it does */
static inline int match(t_scanner *scanner, char expected)
{
	if (is_at_end(scanner))
		return 0;
	if (*scanner->current != expected)
		return 0;
	
	scanner->current++;
	return 1;
}

/* Check if character is valid in a word */
static inline int is_word_char(char c)
{
	return (isalnum(c) || c == '_' || c == '-' || c == '.');
}

/* ============================================================================
 * CHARACTER HANDLERS (Jump Table Targets)
 * ============================================================================ */

static t_token handle_pipe(t_scanner *scanner)
{
	advance(scanner);
	
	if (match(scanner, '|'))
		return make_token(scanner, TOKEN_PIPE_PIPE);
	
	return make_token(scanner, TOKEN_PIPE);
}

static t_token handle_ampersand(t_scanner *scanner)
{
	advance(scanner);
	
	if (match(scanner, '&'))
		return make_token(scanner, TOKEN_AMP_AMP);
	
	return make_token(scanner, TOKEN_AMPERSAND);
}

static t_token handle_semicolon(t_scanner *scanner)
{
	advance(scanner);
	return make_token(scanner, TOKEN_SEMICOLON);
}

static t_token handle_greater(t_scanner *scanner)
{
	advance(scanner);
	
	if (match(scanner, '>'))
		return make_token(scanner, TOKEN_REDIR_APPEND);
	if (match(scanner, '&'))
		return make_token(scanner, TOKEN_REDIR_FD_OUT);
	if (match(scanner, '|'))
		return make_token(scanner, TOKEN_REDIR_CLOBBER);
	
	return make_token(scanner, TOKEN_REDIR_OUT);
}

static t_token handle_less(t_scanner *scanner)
{
	advance(scanner);
	
	if (match(scanner, '<'))
	{
		if (match(scanner, '-'))
			return make_token(scanner, TOKEN_REDIR_HEREDOC_STRIP);
		return make_token(scanner, TOKEN_REDIR_HEREDOC);
	}
	if (match(scanner, '&'))
		return make_token(scanner, TOKEN_REDIR_FD_IN);
	if (match(scanner, '>'))
		return make_token(scanner, TOKEN_REDIR_INOUT);
	
	return make_token(scanner, TOKEN_REDIR_IN);
}

static t_token handle_lparen(t_scanner *scanner)
{
	advance(scanner);
	return make_token(scanner, TOKEN_LEFT_PAREN);
}

static t_token handle_rparen(t_scanner *scanner)
{
	advance(scanner);
	return make_token(scanner, TOKEN_RIGHT_PAREN);
}

static t_token handle_lbrace(t_scanner *scanner)
{
	advance(scanner);
	return make_token(scanner, TOKEN_LEFT_BRACE);
}

static t_token handle_rbrace(t_scanner *scanner)
{
	advance(scanner);
	return make_token(scanner, TOKEN_RIGHT_BRACE);
}

static t_token handle_newline(t_scanner *scanner)
{
	advance(scanner);
	scanner->line++;
	return make_token(scanner, TOKEN_NEWLINE);
}

static t_token handle_squote(t_scanner *scanner)
{
	advance(scanner); /* Skip opening ' */
	
	/* Read until closing ' or EOF */
	while (peek(scanner) != '\'' && !is_at_end(scanner))
		advance(scanner);
	
	if (is_at_end(scanner))
		return error_token(scanner, "Unterminated string");
	
	advance(scanner); /* Closing ' */
	return make_token(scanner, TOKEN_SQUOTE);
}

static t_token handle_dquote(t_scanner *scanner)
{
	advance(scanner); /* Skip opening " */
	
	/* Read until closing " or EOF, handle escapes */
	while (peek(scanner) != '"' && !is_at_end(scanner))
	{
		if (peek(scanner) == '\\')
		{
			advance(scanner); /* Skip backslash */
			if (!is_at_end(scanner))
				advance(scanner); /* Skip escaped char */
		}
		else
		{
			advance(scanner);
		}
	}
	
	if (is_at_end(scanner))
		return error_token(scanner, "Unterminated string");
	
	advance(scanner); /* Closing " */
	return make_token(scanner, TOKEN_DQUOTE);
}

static t_token handle_dollar(t_scanner *scanner)
{
	advance(scanner);
	
	/* Special variables - O(1) switch */
	switch (peek(scanner))
	{
		case '?':
			advance(scanner);
			return make_token(scanner, TOKEN_DOLLAR_QUESTION);
		case '$':
			advance(scanner);
			return make_token(scanner, TOKEN_DOLLAR_DOLLAR);
		case '!':
			advance(scanner);
			return make_token(scanner, TOKEN_DOLLAR_BANG);
		case '#':
			advance(scanner);
			return make_token(scanner, TOKEN_DOLLAR_HASH);
		case '*':
			advance(scanner);
			return make_token(scanner, TOKEN_DOLLAR_STAR);
		case '@':
			advance(scanner);
			return make_token(scanner, TOKEN_DOLLAR_AT);
		case '-':
			advance(scanner);
			return make_token(scanner, TOKEN_DOLLAR_DASH);
		case '0':
			advance(scanner);
			return make_token(scanner, TOKEN_DOLLAR_ZERO);
		case '(':
			advance(scanner);
			if (match(scanner, '('))
				return make_token(scanner, TOKEN_DOLLAR_DPAREN);
			return make_token(scanner, TOKEN_DOLLAR_LPAREN);
		case '{':
			advance(scanner);
			return make_token(scanner, TOKEN_DOLLAR_LBRACE);
		default:
			return make_token(scanner, TOKEN_DOLLAR);
	}
}

static t_token handle_bquote(t_scanner *scanner)
{
	advance(scanner); /* Skip opening ` */
	
	while (peek(scanner) != '`' && !is_at_end(scanner))
		advance(scanner);
	
	if (!is_at_end(scanner))
		advance(scanner); /* Skip closing ` */
	
	return make_token(scanner, TOKEN_BQUOTE);
}

static t_token handle_backslash(t_scanner *scanner)
{
	advance(scanner);
	
	if (!is_at_end(scanner))
		advance(scanner); /* Skip escaped character */
	
	return make_token(scanner, TOKEN_BACKSLASH);
}

static t_token handle_hash(t_scanner *scanner)
{
	/* Skip comment until newline */
	while (peek(scanner) != '\n' && !is_at_end(scanner))
		advance(scanner);
	
	return make_token(scanner, TOKEN_HASH);
}

static t_token handle_tilde(t_scanner *scanner)
{
	advance(scanner);
	return make_token(scanner, TOKEN__TILDE);
}

static t_token handle_star(t_scanner *scanner)
{
	advance(scanner);
	return make_token(scanner, TOKEN_GLOB_STAR);
}

static t_token handle_question(t_scanner *scanner)
{
	advance(scanner);
	return make_token(scanner, TOKEN_GLOB_QUESTION);
}

static t_token handle_lbracket(t_scanner *scanner)
{
	advance(scanner);
	return make_token(scanner, TOKEN_GLOB_BRACKET_OPEN);
}

static t_token handle_rbracket(t_scanner *scanner)
{
	advance(scanner);
	return make_token(scanner, TOKEN_GLOB_BRACKET_COSE);
}

static t_token handle_word(t_scanner *scanner)
{
	/* Consume word characters */
	while (is_word_char(peek(scanner)))
		advance(scanner);
	
	/* Check if it's a keyword using perfect hash */
	int len = (int)(scanner->current - scanner->start);
	t_token_type type = lookup_keyword(scanner->start, len);
	
	return make_token(scanner, type);
}

static t_token handle_whitespace(t_scanner *scanner)
{
	/* Skip all whitespace */
	while (peek(scanner) == ' ' || peek(scanner) == '\t')
		advance(scanner);
	
	return make_token(scanner, TOKEN_WHITESPACE);
}

static t_token handle_eof(t_scanner *scanner)
{
	return make_token(scanner, TOKEN_EOF);
}

static t_token handle_error(t_scanner *scanner)
{
	advance(scanner);
	return error_token(scanner, "Unexpected character");
}

/* ============================================================================
 * MAIN SCANNER API
 * ============================================================================ */

/* Initialize scanner */
void scanner_init(t_scanner *scanner, const char *source)
{
	scanner->start = source;
	scanner->current = source;
	scanner->line = 1;
	
	/* Initialize jump table if not already done */
	if (!g_jump_table_initialized)
		scanner_init_jump_table();
}

/* Get next token using jump table */
t_token scanner_next_token(t_scanner *scanner)
{
	t_token token;
	
	/* Skip whitespace */
	while (peek(scanner) == ' ' || peek(scanner) == '\t')
		advance(scanner);
	
	/* Update start position */
	scanner->start = scanner->current;
	
	if (is_at_end(scanner))
		return make_token(scanner, TOKEN_EOF);
	
	/* Use jump table: O(1) dispatch to handler */
	char c = peek(scanner);
	token = g_jump_table[(unsigned char)c](scanner);
	
	/* Skip whitespace tokens and get next */
	if (token.type == TOKEN_WHITESPACE)
		return scanner_next_token(scanner);
	
	return token;
}

/* Peek at next token without consuming */
t_token scanner_peek_token(t_scanner *scanner)
{
	t_scanner temp = *scanner;
	return scanner_next_token(&temp);
}