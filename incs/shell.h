/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   shell.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dlesieur <dlesieur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/30 18:09:42 by dlesieur          #+#    #+#             */
/*   Updated: 2025/10/30 18:09:43 by dlesieur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SHELL_H
# define SHELL_H

#include "libft.h"
#include <readline/readline.h>
#include <readline/history.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include "libft.h"

/**
 * structure to pass aruond that holds a bitmap of file 
 * descriptors to close, and the size of that structure. 
 */
# define FD_BITMAP_SIZE 32
# define CTLEC			'\001'
# define CTLNUL			'\177'
# define HEREDOC_MAX 16

typedef struct s_fd_bitmap
{
	int		size;
	char	*bitmap;
}	t_fd_bitmap;


typedef struct s_user_info
{
	size_t	uid;
	size_t	euid;
	size_t	saveuid;
	size_t	gid;
	size_t	egid;
	size_t	save_gid;
	char	*user_name;
	char	*shell;
	char	*home_dir;
}	t_user_info;

typedef struct s_parser_state
{
	int		parser_state;
	int		*token_state;
	int		parsing_command;
	char	*token;
	size_t	token_buffer_size;
	int		eof_token;
	int		input_line_terminator;
	int		eof_encountered;
	int		eol_lookahead;
	char	**prompt_string_pointer;
	int		cur_cmd_line_count;
	int		remember_on_history;
	int		history_expansion_inhibited;
	int		last_command_exit_value;
	t_array	*pipestatus;
	int		expand_aliases;
	int		echo_input_at_read;
	int		need_here_doc;
	int		here_doc_first_line;
	int		esacs_needed;
	int		expecting_in;
	int		incmd;
	void	*pushed_strings;
	//t_redirect	*redir_stack[HEREDOC_MAX];
}	t_parser_state;

typedef struct s_input_state
{
	char	*input_line;
	size_t	input_line_index;
	size_t	input_line_size;
	size_t	input_line_len;
	char	*input_property;
	size_t	input_propsize;
}	t_input_state;


#endif
