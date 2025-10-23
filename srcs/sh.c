/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sh.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dlesieur <dlesieur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/23 18:43:35 by dlesieur          #+#    #+#             */
/*   Updated: 2025/10/23 18:44:04 by dlesieur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <readline/readline.h>
#include <readline/history.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdarg.h>
#include <stdbool.h>
#include <unistd.h>

/**
 * readline, rl_clear_history, rl_on_new_line,
 * rl_replace_line, rl_redisplay, add-history
 */

 /**
  * access
  * fork
  * wait
  * waitpid
  * wait3
  * wait4
  * execve
   dup
    dup2, pi
  */

  /**
   * signal
   * sigaction
   * kill
   * exit
   * getcwd
   * chdir
   * stat
   * lstat
   * fstat
   * unlink
   * 
   */
int main(int argc, char **argv, char **envp)
{
    bool    run = true;
    char    *line;

    while  (run)
    {
        line = readline("hello >");
        if (line)
            add_history(line);
        else
            continue ;
        printf("process: %s\n", line);
        if (strcmp(line, "exit") == 0)
            run = false;
        if (strcmp(line, "history") == 0)
            rl_redisplay();
    }
    return (0);
}