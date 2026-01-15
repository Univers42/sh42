/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expander.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marvin <marvin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/09 23:31:37 by marvin            #+#    #+#             */
/*   Updated: 2026/01/09 23:31:37 by marvin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "shell.h"
#include <fcntl.h>
#include <stdbool.h>
#include <stdio.h>
#include "expander.h"
#include "glob.h"
#include "env.h"
#include "helpers.h"
#include <errno.h>
#include <unistd.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <string.h>
#include "arith.h"

char	*expand_word_single(t_shell *state, t_ast_node *curr)
{
	t_vec	args;
	size_t	i;
	char	*temp;

	vec_init(&args);
	args.elem_size = sizeof(char *);
	expand_word(state, curr, &args, false);
	if (args.len == 1)
	{
		temp = ((char **)args.ctx)[0];
		free(args.ctx);
		return (temp);
	}
	if (args.len == 0)
	{
		t_string s = word_to_string(*curr);
		if (!vec_ensure_space_n(&s, 1))
		{
			free(s.ctx);
			return (NULL);
		}
		((char *)s.ctx)[s.len] = '\0';
		return ((char *)s.ctx);
	}
	i = 0;
	while (i < args.len)
	{
		free(((char **)args.ctx)[i]);
		i++;
	}
	free(args.ctx);
	return (0);
}

static bool	is_empty_command(const char *cmd)
{
	if (!cmd)
		return (true);
	while (*cmd)
	{
		if (*cmd != ' ' && *cmd != '\t' && *cmd != '\n')
			return (false);
		cmd++;
	}
	return (true);
}

static char	*capture_subshell_output(t_shell *state, const char *cmd)
{
	int			pipefd[2];
	pid_t		pid;
	t_string	out;
	char		*ret;
	size_t		nlen;

	if (is_empty_command(cmd))
		return (ft_strdup(""));
	if (pipe(pipefd) == -1)
		return (ft_strdup(""));
	pid = fork();
	if (pid == -1)
	{
		close(pipefd[0]);
		close(pipefd[1]);
		return (ft_strdup(""));
	}
	if (pid == 0)
	{
		close(pipefd[0]);
		dup2(pipefd[1], STDOUT_FILENO);
		close(pipefd[1]);
		char *const argv[] = {"/bin/sh", "-c", (char *)cmd, NULL};
		char **envp = get_envp(state, "/bin/sh");
		execve("/bin/sh", argv, envp);
		if (envp)
			free_tab(envp);
		_exit(127);
	}
	close(pipefd[1]);
	vec_init(&out);
	out.elem_size = 1;
	vec_append_fd(pipefd[0], &out);
	close(pipefd[0]);
	{
		int status;
		while (waitpid(pid, &status, 0) == -1 && errno == EINTR)
			;
	}
	ret = malloc(out.len + 1);
	if (!ret)
	{
		free(out.ctx);
		return (ft_strdup(""));
	}
	if (out.len)
		memcpy(ret, out.ctx, out.len);
	ret[out.len] = '\0';
	nlen = out.len;
	while (nlen > 0 && ret[nlen - 1] == '\n')
		nlen--;
	ret[nlen] = '\0';
	free(out.ctx);
	return (ret);
}

static void	handle_arith_result(t_token *tok, t_string *outbuf,
	bool *had_error, bool *changed)
{
	char	*newstr;

	if (*had_error && *changed)
	{
		newstr = ft_strdup("");
		if (tok->allocated && tok->start)
			free((char *)tok->start);
		tok->start = newstr;
		tok->len = 0;
		tok->allocated = true;
		free(outbuf->ctx);
	}
	else if (*had_error)
		free(outbuf->ctx);
	else if (*changed)
	{
		newstr = malloc(outbuf->len + 1);
		if (newstr)
		{
			if (outbuf->len)
				memcpy(newstr, outbuf->ctx, outbuf->len);
			newstr[outbuf->len] = '\0';
			if (tok->allocated && tok->start)
				free((char *)tok->start);
			tok->start = newstr;
			tok->len = outbuf->len;
			tok->allocated = true;
		}
		free(outbuf->ctx);
	}
	else
		free(outbuf->ctx);
}

static void	process_arith_expansion(t_shell *state, const char **s, int *len,
	int *pos, t_string *outbuf, bool *changed, bool *had_error)
{
	int			depth;
	int			j;
	int			expr_start;
	int			expr_len;
	const char	*expr;
	const char	*semi;
	char		*result;

	depth = 2;
	j = *pos + 3;
	while (j < *len && depth > 0)
	{
		if ((*s)[j] == '(' && j + 1 < *len && (*s)[j + 1] == '(')
			{ depth += 2; j += 2; }
		else if ((*s)[j] == ')' && j + 1 < *len && (*s)[j + 1] == ')')
			{ depth -= 2; j += 2; }
		else if ((*s)[j] == '(')
			{ depth++; j++; }
		else if ((*s)[j] == ')')
			{ depth--; j++; }
		else
			j++;
	}
	if (depth != 0)
		return ;
	if (*pos > 0 && outbuf->len == 0)
		vec_push_nstr(outbuf, *s, *pos);
	expr_start = *pos + 3;
	expr_len = j - 2 - expr_start;
	expr = *s + expr_start;
	semi = ft_strnchr((char *)expr, ';', expr_len);
	if (semi)
	{
		int remaining = expr_len - (int)(semi - expr);
		ft_eprintf("%s: $((: invalid arithmetic operator (error token is \"%.*s\")\n",
			state->context, remaining, semi);
		/* Set exit code to 1 for arithmetic syntax errors */
		set_cmd_status(state, res_status(1));
		*had_error = true;
		*changed = true;
		/* Update position so caller knows we processed this */
		*pos = 0;
		*len = *len - j;
		*s = *s + j;
		/* Do NOT set g_should_unwind here - that causes exit code 130 */
		return ;
	}
	result = arith_expand(state, expr, expr_len);
	if (result)
	{
		vec_push_nstr(outbuf, result, ft_strlen(result));
		free(result);
		*changed = true;
	}
	else
	{
		/* Arithmetic evaluation error - status already set by arith_expand */
		*had_error = true;
		*changed = true;
	}
	*pos = 0;
	*len = *len - j;
	*s = *s + j;
}

