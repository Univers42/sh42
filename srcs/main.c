/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dlesieur <dlesieur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/04 13:49:41 by dlesieur          #+#    #+#             */
/*   Updated: 2025/12/11 18:34:35 by dlesieur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include "libft.h"

/**
 * REPL algorithm used really common algorithm for the shell program
 * it runs forever until a condiiton is met to cut the program safely or abort
 * through signals for instance..
 *
 * We use arena for several reasons :
 * arena allow O(1) allocation (bump) thank to lifoba library
 * very cheap reset to free everything at once. it's ideal for REPL
 * we stop tracking and individually freeing many objects.
 * finally it avoid the fragmentation and reduce malloc overhead
 *
 * the pitfall of this arena is that is's not thread safe,, for that we should
 * recreate our own malloc or shel malloc, things that will do later on..
 *
 * - other caveat is that we cannot put global state or things that needs
 * special care into the arena.
 * This arena is not specific context so we cannot
 *
 * - lifoba give us a fast stack/arena allocator sepcialized for short-lived
 * parsing/execution data
 * - more precisely :
 * it will handle AST Nodes, tokens, parse tree, temporarary strings, redirect ndoes
 * small vectors used only during parse/execute
 *
 * !What we shouldn't put into the arena
 * long-lived/global objects (env, persistent caches, history)
 * Resources requiring non-trivial teardown (open file descriptors, sockets, DIR*,
 * malloced internals that must best be free)
 * pointers kept across a pop/reset -- they become invalid
 */
typedef struct s_hellish
{
	t_stream_dft_data	*repl;
	t_dyn_str			cwd;
	t_vec				redirs;
	int					iheredoc;
} t_hellish;

t_stream_dft_data *get_repl(void)
{
	t_stream_dft_data data = {0};

	return (&data);
}

void shut_app(void (*destroy)(void *), void *arg)
{
	destroy(arg);
	forward_exit_status(arg->);
}

static t_status res_status(int status)
{
	return ((t_status){.status = status, .pid = -1, .c_c = false});
}

static void init_cwd(t_dyn_str *_cwd)
{
	char *cwd;

	dyn_str_init(_cwd);
	cwd = getcwd(NULL, 0);
	if (!cwd)
		cwd = getenv("PWD");
	if (cwd)
		dyn_str_pushstr(_cwd, cwd);
	else
		ft_eprintf("Shell-init: getcwd failed: %s\n", strerror(errno));
	free(cwd);
}

// manual liberation, better not to do it like that but temporary situation.
void cleanse(t_hellish *hell)
{
	t_stream_dft_data *repl;

	repl = hell->repl;
	dyn_str_reset(repl->input);
	free(repl->last_cmd_status_s);
	free(repl->pid);
	reset_heap_memb(&repl->base_context, NULL, 0);
	reset_heap_memb(&repl->context, NULL, 0);
	free(repl->rl.str.buff);
	free_hist(repl->hist);
	free(repl->cwd.buff);
	free_env(repl->env);
}

static t_status init_shell(t_hellish *shell, t_stream_dft_data *repl, **argv, char **envp)
{
	set_unwind_sig();
	*shell = {0};
	repl->pid = xgetpid();
	repl->context = ft_strdup(argv[0]);
	repl->base_context = ft_strdup(argv[0]);
	set_cmd_status(&repl->last_cmd_status_res, res_status(0), &repl->last_cmd_status_s);
	repl->last_cmd_status_res = res_status(0);
	init_cwd(&repl->cwd);
	repl->env = env_to_vec_env(&shell->cwd, envp);
	(void)hellish;
}

static void parse_execute(t_hellish *shell,)
{
	char	*prompt;
	t_deque	tt;
	t_fnctx	ctx;

	ctx = (t_fnctx){.fn (void (*)(void *) cleanse), .arg = &shell};
}


t_status run_sh(t_hellish *shell, t_stream_dft_data *repl)
{
	repl = shell->repl;
	while (repl->should_exit)
	{
		dyn_str_init(&repl->input);
		get_g_sig()->should_unwind = 0;
		parse_execute(&repl);
		if (shell->repl->cursor > 1)
			manage_history(repl->hist, repl->hist);
		buff_readline_reset(&repl.rl);
		reset_heap_memb(&repl->input.buff, &repl->input, sizeof(repl->input));
	}
	return (repl->should_exit);
}

int	main(int argc, char **argv, char **envp)
{
	t_hellish			hellish;
	t_status stat;

	&hellish->repl = &repl;
	(void)argc;
	stat = init_shell(hellish.repl, argv, envp);
	if (!!stat)
		stat = run_sh(&hellish.repl, );
	return (shut_app(cleanse, hellish));
}
