/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   prompt.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dlesieur <dlesieur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/19 07:35:12 by anddokhn          #+#    #+#             */
/*   Updated: 2025/12/12 16:20:00 by dlesieur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "repl_public.h"

static void	init_prompt_data(t_prompt_data *data, t_status *st_res)
{
	data->user = getenv("USER");
	if (!data->user)
		data->user = "inferno";
	cap_cmd("pwd", &data->cwd);
	data->git = get_git();
	data->cols = get_cols();
	data->status = st_res->status;
	data->short_path = shorten_path(data->cwd.buff, data->cols - 60);
}

static void	free_prompt_data(t_prompt_data *data)
{
	free(data->cwd.buff);
	free(data->short_path);
	if (data->git.data)
		free(data->git.data);
}

t_dyn_str	prompt_normal(t_status *st_res, char **st_s)
{
	t_dyn_str		p;
	t_prompt_data	data;

	(void)st_s;
	dyn_str_init(&p);
	ensure_locale();
	init_prompt_data(&data, st_res);
	build_left_side_line1(&p, &data);
	build_right_side_line1(&p, &data);
	build_line2(&p, st_res);
	free_prompt_data(&data);
	return (p);
}

static const char	*get_token_str(t_token_type type)
{
	if (type == TOKEN_LEFT_BRACE)
		return ("subsh");
	if (type == TOKEN_PIPE)
		return ("pipe");
	if (type == TOKEN_LOGICAL_AND)
		return ("and");
	if (type == TOKEN_LOGICAL_OR)
		return ("or");
	return (NULL);
}

t_dyn_str	prompt_more_input(t_parse *parser)
{
	t_dyn_str		ret;
	t_token_type	curr;
	size_t			i;
	const char		*token_str;

	dyn_str_init(&ret);
	dyn_str_pushstr(&ret, FG_CHARCOAL);
	i = 0;
	while (i < parser->stack.len)
	{
		curr = (t_token_type)(uintptr_t)vec_idx(&parser->stack, i++);
		token_str = get_token_str(curr);
		if (token_str)
		{
			dyn_str_pushstr(&ret, token_str);
			dyn_str_pushstr(&ret, " ");
		}
	}
	dyn_str_pushstr(&ret, RESET);
	dyn_str_pushstr(&ret, FG_EMBER);
	dyn_str_pushstr(&ret, "..> ");
	dyn_str_pushstr(&ret, RESET);
	return (ret);
}
