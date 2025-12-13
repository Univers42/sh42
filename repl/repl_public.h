/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   repl_public.h                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alcacere <alcacere@student.42madrid.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/12 15:14:16 by alcacere          #+#    #+#             */
/*   Updated: 2025/12/12 15:14:18 by alcacere         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef REPL_PUBLIC_H
# define REPL_PUBLIC_H

# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include <unistd.h>
# include <sys/ioctl.h>
# include <sys/wait.h>
# include <sys/time.h>
# include <wchar.h>
# include <locale.h>
# include <errno.h>
# include <stdbool.h>
# include <readline/readline.h>

# include "libft.h"
# include "parser.h"
# include "lexer.h"
# include "ft_colors.h"
# include "system.h"
# include "ft_wctype.h"
# include "ft_readline.h" 

typedef struct s_xreadline_params
{
	t_rl		*rl;
	t_dyn_str	*ret;
	char		*prompt;
	t_status	*last_cmd_status_res;
	char		**last_cmd_status_s;
	int			*input_method;
	char		**context;
	char		**base_context;
}				t_xreadline_params;

typedef struct s_tokenizer_ctx
{
	t_rl		*rl;
	char		**prompt;
	t_dyn_str	*input;
	t_status	*last_cmd_status_res;
	char		**last_cmd_status_s;
	int			*input_method;
	char		**context;
	char		**base_context;
	bool		*should_exit;
	t_deque		*out_tokens;
	char		looking_for;
}				t_tokenizer_ctx;

int		xreadline(t_xreadline_params *p);
void	get_more_tokens(t_tokenizer_ctx *ctx);
bool	ends_with_bs_nl(t_dyn_str s);

const char	*tilde_expand(const char *path, t_dyn_str *r);
void		child_process_cap(int *pp, const char *cmd);
void		read_from_pipe(int fd, t_dyn_str *out);

typedef struct s_prompt_data
{
	t_dyn_str	cwd;
	t_vcs_info	git;
	char		*user;
	char		*short_path;
	int			cols;
	int			status;
}				t_prompt_data;

int			get_cols(void);
int			vis_width(const char *s);
char		*shorten_path(const char *path, int max_w);
int			cap_cmd(const char *cmd, t_dyn_str *out);
t_vcs_info	get_git(void);
void		build_left_side_line1(t_dyn_str *p, t_prompt_data *data);
void		build_right_side_line1(t_dyn_str *p, t_prompt_data *data);
void		build_line2(t_dyn_str *p, t_status *st_res);
t_dyn_str	prompt_normal(t_status *st_res, char **st_s);
t_dyn_str	prompt_more_input(t_parse *parser);

char		*getpid_hack(void);
t_status	res_status(int status);
void		init_cwd(t_dyn_str *_cwd);

#endif
