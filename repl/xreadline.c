/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   xreadline.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alcacere <alcacere@student.42madrid.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/12 13:30:45 by alcacere          #+#    #+#             */
/*   Updated: 2025/12/12 15:14:52 by alcacere         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "repl_public.h"
#include <sys/wait.h>
#include <unistd.h>

static int	return_last_line(t_rl *rl, t_dyn_str *ret)
{
	int	len;

	len = rl->str.len - rl->cursor;
	dyn_str_pushnstr(ret, rl->str.buff + rl->cursor, len);
	rl->cursor = 0;
	rl->str.len = 0;
	rl->has_line = false;
	if (len == 0)
		return (1);
	return (4);
}

static int	return_new_line(t_rl *rl, t_dyn_str *ret, char **context,
		char **base_context)
{
	char	*temp;
	int		len;

	rl->lineno++;
	update_context(rl, context, base_context);
	temp = ft_strchr(rl->str.buff + rl->cursor, '\n');
	if (temp == 0)
		return (return_last_line(rl, ret));
	len = temp - (rl->str.buff + rl->cursor) + 1;
	if (len)
		dyn_str_pushnstr(ret, rl->str.buff + rl->cursor, len);
	rl->cursor += len;
	rl->has_line = rl->cursor != rl->str.len;
	if (len == 0)
		return (1);
	return (4);
}

static int	handle_get_input_code(int code, t_xreadline_params *p)
{
	if (code == 1)
	{
		p->rl->has_finished = true;
		return (1);
	}
	if (code == 2)
	{
		set_cmd_status(p->last_cmd_status_res,
			(t_status){.status = CANCELED, .c_c = true},
			p->last_cmd_status_s);
		return (2);
	}
	return (0);
}

static int	get_new_input_if_needed(t_xreadline_params *p)
{
	int	code;
	int	ret_code;

	if (p->rl->has_line)
		return (0);
	if (*p->input_method == INP_ARG || *p->input_method == INP_FILE)
	{
		p->rl->has_finished = true;
		return (1);
	}
	if (*p->input_method == INP_STDIN_NOTTY)
		code = get_more_input_notty(p->rl);
	else
		code = get_more_input_readline(p->rl, p->prompt);
	ret_code = handle_get_input_code(code, p);
	if (ret_code != 0)
		return (ret_code);
	if (*p->input_method == INP_READLINE)
		dyn_str_push(&p->rl->str, '\n');
	p->rl->has_line = true;
	return (0);
}

int	xreadline(t_xreadline_params *p)
{
	int	code;

	if (p->rl->has_finished)
		return (0);
	code = get_new_input_if_needed(p);
	if (code == 1)
		return (0);
	else if (code == 2)
		return (2);
	return (return_new_line(p->rl, p->ret, p->context, p->base_context));
}
