/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   verif_token_type.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dlesieur <dlesieur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/26 23:13:09 by dlesieur          #+#    #+#             */
/*   Updated: 2025/10/26 23:26:04 by dlesieur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "lexer.h"
#include <stdbool.h>

bool	is_redir_token(t_token_type type)
{
	return (type >= TOKEN_REDIR_IN && type <= TOKEN_REDIR_CLOBBER);
}

bool	is_logical_token(t_token_type type)
{
	return (type == TOKEN_PIPE_PIPE || type == TOKEN_AMP_AMP);
}

bool	is_keyword_token(t_token_type type)
{
	return (type >= TOKEN_IF && type <= TOKEN_SELECT);
}

bool	is_quote_token(t_token_type	type)
{
	return (type >= TOKEN_SQUOTE_START && type <= TOKEN_BQUOTE_END);
}

bool	is_expansion(t_token_type type)
{
	return (type >= TOKEN_DOLLAR && (type) <= TOKEN_DOLLAR_ZERO);
}
