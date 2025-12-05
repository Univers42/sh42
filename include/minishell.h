/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dlesieur <dlesieur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/04 13:50:55 by dlesieur          #+#    #+#             */
/*   Updated: 2025/12/04 14:03:50 by dlesieur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

#include "libft.h"

# define PROMPT "\002> \002"
# define HIST_FILE ".minishell_history"
# define LEXER_SQUOTE_PROMPT "squote> "
# define LEXER_DQUOTE_PROMPT "dquote> "

uint32_t	should_unwind;

typedef struct s_state
{
	t_dyn_str	input;
}	t_state;

#endif