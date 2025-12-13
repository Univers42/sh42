/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_more_tokens.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alcacere <alcacere@student.42madrid.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/12 13:28:15 by alcacere          #+#    #+#             */
/*   Updated: 2025/12/12 13:28:19 by alcacere         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "repl_public.h"

static int	readline_cmd_wrapper(t_tokenizer_ctx *ctx)
{
	int	stat;

	stat = xreadline(ctx->rl, ctx->input, *(ctx->prompt),
			ctx->last_cmd_status_res, ctx->last_cmd_status_s, ctx->input_method,
			ctx->context, ctx->base_context);
	if (stat == 0)
		return (1);
	if (stat == 2)
	{
		if (*(ctx->input_method) != INP_READLINE)
			*(ctx->should_exit) = true;
		return (2);
	}
	return (0);
}

static void	extend_bs(t_tokenizer_ctx *ctx)
{
	char	*prompt;

	while (ends_with_bs_nl(*(ctx->input)))
	{
		dyn_str_pop(ctx->input);
		dyn_str_pop(ctx->input);
		prompt = ft_strdup("> ");
		if (readline_cmd_wrapper(ctx))
		{
			free(prompt);
			return ;
		}
		free(prompt);
	}
}

static void	handle_eof(t_tokenizer_ctx *ctx)
{
	char	*context_str;

	if (ctx->context && *(ctx->context))
		context_str = *(ctx->context);
	else
		context_str = "input";
	if (ctx->looking_for && ctx->input->len)
	{
		ft_eprintf("%s: unexpected EOF while looking for matching `%%c'\n",
			context_str, ctx->looking_for);
	}
	else if (ctx->input->len)
		ft_eprintf("%s: syntax error: unexpected end of file\n", context_str);
	if (*(ctx->input_method) == INP_READLINE)
		ft_eprintf("exit\n");
	if (!ctx->last_cmd_status_res->status && ctx->input->len)
		set_cmd_status(ctx->last_cmd_status_res,
			(t_status){.status = SYNTAX_ERR}, ctx->last_cmd_status_s);
	*(ctx->should_exit) = true;
}

void	get_more_tokens(t_tokenizer_ctx *ctx)
{
	int		stat;
	char	*curr_prompt;
	char	*next_prompt;

	while (*(ctx->prompt))
	{
		curr_prompt = *(ctx->prompt);
		stat = readline_cmd_wrapper(ctx);
		if (stat)
		{
			if (stat == 1)
				handle_eof(ctx);
			free(curr_prompt);
			return ;
		}
		extend_bs(ctx);
		next_prompt = tokenizer(ctx->input->buff, ctx->out_tokens);
		free(curr_prompt);
		if (next_prompt)
			*(ctx->prompt) = ft_strdup(next_prompt);
		else
			*(ctx->prompt) = NULL;
	}
}

bool	end_with_bs_nl(t_dyn_str s)
{
	size_t	i;
	bool	unterminated;

	if (s.len == 0)
		return (false);
	i = s.len;
	unterminated = false;
	if (s.buff[--i] != '\n')
		return (false);
	while (i > 0)
	{
		i--;
		if (s.buff[i] == '\\')
			unterminated = !unterminated;
		else
			break ;
	}
	return (unterminated);
}
