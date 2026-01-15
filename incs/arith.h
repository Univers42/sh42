/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   arith.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dlesieur <dlesieur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/15 15:00:00 by dlesieur          #+#    #+#             */
/*   Updated: 2026/01/15 15:00:00 by dlesieur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef ARITH_H
# define ARITH_H

# include <stdbool.h>
# include <stddef.h>

/* Forward declaration */
typedef struct s_shell	t_shell;

/* Token types for arithmetic lexer */
typedef enum e_arith_tok
{
	ARITH_EOF = 0,
	ARITH_NUM,
	ARITH_VAR,
	ARITH_PLUS,
	ARITH_MINUS,
	ARITH_MUL,
	ARITH_DIV,
	ARITH_MOD,
	ARITH_POW,
	ARITH_LT,
	ARITH_GT,
	ARITH_LE,
	ARITH_GE,
	ARITH_EQ,
	ARITH_NE,
	ARITH_AND,
	ARITH_OR,
	ARITH_NOT,
	ARITH_BNOT,
	ARITH_BAND,
	ARITH_BOR,
	ARITH_BXOR,
	ARITH_SHL,
	ARITH_SHR,
	ARITH_ASSIGN,
	ARITH_INC,
	ARITH_DEC,
	ARITH_LPAREN,
	ARITH_RPAREN,
	ARITH_QUESTION,
	ARITH_COLON,
	ARITH_COMMA
}	t_arith_tok;

typedef struct s_arith_token
{
	t_arith_tok	type;
	long		value;
	char		*name;
}	t_arith_token;

typedef struct s_arith_lexer
{
	const char		*input;
	int				len;
	int				pos;
	t_arith_token	current;
	int				error;
}	t_arith_lexer;

typedef struct s_arith_ctx
{
	t_shell			*state;
	t_arith_lexer	*lexer;
	int				error;
}	t_arith_ctx;

/* Lexer functions */
void	arith_lexer_init(t_arith_lexer *lex, const char *input, int len);
int		arith_lexer_next(t_arith_lexer *lex);
void	arith_lexer_free_token(t_arith_token *tok);

/* Parser/evaluator functions */
long	arith_eval_expr(t_arith_ctx *ctx);

/* Main entry point */
char	*arith_expand(t_shell *state, const char *expr, int len);

#endif
