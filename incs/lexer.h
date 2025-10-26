#ifndef LEXER_H
#define LEXER_H

#include "libft.h"

/**
 * 
 */
typedef enum s_token_type
{
	/* === Operators & Redirections === */
	TOKEN_PIPE,					// |	- Pipeline
	TOKEN_PIPE_PIPE,			// ||	- Logical OR
	TOKEN_AMPERSAND,			// &	- Background Job
	TOKEN_AMP_AMP,				// &&	- Logical AND
	TOKEN_SEMICOLON,			// ;	- Command separator
	TOKEN_NEWLINE,				// '\n'	- Command terminator
	/* ===	I/O	REDIRECTIONS === */
	TOKEN_REDIR_IN,				//	<	- input redirection
	TOKEN_REDIR_OUT,			//	>	- output redirection (truncate)
	TOKEN_REDIR_APPEND,			//	>>	- Output redirection (append)
	TOKEN_REDIR_HEREDOC,		//	<<	- Here document
	TOKEN_REDIR_HEREDOC_STRIP,	//	<<-	- Here document (strip tabs)
	TOKEN_REDIR_FD_IN,			//	<&	- Duplicate input fd
	TOKEN_REDIR_FD_OUT,			//	>	- Duplicate output fd
	TOKEN_REDIR_INOUT,			//	<>	- Input/Output fd
	TOKEN_REDIR_CLOBBER,		//	>|	- Force overwrite (noclobber)
	/* === GROUPING & SUBSHELLS === */
	TOKEN_LEFT_PAREN,			// 	(	- Subshell start
	TOKEN_RIGHT_PAREN,			// 	)	- Subshell end
	TOKEN_LEFT_BRACE,			// 	{	- Command group start
	TOKEN_RIGHT_BRACE,			// 	}	- Command group end
	TOKEN_GREATER,				//	>
	/* === SHELL KEYWORDS === */
	TOKEN_IF,					//	if
	TOKEN_THEN,					//	then
	TOKEN_ELIF,					//	elif
	TOKEN_FI,					//	fi
	TOKEN_ELSE,					//	else
	TOKEN_CASE,					//	case
	TOKEN_ESAC,					//	esac
	TOKEN_FOR,					//	for
	TOKEN_WHILE,				//	while
	TOKEN_UNTIL,				//	UNTIL
	TOKEN_CLASS,				//	class
	TOKEN_DO,					//	do
	TOKEN_DONE,					//	done
	TOKEN_IN,					//	in
	TOKEN_FUNCTION,				//	function
	TOKEN_SELECT,				//	select (bash extension)
	/* === WORDS & LITERALS === */
	TOKEN_SQUOTE_START,			// '	- Single quote start
	TOKEN_SQUOTE,				// ''	- type of a word that is single quote
	TOKEN_SQUOTE_END,			// '	- Single quote end
	TOKEN_DQUOTE_START,			// "	- Double quote start
	TOKEN_DQUOTE,				// ""	- Double quote type of word
	TOKEN_DQUOTE_END,			// ""	- Double quote end
	TOKEN_BQUOTE_START,			// `	- Command substitution start
	TOKEN_BQUOTE,				// ``	- Command susbstitution type of word
	TOKEN_BQUOTE_END,			// `	- Command substitution end
	/* === EXPANSIONS === */
	TOKEN_DOLLAR,				// $	- Variable/expansion prefix
	TOKEN_DOLLAR_LPAREN,		// $(	- Command substitution
	TOKEN_DOLLAR_LBRACE,		// ${	- parameter expansion
	TOKEN_DOLLAR_DPAREN,		// $((	- Arithmetic expansion
	TOKEN__TILDE,				// ~	- Home directory expansion
	TOKEN_GLOB_STAR,			// *	- Home directory expansion
	TOKEN_GLOB_QUESTION,		// ?	- Glob single char
	TOKEN_GLOB_BRACKET_OPEN,	// [	- Glob character class start
	TOKEN_GLOB_BRACKET_COSE,		// ]	- Glob character class end
	/* === SPECIAL CHARACTERS === */
	TOKEN_BACKSLASH,			// \	- Escape character
	TOKEN_HASH,					// #	- Comment start
	TOKEN_BANG,					// !	- History expansion / negation
	TOKEN_AT,					// @	- Special parameter
	TOKEN_QUESTION,				// ?	- Exit status variable
	TOKEN_DASH,					// -	- previous directory / options
	TOKEN_DOLLAR_HASH,			// $#	- Argument count
	TOKEN_DOLLAR_STAR,			// $*	- All positional parameters
	TOKEN_DOLLAR_AT,			// $@	- All positinal parameters (quoted)
	TOKEN_DOLLAR_QUESTION,		// $?	- last exist status
	TOKEN_DOLLAR_DOLLAR,		// $$	- Current shell pid
	TOKEN_DOLLAR_BANG,			// $!	- Last background job PID
	TOKEN_DOLLAR_DASH,			// $-	- current shell flags
	TOKEN_DOLLAR_ZERO,			// $0	- shell/script name

	/* === NOT SPECIFIED BUT YET USEFUL === */
	TOKEN_GREATER_GREATER,		//	>>
	TOKEN_COMMA,				// 	,
	TOKEN_DOT,					//	.
	TOKEN_MINUS,				//	-
	TOKEN_PLUS,					//	+
	TOKEN_SLASH,				//	/
	TOKEN_STAR,					//	*
	TOKEN_BANG_EQUAL,			//	!=
	TOKEN_EQUAL,				//	=
	TOKEN_EQUAL_EQUAL,			//	==
	TOKEN_GREATER_AMPERSAND,	//	>&
	TOKEN_GREATER_EQUAL,		//	>=
	TOKEN_LESS,					//	<
	TOKEN_LESS_LESS,			//	<<
	TOKEN_LESS_AMPERSAND,		//	<&
	TOKEN_LESS_EQUAL,			//	<=
	TOKEN_IDENTIFIER,			//	DEFAULT
	TOKEN_STRING,				//	""
	TOKEN_AND,					//	&&
	TOKEN_WORD,
	TOKEN_NUMBER,				//	123456789
	TOKEN_FALSE,				//	false
	TOKEN_FUN,					//	function
	TOKEN_NIL,					//	nil
	TOKEN_OR,					//	||
	TOKEN_PRINT,				//	print
	TOKEN_RETURN,				//	return
	TOKEN_SUPER,				//	super
	TOKEN_THIS,					//	this
	TOKEN_TRUE,					//	true
	TOKEN_VAR,					//	var
	/* === Control ===*/
	TOKEN_WHITESPACE,			//		- space/tab (usually ignored)
	TOKEN_EOF,					//		- eof
	TOKEN_ERR,					//		- Lexer Error
	TOKEN_COUNT,				//		- Total number of token types
}	t_token_type;

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
bool			is_redir_token(t_token_type type);
bool			is_logical_token(t_token_type type);
bool			is_keyword_token(t_token_type type);
bool			is_quote_token(t_token_type	type);
bool			is_expansion(t_token_type type);

