/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   history_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alcacere <alcacere@student.42madrid.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/12 13:28:46 by alcacere          #+#    #+#             */
/*   Updated: 2025/12/12 13:28:48 by alcacere         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "repl_public.h"

t_dyn_str	encode_cmd_hist(char *cmd)
{
	t_dyn_str	ret;

	dyn_str_init(&ret);
	while (*cmd)
	{
		if (*cmd == '\\' || *cmd == '\n')
			dyn_str_push(&ret, '\\');
		dyn_str_push(&ret, *cmd);
		cmd++;
	}
	dyn_str_push(&ret, '\n');
	return (ret);
}

static bool	worthy_of_being_remembered(t_hist *hist, t_rl *rl)
{
	return (rl->cursor > 1 && hist->active && (!hist->cmds.len
			|| !str_slice_eq_str(rl->str.buff, rl->cursor - 1,
				*(char **)vec_idx(&hist->cmds, hist->cmds.len - 1))));
}

void	manage_history(t_hist *hist, t_rl *rl)
{
	char	*hist_entry;
	char	*enc_hist_entry;

	if (worthy_of_being_remembered(hist, rl))
	{
		hist_entry = ft_strndup(rl->str.buff, rl->cursor - 1);
		add_history(hist_entry);
		vec_push(&hist->cmds, &hist_entry);
		if (hist->append_fd >= 0)
		{
			enc_hist_entry = encode_cmd_hist(hist_entry).buff;
			if (write_to_file(enc_hist_entry, hist->append_fd) < 0)
			{
				warning_error("Failed to write to the history file");
				close(hist->append_fd);
				hist->append_fd = -1;
			}
			free(enc_hist_entry);
		}
	}
}

void	init_history(t_hist *hist, t_vec *env)
{
	*hist = (t_hist){.append_fd = -1, .active = true};
	parse_history_file(hist, env);
}

void	free_hist(t_hist *hist)
{
	size_t	i;

	i = 0;
	while (i < hist->cmds.len)
	{
		free(*(char **)vec_idx(&hist->cmds, i));
		i++;
	}
	free(hist->cmds.buff);
	vec_init(&hist->cmds, &(t_vec_config){0});
}
