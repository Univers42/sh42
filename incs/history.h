/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   history.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marvin <marvin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/10 02:32:22 by marvin            #+#    #+#             */
/*   Updated: 2026/01/10 02:32:22 by marvin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef HISTORY_H
# define HISTORY_H

/* Forward declaration to avoid circular includes */
typedef struct s_shell	t_shell;

/* History file name (stored in $HOME) */
# ifndef HIST_FILE
#  define HIST_FILE ".minishell_history"
# endif

/* Maximum history entries to keep (0 = unlimited) */
# ifndef HIST_MAX_ENTRIES
#  define HIST_MAX_ENTRIES 1000
# endif

typedef struct s_history
{
	bool		hist_active;
	int			append_fd;
	t_vec		hist_cmds;
}	t_history;

/* History management functions */
void		manage_history(t_shell *state);
void		init_history(t_shell *state);
void		free_hist(t_shell *state);
void		parse_history_file(t_shell *state);
t_string	encode_cmd_hist(char *cmd);
char		*get_hist_file_path(t_shell *state);
t_string	parse_single_cmd(t_string hist, size_t *cur);
t_vec		parse_hist_file(t_string hist);
bool		worthy_of_being_remembered(t_shell *state);

/* Readline history setup */
void		setup_history_search(void);

/* History expansion (!, !!, !word, !number, !-number) */
char		*history_expand_line(t_shell *state, const char *input,
				int *expanded);

#endif