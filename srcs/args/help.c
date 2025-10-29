/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   help.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dlesieur <dlesieur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/29 20:13:01 by dlesieur          #+#    #+#             */
/*   Updated: 2025/10/29 20:35:45 by dlesieur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

void	arg_help(void)
{
	printf("%s\t\t%s\n%s\t\t%s\n%s\t\t%s\n", 
	"--debug",
	"Enables internal shell debugging features, for dev",
	"--help",
	"Show this message",
	"--login",
	"Makes bash act as a login shell, meaning it reads `/etc/profile`",
	"--posix",
	"force shell to conform strictly to the POSIX standard, altering behavior"
	"to be less bash specific",
	"--pretty-print", "used with the parser to output the parse tree in a"
	"readable format");
}