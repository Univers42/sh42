/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   api_repl.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alcacere <alcacere@student.42madrid.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/12 13:32:07 by alcacere          #+#    #+#             */
/*   Updated: 2025/12/12 13:32:10 by alcacere         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "repl_public.h"
#include "ft_stdio.h"
#include "ipc.h"
#include "var.h"
#include "lexer.h"

static void	free_all_state(t_stream_dft_data *shell)
{
	free(shell->input.buff);
	ft_bzero(&shell->input, sizeof(t_dyn_str));
	free(shell->last_cmd_status_s);
	free(shell->pid);
	free(shell->context);
	free(shell->base_context);
	shell->context = 0;
	shell->base_context = 0;
	free(shell->rl.str.buff);
	free_hist(&shell->hist);
	free(shell->cwd.buff);
}

static void	init_repl(t_stream_dft_data *meta, char **argv, char **envp,
		t_repl_config *conf)
{
	t_fnctx	ctx;

	ctx.fn = (void (*)(void *))free_all_state;
	ctx.arg = meta;
	(void)ctx;
	(void)conf;
	set_unwind_sig();
	*meta = (t_stream_dft_data){0};
	meta->pid = getpid_hack();
	meta->context = ft_strdup(argv[0]);
	meta->base_context = ft_strdup(argv[0]);
	set_cmd_status(&meta->last_cmd_status_res, res_status(0),
		&meta->last_cmd_status_s);
	meta->last_cmd_status_res = res_status(0);
	init_cwd(&meta->cwd);
	meta->env = env_to_vec_env(&meta->cwd, envp);
	init_history(&meta->hist, &meta->env);
}

static t_tokenizer_ctx	init_tokenizer_context(t_stream_dft_data *meta,
		char **prompt, t_deque *tt)
{
	t_tokenizer_ctx	ctx;

	ctx.rl = &meta->rl;
	ctx.prompt = prompt;
	ctx.input = &meta->input;
	ctx.last_cmd_status_res = &meta->last_cmd_status_res;
	ctx.last_cmd_status_s = &meta->last_cmd_status_s;
	ctx.input_method = &meta->input_method;
	ctx.context = &meta->context;
	ctx.base_context = &meta->base_context;
	ctx.should_exit = &meta->should_exit;
	ctx.out_tokens = tt;
	ctx.looking_for = '\0';
	return (ctx);
}

static void	parse_input(t_stream_dft_data *meta)
{
	t_parse				parser;
	t_deque				tt;
	t_tokenizer_ctx		ctx;
	char				*prompt;

	ft_bzero(&parser, sizeof(t_parse));
	parser.st = ST_INIT;
	prompt = prompt_normal(&meta->last_cmd_status_res,
			&meta->last_cmd_status_s).buff;
	deque_init(&tt, 64, sizeof(t_token), NULL);
	ctx = init_tokenizer_context(meta, &prompt, &tt);
	get_more_tokens(&ctx);
	if (get_g_sig()->should_unwind)
		set_cmd_status(&meta->last_cmd_status_res,
			(t_status){.status = CANCELED, .pid = -1, .c_c = true},
			&meta->last_cmd_status_s);
	free(parser.stack.buff);
	ft_bzero(&parser.stack, sizeof(t_vec));
	if (tt.cap && tt.buf)
		free(tt.buf);
	if ((get_g_sig()->should_unwind && meta->input_method != INP_READLINE)
		|| meta->rl.has_finished)
		meta->should_exit = true;
}

void	repl(t_repl_config *conf, char **argv, char **envp)
{
	t_stream_dft_data	meta;

	init_repl(&meta, argv, envp, conf);
	while (meta.should_exit)
	{
		dyn_str_init(&meta.input);
		get_g_sig()->should_unwind = 0;
		parse_input(&meta);
		if (meta.rl.cursor > 1)
			manage_history(&meta.hist, &meta.rl);
		buff_readline_reset(&meta.rl);
		free(meta.input.buff);
		ft_bzero(&meta.input, sizeof(t_dyn_str));
	}
	free_env(&meta.env);
	free_all_state(&meta);
	forward_exit_status(meta.last_cmd_status_res);
}
