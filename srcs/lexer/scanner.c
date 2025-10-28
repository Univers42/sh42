#include "lexer.h"
#include <stdio.h>
#include <string.h> // for strlen, memcmp if needed

t_token scan_token(t_scanner *scan)
{
	skip_unused_data(scan);
	scan->start = scan->current;
	if (scan_is_at_end(scan))
		return make_token(scan, TOKEN_EOF);
	const t_dispatch_data *dispatch = get_dispatch_singleton();
	unsigned char c = (unsigned char)*scan->current;

	/* Fast path for common literal chars */
	if (c < 64 && !(dispatch->fast_path_map & (1ULL << c)))
		return handle_literal(scan);

	/* Alphabetic or underscore -> identifier */
	if (ft_isalpha(c) || c == '_')
		return identifier(scan);

	/* Digit -> number */
	if (ft_isdigit(c))
		return number(scan);

	if (c < 128)
		return dispatch->dispatch_table[c](scan);
	return handle_literal(scan);
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
    const char *name = "UNKNOWN";
    switch (token->type)
    {
        case TOKEN_OPERATOR: name = "TOKEN_OPERATOR"; break;
        case TOKEN_PIPE: name = "TOKEN_PIPE"; break;
        case TOKEN_PIPE_PIPE: name = "TOKEN_PIPE_PIPE"; break;
        case TOKEN_AMPERSAND: name = "TOKEN_AMPERSAND"; break;
        case TOKEN_AMP_AMP: name = "TOKEN_AMP_AMP"; break;
        case TOKEN_SEMICOLON: name = "TOKEN_SEMICOLON"; break;
        case TOKEN_NEWLINE: name = "TOKEN_NEWLINE"; break;

        case TOKEN_ARITH_OP: name = "TOKEN_ARITH_OP"; break;
        case TOKEN_OP_INCREMENT: name = "TOKEN_OP_INCREMENT"; break;
        case TOKEN_OP_DECREMENT: name = "TOKEN_OP_DECREMENT"; break;
        case TOKEN_OP_ADD: name = "TOKEN_OP_ADD"; break;
        case TOKEN_OP_MINUS: name = "TOKEN_OP_MINUS"; break;
        case TOKEN_OP_MULT: name = "TOKEN_OP_MULT"; break;
        case TOKEN_OP_DIV: name = "TOKEN_OP_DIV"; break;
        case TOKEN_OP_MOD: name = "TOKEN_OP_MOD"; break;
        case TOKEN_COMP_EQ: name = "TOKEN_COMP_EQ"; break;
        case TOKEN_COMP_NE: name = "TOKEN_COMP_NE"; break;
        case TOKEN_COMP_GT: name = "TOKEN_COMP_GT"; break;
        case TOKEN_COMP_LT: name = "TOKEN_COMP_LT"; break;
        case TOKEN_COMP_GTEQ: name = "TOKEN_COMP_GTEQ"; break;
        case TOKEN_COMP_LTEQ: name = "TOKEN_COMP_LTEQ"; break;
        case TOKEN_LOGICAL_AND: name = "TOKEN_LOGICAL_AND"; break;
        case TOKEN_LOGICAL_OR: name = "TOKEN_LOGICAL_OR"; break;
        case TOKEN_TERNARY_COND: name = "TOKEN_TERNARY_COND"; break;
        case TOKEN_TERNARY_COLON: name = "TOKEN_TERNARY_COLON"; break;
        case TOKEN_SIZEOF: name = "TOKEN_SIZEOF"; break;
        case TOKEN_OP_SHIFT_LEFT: name = "TOKEN_OP_SHIFT_LEFT"; break;
        case TOKEN_OP_SHIFT_RIGHT: name = "TOKEN_OP_SHIFT_RIGHT"; break;
        case TOKEN_OP_BITWISE_AND: name = "TOKEN_OP_BITWISE_AND"; break;
        case TOKEN_OP_BITWISE_OR: name = "TOKEN_OP_BITWISE_OR"; break;
        case TOKEN_OP_BITWISE_XOR: name = "TOKEN_OP_BITWISE_XOR"; break;
        case TOKEN_ASS_EQUAL: name = "TOKEN_ASS_EQUAL"; break;
        case TOKEN_ASS_PLUS: name = "TOKEN_ASS_PLUS"; break;
        case TOKEN_ASS_MULT: name = "TOKEN_ASS_MULT"; break;
        case TOKEN_ASS_DIV: name = "TOKEN_ASS_DIV"; break;
        case TOKEN_ASS_MOD: name = "TOKEN_ASS_MOD"; break;
        case TOKEN_ASS_MINUS: name = "TOKEN_ASS_MINUS"; break;
        case TOKEN_ASS_OR: name = "TOKEN_ASS_OR"; break;
        case TOKEN_ASSS_AND: name = "TOKEN_ASSS_AND"; break;
        case TOKEN_ASS_SHIFT: name = "TOKEN_ASS_SHIFT"; break;
        case TOKEN_ASS_UNSHIFT: name = "TOKEN_ASS_UNSHIFT"; break;
        case TOKEN_ASS_XOR: name = "TOKEN_ASS_XOR"; break;

        case TOKEN_REDIR_IO: name = "TOKEN_REDIR_IO"; break;
        case TOKEN_REDIR_IN: name = "TOKEN_REDIR_IN"; break;
        case TOKEN_REDIR_OUT: name = "TOKEN_REDIR_OUT"; break;
        case TOKEN_REDIR_APPEND: name = "TOKEN_REDIR_APPEND"; break;
        case TOKEN_REDIR_HEREDOC: name = "TOKEN_REDIR_HEREDOC"; break;
        case TOKEN_REDIR_HEREDOC_STRIP: name = "TOKEN_REDIR_HEREDOC_STRIP"; break;
        case TOKEN_REDIR_FD_IN: name = "TOKEN_REDIR_FD_IN"; break;
        case TOKEN_REDIR_FD_OUT: name = "TOKEN_REDIR_FD_OUT"; break;
        case TOKEN_REDIR_INOUT: name = "TOKEN_REDIR_INOUT"; break;
        case TOKEN_REDIR_CLOBBER: name = "TOKEN_REDIR_CLOBBER"; break;

        case TOKEN_GROUPING: name = "TOKEN_GROUPING"; break;
        case TOKEN_LEFT_PAREN: name = "TOKEN_LEFT_PAREN"; break;
        case TOKEN_RIGHT_PAREN: name = "TOKEN_RIGHT_PAREN"; break;
        case TOKEN_LEFT_BRACE: name = "TOKEN_LEFT_BRACE"; break;
        case TOKEN_RIGHT_BRACE: name = "TOKEN_RIGHT_BRACE"; break;
        case TOKEN_CMD_SUBST_START: name = "TOKEN_CMD_SUBST_START"; break;
        case TOKEN_ARITH_EXP_START: name = "TOKEN_ARITH_EXP_START"; break;

        case TOKEN_KEYWORDS: name = "TOKEN_KEYWORDS"; break;
        case TOKEN_IF: name = "TOKEN_IF"; break;
        case TOKEN_THIS: name = "TOKEN_THIS"; break;
        case TOKEN_THEN: name = "TOKEN_THEN"; break;
        case TOKEN_ELIF: name = "TOKEN_ELIF"; break;
        case TOKEN_FI: name = "TOKEN_FI"; break;
        case TOKEN_ELSE: name = "TOKEN_ELSE"; break;
        case TOKEN_CASE: name = "TOKEN_CASE"; break;
        case TOKEN_ESAC: name = "TOKEN_ESAC"; break;
        case TOKEN_FOR: name = "TOKEN_FOR"; break;
        case TOKEN_WHILE: name = "TOKEN_WHILE"; break;
        case TOKEN_UNTIL: name = "TOKEN_UNTIL"; break;
        case TOKEN_DO: name = "TOKEN_DO"; break;
        case TOKEN_DONE: name = "TOKEN_DONE"; break;
        case TOKEN_IN: name = "TOKEN_IN"; break;
        case TOKEN_FUNCTION: name = "TOKEN_FUNCTION"; break;
        case TOKEN_SELECT: name = "TOKEN_SELECT"; break;
        case TOKEN_CLASS: name = "TOKEN_CLASS"; break;
        case TOKEN_VAR: name = "TOKEN_VAR"; break;
        case TOKEN_RETURN: name = "TOKEN_RETURN"; break;
        case TOKEN_SUPER: name = "TOKEN_SUPER"; break;
        case TOKEN_PRINT: name = "TOKEN_PRINT"; break;
        case TOKEN_TRUE: name = "TOKEN_TRUE"; break;
        case TOKEN_FALSE: name = "TOKEN_FALSE"; break;
        case TOKEN_NIL: name = "TOKEN_NIL"; break;

        case TOKEN_LITERALS: name = "TOKEN_LITERALS"; break;
        case TOKEN_IDENTIFIER: name = "TOKEN_IDENTIFIER"; break;
        case TOKEN_WORD: name = "TOKEN_WORD"; break;
        case TOKEN_FLAG: name = "TOKEN_FLAG"; break;
        case TOKEN_NUMBER: name = "TOKEN_NUMBER"; break;
        case TOKEN_SINGLE_QUOTED_STRING: name = "TOKEN_SINGLE_QUOTED_STRING"; break;
        case TOKEN_DOUBLE_QUOTED_STRING: name = "TOKEN_DOUBLE_QUOTED_STRING"; break;
        case TOKEN_BQUOTE: name = "TOKEN_BQUOTE"; break;

        case TOKEN_INHIBITORS: name = "TOKEN_INHIBITORS"; break;
        case TOKEN_BACKSLASH: name = "TOKEN_BACKSLASH"; break;
        case TOKEN_DQUOTE_START: name = "TOKEN_DQUOTE_START"; break;
        case TOKEN_SQUOTE_START: name = "TOKEN_SQUOTE_START"; break;
        case TOKEN_BQUOTE_START: name = "TOKEN_BQUOTE_START"; break;
        case TOKEN_DQUOTE_END: name = "TOKEN_DQUOTE_END"; break;
        case TOKEN_BQUOTE_END: name = "TOKEN_BQUOTE_END"; break;
        case TOKEN_SQUOTE_END: name = "TOKEN_SQUOTE_END"; break;

        case TOKEN_PAR_FORMAT: name = "TOKEN_PAR_FORMAT"; break;
        case TOKEN_PAR_EXP_START: name = "TOKEN_PAR_EXP_START"; break;
        case TOKEN_COLON_DASH: name = "TOKEN_COLON_DASH"; break;
        case TOKEN_COLON_EQUAL: name = "TOKEN_COLON_EQUAL"; break;
        case TOKEN_COLON_QUESTION: name = "TOKEN_COLON_QUESTION"; break;
        case TOKEN_COLON_PLUS: name = "TOKEN_COLON_PLUS"; break;
        case TOKEN_HASH_OP: name = "TOKEN_HASH_OP"; break;
        case TOKEN_PERCENT: name = "TOKEN_PERCENT"; break;
        case TOKEN_PERCENT_PERCENT: name = "TOKEN_PERCENT_PERCENT"; break;
        case TOKEN_POUND: name = "TOKEN_POUND"; break;
        case TOKEN_POUND_POUND: name = "TOKEN_POUND_POUND"; break;

        case TOKEN_PROCESS_SUBST: name = "TOKEN_PROCESS_SUBST"; break;
        case TOKEN_SUBST_IN: name = "TOKEN_SUBST_IN"; break;
        case TOKEN_SUBST_OUT: name = "TOKEN_SUBST_OUT"; break;

        case TOKEN_GLOBBING: name = "TOKEN_GLOBBING"; break;
        case TOKEN_GLOB_STAR: name = "TOKEN_GLOB_STAR"; break;
        case TOKEN_GLOB_QUESTION: name = "TOKEN_GLOB_QUESTION"; break;
        case TOKEN_GLOB_BRACKET_OPEN: name = "TOKEN_GLOB_BRACKET_OPEN"; break;
        case TOKEN_GLOB_BRACKET_CLOSE: name = "TOKEN_GLOB_BRACKET_CLOSE"; break;
        case TOKEN_GLOB_NEGATION: name = "TOKEN_GLOB_NEGATION"; break;

        case TOKEN_HIST_EXP: name = "TOKEN_HIST_EXP"; break;
        case TOKEN_BANG_BANG: name = "TOKEN_BANG_BANG"; break;
        case TOKEN_BANG_WORD: name = "TOKEN_BANG_WORD"; break;
        case TOKEN_BANG_NUMBER: name = "TOKEN_BANG_NUMBER"; break;
        case TOKEN_BANG_DASH_NUMBER: name = "TOKEN_BANG_DASH_NUMBER"; break;

        case TOKEN_SPECIAL_PARAMS: name = "TOKEN_SPECIAL_PARAMS"; break;
        case TOKEN_DOLLAR_HASH: name = "TOKEN_DOLLAR_HASH"; break;
        case TOKEN_DOLLAR_STAR: name = "TOKEN_DOLLAR_STAR"; break;
        case TOKEN_DOLLAR_AT: name = "TOKEN_DOLLAR_AT"; break;
        case TOKEN_DOLLAR_QUESTION: name = "TOKEN_DOLLAR_QUESTION"; break;
        case TOKEN_DOLLAR_DOLLAR: name = "TOKEN_DOLLAR_DOLLAR"; break;
        case TOKEN_DOLLAR_BANG: name = "TOKEN_DOLLAR_BANG"; break;
        case TOKEN_DOLLAR_DASH: name = "TOKEN_DOLLAR_DASH"; break;
        case TOKEN_DOLLAR_ZERO: name = "TOKEN_DOLLAR_ZERO"; break;
        case TOKEN_AT: name = "TOKEN_AT"; break;
        case TOKEN_QUESTION: name = "TOKEN_QUESTION"; break;

        case TOKEN_SPECIAL_CHARS: name = "TOKEN_SPECIAL_CHARS"; break;
        case TOKEN_TILDE: name = "TOKEN_TILDE"; break;
        case TOKEN_HASH: name = "TOKEN_HASH"; break;
        case TOKEN_BANG: name = "TOKEN_BANG"; break;
        case TOKEN_DOLLAR: name = "TOKEN_DOLLAR"; break;
        case TOKEN_DASH: name = "TOKEN_DASH"; break;
        case TOKEN_COMMA: name = "TOKEN_COMMA"; break;
        case TOKEN_DOT: name = "TOKEN_DOT"; break;

        case TOKEN_CONTROL: name = "TOKEN_CONTROL"; break;
        case TOKEN_WHITESPACE: name = "TOKEN_WHITESPACE"; break;
        case TOKEN_EOF: name = "TOKEN_EOF"; break;
        case TOKEN_ERR: name = "TOKEN_ERR"; break;
        case TOKEN_COUNT: name = "TOKEN_COUNT"; break;

        default: name = "UNKNOWN"; break;
    }

    printf("[token] %-16s | line %d | '%.*s'\n",
           name, token->line, token->length, token->start);
}

