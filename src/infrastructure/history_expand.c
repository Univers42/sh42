/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   history_expand.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marvin <marvin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/15 12:00:00 by marvin            #+#    #+#             */
/*   Updated: 2026/01/15 12:00:00 by marvin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "shell.h"
#include "helpers.h"

/*
** Get history entry by positive index (1-based).
** Returns NULL if out of range.
*/
static char	*get_hist_by_number(t_shell *state, int num)
{
	if (num < 1 || (size_t)num > state->hist.hist_cmds.len)
		return (NULL);
	return (((char **)state->hist.hist_cmds.ctx)[num - 1]);
}

/*
** Get history entry by negative offset (-1 = last, -2 = second to last).
** Returns NULL if out of range.
*/
static char	*get_hist_by_neg_offset(t_shell *state, int neg_offset)
{
	int	idx;

	if (neg_offset >= 0 || state->hist.hist_cmds.len == 0)
		return (NULL);
	idx = (int)state->hist.hist_cmds.len + neg_offset;
	if (idx < 0)
		return (NULL);
	return (((char **)state->hist.hist_cmds.ctx)[idx]);
}

/*
** Get history entry that starts with the given prefix.
** Searches from most recent backwards.
** Returns NULL if not found.
*/
static char	*get_hist_by_prefix(t_shell *state, const char *prefix, int len)
{
	int		i;
	char	*entry;

	if (!prefix || len <= 0 || state->hist.hist_cmds.len == 0)
		return (NULL);
	i = (int)state->hist.hist_cmds.len - 1;
	while (i >= 0)
	{
		entry = ((char **)state->hist.hist_cmds.ctx)[i];
		if (entry && ft_strncmp(entry, prefix, len) == 0)
			return (entry);
		i--;
	}
	return (NULL);
}

/*
** Check if character at position is inside single quotes.
*/
static int	is_in_single_quotes(const char *s, int pos)
{
	int	i;
	int	in_sq;

	i = 0;
	in_sq = 0;
	while (i < pos)
	{
		if (s[i] == '\'' && !in_sq)
			in_sq = 1;
		else if (s[i] == '\'' && in_sq)
			in_sq = 0;
		i++;
	}
	return (in_sq);
}

/*
** Parse and expand a single history reference starting after '!'.
** Returns the expanded string (must be freed) or NULL on error.
** Updates *consumed to indicate how many chars were consumed after '!'.
*/
static char	*expand_hist_ref(t_shell *state, const char *s, int *consumed)
{
	int		num;
	int		i;
	char	*result;
	char	num_buf[32];

	*consumed = 0;
	if (!s || !s[0])
		return (NULL);
	/* !! - repeat last command */
	if (s[0] == '!')
	{
		*consumed = 1;
		result = get_hist_by_neg_offset(state, -1);
		if (!result)
		{
			ft_eprintf("%s: !!: event not found\n", state->context);
			return (NULL);
		}
		return (ft_strdup(result));
	}
	/* !-N - negative offset */
	if (s[0] == '-' && s[1] && ft_isdigit(s[1]))
	{
		i = 1;
		while (s[i] && ft_isdigit(s[i]) && i < 30)
			i++;
		*consumed = i;
		ft_memcpy(num_buf, s, i);
		num_buf[i] = '\0';
		num = ft_atoi(num_buf);
		result = get_hist_by_neg_offset(state, num);
		if (!result)
		{
			ft_eprintf("%s: !%s: event not found\n", state->context, num_buf);
			return (NULL);
		}
		return (ft_strdup(result));
	}
	/* !N - positive number */
	if (ft_isdigit(s[0]))
	{
		i = 0;
		while (s[i] && ft_isdigit(s[i]) && i < 30)
			i++;
		*consumed = i;
		ft_memcpy(num_buf, s, i);
		num_buf[i] = '\0';
		num = ft_atoi(num_buf);
		result = get_hist_by_number(state, num);
		if (!result)
		{
			ft_eprintf("%s: !%s: event not found\n", state->context, num_buf);
			return (NULL);
		}
		return (ft_strdup(result));
	}
	/* !word - search by prefix */
	if (ft_isalpha(s[0]) || s[0] == '_')
	{
		i = 0;
		while (s[i] && !ft_isspace(s[i]) && s[i] != '\n' && s[i] != '!'
			&& s[i] != '$' && s[i] != '|' && s[i] != '&' && s[i] != ';'
			&& s[i] != '<' && s[i] != '>' && s[i] != '(' && s[i] != ')')
			i++;
		*consumed = i;
		result = get_hist_by_prefix(state, s, i);
		if (!result)
		{
			ft_eprintf("%s: !%.*s: event not found\n", state->context, i, s);
			return (NULL);
		}
		return (ft_strdup(result));
	}
	/* Unknown pattern after ! - not a history expansion */
	return (NULL);
}

/*
** Expand all history references in the input string.
** Returns a new string with expansions applied, or NULL on error.
** If no expansions occurred, returns a copy of the original.
** Sets *expanded to 1 if any expansion occurred.
*/
char	*history_expand_line(t_shell *state, const char *input, int *expanded)
{
	t_string	result;
	int			i;
	int			len;
	int			consumed;
	char		*expansion;

	*expanded = 0;
	if (!input || !state->hist.hist_active || state->hist.hist_cmds.len == 0)
		return (ft_strdup(input ? input : ""));
	vec_init(&result);
	result.elem_size = 1;
	len = ft_strlen(input);
	i = 0;
	while (i < len)
	{
		/* Check for ! that might be history expansion */
		if (input[i] == '!' && i + 1 < len && !is_in_single_quotes(input, i))
		{
			/* Skip escaped ! (backslash before) */
			if (i > 0 && input[i - 1] == '\\')
			{
				/* Remove the backslash and keep the ! */
				if (result.len > 0)
					result.len--;
				vec_push(&result, (void *)&input[i]);
				i++;
				continue ;
			}
			/* Check what follows ! */
			char next = input[i + 1];
			/* Skip ! followed by space or = (not history expansion) */
			if (next == ' ' || next == '\t' || next == '\n' || next == '=')
			{
				vec_push(&result, (void *)&input[i]);
				i++;
				continue ;
			}
			/* Try to expand */
			expansion = expand_hist_ref(state, input + i + 1, &consumed);
			if (expansion)
			{
				vec_push_str(&result, expansion);
				free(expansion);
				i += 1 + consumed;
				*expanded = 1;
				continue ;
			}
			else if (consumed == 0)
			{
				/* Not a valid expansion pattern, keep ! as literal */
				vec_push(&result, (void *)&input[i]);
				i++;
				continue ;
			}
			else
			{
				/* Error occurred (message already printed) */
				free(result.ctx);
				return (NULL);
			}
		}
		vec_push(&result, (void *)&input[i]);
		i++;
	}
	/* Null-terminate */
	if (!vec_ensure_space_n(&result, 1))
	{
		free(result.ctx);
		return (ft_strdup(input));
	}
	((char *)result.ctx)[result.len] = '\0';
	return ((char *)result.ctx);
}
