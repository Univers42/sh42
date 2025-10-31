/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   config.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dlesieur <dlesieur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/31 16:23:28 by dlesieur          #+#    #+#             */
/*   Updated: 2025/10/31 16:43:09 by dlesieur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/* config.h -- Configuration file for bash. */

#ifndef CONFIG_H
#define CONFIG_H

/* Template settings for autoconf */

/* Configuration feature settings controllable by autoconf. */

/* Define JOB_CONTROL if your operating system supports
   BSD-like job control. */
#undef JOB_CONTROL

/* Define ALIAS if you want the alias features. */
#undef ALIAS

/* Define BRACE_EXPANSION if you want curly brace expansion a la Csh:
   foo{a,b} -> fooa foob.  Even if this is compiled in (the default) you
   can turn it off at shell startup with `-nobraceexpansion', or during
   shell execution with `set +o braceexpand'. */
#undef BRACE_EXPANSION

/* Define READLINE to get the nifty/glitzy editing features.
   This is on by default.  You can turn it off interactively
   with the -nolineediting flag. */
#undef READLINE

/* Define BANG_HISTORY if you want to have Csh style "!" history expansion.
   This is unrelated to READLINE. */
#undef BANG_HISTORY

/* Define HISTORY if you want to have access to previously typed commands.

   If both HISTORY and READLINE are defined, you can get at the commands
   with line editing commands, and you can directly manipulate the history
   from the command line.

   If only HISTORY is defined, the `fc' and `history' builtins are
   available. */
#undef HISTORY

/* Define HELP_BUILTIN if you want the `help' shell builtin and the long
   documentation strings compiled into the shell. */
#undef HELP_BUILTIN

/* Define RESTRICTED_SHELL if you want the generated shell to have the
   ability to be a restricted one.  The shell thus generated can become
   restricted by being run with the name "rbash", or by setting the -r
   flag. */
#undef RESTRICTED_SHELL

/* Define DISABLED_BUILTINS if you want "builtin foo" to always run the
   shell builtin "foo", even if it has been disabled with "enable -n foo". */
#undef DISABLED_BUILTINS

/* Define PROCESS_SUBSTITUTION if you want the K*rn shell-like process
   substitution features "<(file)". */
/* Right now, you cannot do this on machines without fully operational
   FIFO support.  This currently include NeXT and Alliant. */
#undef PROCESS_SUBSTITUTION


/* Define SELECT_COMMAND if you want the Korn-shell style `select' command:
	select word in word_list; do command_list; done */
#undef SELECT_COMMAND

/* Define COMMAND_TIMING of you want the ksh-style `time' reserved word and
   the ability to time pipelines, functions, and builtins. */
#undef COMMAND_TIMING

/* Define ARRAY_VARS if you want ksh-style one-dimensional array variables. */
#undef ARRAY_VARS

/* Define DPAREN_ARITHMETIC if you want the ksh-style ((...)) arithmetic
   evaluation command. */
#undef DPAREN_ARITHMETIC

/* Define EXTENDED_GLOB if you want the ksh-style [*+@?!](patlist) extended
   pattern matching. */
#undef EXTENDED_GLOB

/* Define EXTGLOB_DEFAULT to the value you'd like the extglob shell option
   to have by default */
#undef EXTGLOB_DEFAULT

/* Define COND_COMMAND if you want the ksh-style [[...]] conditional
   command. */
#undef COND_COMMAND

/* Define ARITH_FOR_COMMAND if you want the ksh93-style
	for (( init; test; step )) do list; done
   arithmetic for command. */
#undef ARITH_FOR_COMMAND

/* Define PROGRAMMABLE_COMPLETION for the programmable completion features
   and the complete builtin. */
#undef PROGRAMMABLE_COMPLETION

/* Define DEBUGGER if you want to compile in some features used only by the 
   bash debugger. */
#undef DEBUGGER

/* Define STRICT_POSIX if you want bash to be strictly posix.2 conformant by
   default (except for echo; that is controlled separately). */
#undef STRICT_POSIX

/* Define MEMSCRAMBLE if you want the bash malloc and free to scramble
   memory contents on malloc() and free(). */
#undef MEMSCRAMBLE

/* Define to make the `direxpand' shopt option enabled by default. */
#undef DIRCOMPLETE_EXPAND_DEFAULT

/* Define to make the `globasciiranges' shopt option enabled by default. */
#undef GLOBASCII_DEFAULT

#endif
