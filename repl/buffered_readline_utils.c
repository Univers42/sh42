/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   buffered_readline_utils.c                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alcacere <alcacere@student.42madrid.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/12 13:28:08 by alcacere          #+#    #+#             */
/*   Updated: 2025/12/12 13:28:10 by alcacere         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "repl_public.h"
#include <errno.h>
#include <unistd.h>
#include "ft_stdio.h"
#include "trap.h"

void	buff_readline_update(t_rl *l)
{
	l->has_line = l->cursor != l->str.len;
}

void	buff_readline_reset(t_rl *l)
{
	ft_memmove(l->str.buff, l->str.buff + l->cursor, l->str.len - l->cursor);
	l->str.len -= l->cursor;
	if (l->str.buff)
		l->str.buff[l->str.len] = 0;
	l->cursor = 0;
	buff_readline_update(l);
}

void	buff_readline_init(t_rl *ret)
{
	*ret = (t_rl){0};
}

void	update_context(t_rl *rl, char **context, char **base_context)
{
	char	*new_ctx;
	int		len;
	char	*lineno_str;

	if (!rl->should_update_ctx)
		return ;
	len = ft_strlen(*base_context) + 16;
	new_ctx = (char *)malloc((size_t)len);
	if (!new_ctx)
		return ;
	ft_strlcpy(new_ctx, *base_context, len);
	ft_strlcat(new_ctx, ": line ", len);
	lineno_str = ft_itoa(rl->lineno);
	if (lineno_str)
	{
		ft_strlcat(new_ctx, lineno_str, len);
		free(lineno_str);
	}
	if (*context)
		free(*context);
	*context = new_ctx;
}

int	get_more_input_notty(t_rl *rl)
{
	char	buff[4096 * 2];
	int		ret;
	int		status;

	status = 1;
	set_unwind_sig_norestart();
	while (1)
	{
		ret = read(0, buff, sizeof(buff));
		if (ret < 0 && errno == EINTR)
			status = 2;
		if (ret == 0)
			rl->has_finished = true;
		if (ret == 0)
			dyn_str_pushstr(&rl->str, "\n");
		if (ret <= 0)
			break ;
		status = 0;
		dyn_str_pushnstr(&rl->str, buff, ret);
		if (ft_strnchr(buff, '\n', ret))
			break ;
	}
	set_unwind_sig();
	buff_readline_update(rl);
	return (status);
}
