/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marvin <marvin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/10 02:20:12 by marvin            #+#    #+#             */
/*   Updated: 2026/01/10 02:20:12 by marvin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef LEXER_H
# define LEXER_H

# include "libft.h"
# include <stdbool.h>
# include "public/token.h"
# define LEXER_SQUOTE_PROMPT "squote> "
# define LEXER_DQUOTE_PROMPT "dquote> "

typedef struct s_op_map
{
	char	*str;
	t_tt	t;
}	t_op_map;

typedef struct s_deque_tt
{
	t_deque	deqtok;
	char	looking_for;
}	t_deque_tt;

/* Function prototypes */
char	*tokenizer(char *str, t_deque_tt *ret);
int		advance_dquoted(char **str);
int		advance_squoted(char **str);
bool	is_space(char c);
bool	is_special_char(char c);
char	*tt_to_str(t_tt tt);
void	print_tokens(t_deque_tt *tokens);

#endif