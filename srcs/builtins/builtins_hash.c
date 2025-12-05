/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins_hash.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dlesieur <dlesieur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/05 15:26:36 by dlesieur          #+#    #+#             */
/*   Updated: 2025/12/05 17:17:07 by dlesieur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "ft_hash.h"

#ifndef HAS_BUILTIN_MAP

int (*builtin_func(char *name))(t_hellish *state, t_vec argv)
{
    if (!name)
        return (0);
    else if (!ft_strcmp(name, "echo"))
        return (builtin_echo);
    else if (!ft_strcmp(name, "export"))
        return (builtin_export);
    else if (!ft_strcmp(name, "cd"))
        return (builtin_cd);
    else if (!ft_strcmp(name, "pwd"))
        return (builtin_pwd);
    else if (!ft_strcmp(name, "exit"))
        return (builtin_exit);
    else if (!ft_strcmp(name, "env"))
        return (builtin_env);
    else if (!ft_strcmp(name, "unset"))
        return (builtin_unset);
    return (0);
}

#else

static t_hash *builtin_table_init(void)
{
    static t_hash   table;
    static int      initialized = 0;
    t_hash_config   config;

    if (initialized)
        return (&table);
    config.size = 16;
    config.hash_fn = hash_string;
    config.cmp_fn = cmp_string;
    config.key_free_fn = NULL;
    config.val_free_fn = NULL;
    if (hash_create(&table, &config) < 0)
        return (NULL);
    hash_set(&table, "echo", (void *)builtin_echo);
    hash_set(&table, "export", (void *)builtin_export);
    hash_set(&table, "cd", (void *)builtin_cd);
    hash_set(&table, "pwd", (void *)builtin_pwd);
    hash_set(&table, "exit", (void *)builtin_exit);
    hash_set(&table, "env", (void *)builtin_env);
    hash_set(&table, "unset", (void *)builtin_unset);
    initialized = 1;
    return (&table);
}

int (*builtin_func(char *name))(t_hellish *state, t_vec argv)
{
    t_hash *table;

    if (!name)
        return (0);
    table = builtin_table_init();
    if (!table)
        return (0);
    return ((int (*)(t_hellish *, t_vec))hash_get(table, name));
}

#endif