static void	process_cmd_subst(t_shell *state, const char **s, int *len,
	int *pos, t_string *outbuf, bool *changed)
{
	int		depth;
	int		j;
	int		inner_start;
	int		inlen;
	char	*inner;
	char	*subout;

	depth = 1;
	j = *pos + 2;
	while (j < *len && depth > 0)
	{
		if ((*s)[j] == '(')
			depth++;
		else if ((*s)[j] == ')')
			depth--;
		j++;
	}
	if (depth != 0)
		return ;
	if (*pos > 0 && outbuf->len == 0)
		vec_push_nstr(outbuf, *s, *pos);
	inner_start = *pos + 2;
	inlen = j - 1 - inner_start;
	inner = malloc(inlen + 1);
	if (inner)
	{
		memcpy(inner, *s + inner_start, inlen);
		inner[inlen] = '\0';
		subout = capture_subshell_output(state, inner);
		free(inner);
		if (!subout)
			subout = ft_strdup("");
		if (*subout)
			vec_push_nstr(outbuf, subout, ft_strlen(subout));
		free(subout);
	}
	*changed = true;
	*pos = 0;
	*len = *len - j;
	*s = *s + j - *pos;
}

static void	expand_cmd_substitutions(t_shell *state, t_ast_node *node)
{
	size_t		i;
	t_ast_node	*ch;
	t_token		*tok;
	const char	*s;
	int			len;
	int			pos;
	bool		changed;
	bool		had_error;
	t_string	outbuf;
	char		c;

	if (!node->children.ctx)
		return ;
	i = 0;
	while (i < node->children.len)
	{
		ch = &((t_ast_node *)node->children.ctx)[i];
		if (ch->node_type == AST_TOKEN)
		{
			tok = &ch->token;
			if (tok->tt == TT_WORD)
			{
				s = tok->start;
				len = tok->len;
				pos = 0;
				changed = false;
				had_error = false;
				vec_init(&outbuf);
				outbuf.elem_size = 1;
				while (pos < len && !had_error)
				{
					if (s[pos] == '$' && pos + 2 < len
						&& s[pos + 1] == '(' && s[pos + 2] == '(')
					{
						process_arith_expansion(state, &s, &len, &pos,
							&outbuf, &changed, &had_error);
						if (had_error)
							break ;
						continue ;
					}
					else if (s[pos] == '$' && pos + 1 < len && s[pos + 1] == '(')
					{
						process_cmd_subst(state, &s, &len, &pos,
							&outbuf, &changed);
						continue ;
					}
					c = s[pos++];
					vec_push(&outbuf, &c);
				}
				handle_arith_result(tok, &outbuf, &had_error, &changed);
				/* If there was an arithmetic error, set the marker and return */
				if (had_error && state->last_cmd_status_res.status != 0)
				{
					g_should_unwind = 0xDEAD;
					return ;
				}
			}
		}
		expand_cmd_substitutions(state, &((t_ast_node *)node->children.ctx)[i]);
		if (g_should_unwind)
			return ;
		i++;
	}
}

t_token_old	get_old_token(t_ast_node word)
{
	t_token_old	ret;

	ft_assert(word.node_type == AST_WORD);
	ft_assert(word.children.len);
	ret = ((t_ast_node *)word.children.ctx)[0].token.full_word;
	ft_assert(ret.present);
	return (ret);
}

void	expand_node_glob(t_ast_node *node, t_vec *args, bool keep_as_one)
{
	t_vec		glob_words;
	t_string	temp;
	size_t		j;
	char		ch;

	glob_words = expand_word_glob(*node);
	if (g_should_unwind)
		return ;
	vec_init(&temp);
	j = 0;
	while (j < glob_words.len)
	{
		if (!keep_as_one)
			vec_push(args, &((char **)glob_words.ctx)[j]);
		else
		{
			vec_push_str(&temp, ((char **)glob_words.ctx)[j]);
			free(((char **)glob_words.ctx)[j]);
		}
		if (j++ + 1 < glob_words.len && keep_as_one)
		{
			ch = ' ';
			vec_push(&temp, &ch);
		}
	}
	if (keep_as_one)
		vec_push(args, &temp.ctx);
	free(glob_words.ctx);
	free_ast(node);
}

void	expand_word(t_shell *state, t_ast_node *node,
				t_vec *args, bool keep_as_one)
{
	t_vec_nd	words;
	size_t		i;

	expand_tilde_word(state, node);
	expand_cmd_substitutions(state, node);
	expand_env_vars(state, node);
	vec_init(&words);
	words.elem_size = sizeof(t_ast_node);
	if (!keep_as_one)
		words = split_words(state, node);
	else
	{
		vec_push(&words, node);
		*node = (t_ast_node){};
	}
	i = 0;
	while (i < words.len)
	{
		expand_node_glob(&((t_ast_node *)words.ctx)[i], args, keep_as_one);
		if (g_should_unwind)
			while (i < words.len)
				free_ast(&((t_ast_node *)words.ctx)[i++]);
		if (g_should_unwind)
			break ;
		i++;
	}
	free(words.ctx);
	free_ast(node);
}
