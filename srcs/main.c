/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dlesieur <dlesieur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/04 13:49:41 by dlesieur          #+#    #+#             */
/*   Updated: 2025/12/11 14:55:58 by dlesieur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
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
