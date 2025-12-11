/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dlesieur <dlesieur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/05 15:27:34 by dlesieur          #+#    #+#             */
/*   Updated: 2025/12/06 18:22:37 by dlesieur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef BUILTINS_H
# define BUILTINS_H

int (*builtin_func(char *name))(t_hellish *state, t_vec argv);
int parse_flags(t_vec argv, int *n, int *e);
int print_args(int e, t_vec argv, size_t i);
void	parse_numeric_escape(char **str);
int	e_parser(char *s);
int builtin_echo(t_state *state, t_vec argv);
int builtin_env(t_state *state, t_vec *argv);
int	builtin_pwd(t_state *state, t_vec_str argv);
void    try_unset(t_state *state, char *key);
int builtin_unset(t_hellish *state, t_vec argv);

# endif