typedef t_token (*t_char_handler)(t_scanner *scanner);

/* Character handler functions */
t_token handle_pipe(t_scanner *scanner);
t_token handle_ampersand(t_scanner *scanner);
t_token handle_semicolon(t_scanner *scanner);
t_token handle_greater(t_scanner *scanner);
t_token handle_less(t_scanner *scanner);
t_token handle_lparen(t_scanner *scanner);
t_token handle_rparen(t_scanner *scanner);
t_token handle_lbrace(t_scanner *scanner);
t_token handle_rbrace(t_scanner *scanner);
t_token handle_newline(t_scanner *scanner);
t_token handle_squote(t_scanner *scanner);
t_token handle_dquote(t_scanner *scanner);
t_token handle_dollar(t_scanner *scanner);
t_token handle_bquote(t_scanner *scanner);
t_token handle_backslash(t_scanner *scanner);
t_token handle_hash(t_scanner *scanner);
t_token handle_tilde(t_scanner *scanner);
t_token handle_star(t_scanner *scanner);
t_token handle_question(t_scanner *scanner);
t_token handle_lbracket(t_scanner *scanner);
t_token handle_rbracket(t_scanner *scanner);
t_token handle_word(t_scanner *scanner);
t_token handle_whitespace(t_scanner *scanner);
t_token handle_eof(t_scanner *scanner);
t_token handle_error(t_scanner *scanner);


#endif