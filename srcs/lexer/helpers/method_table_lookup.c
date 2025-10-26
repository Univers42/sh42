/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   method_table_lookup.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dlesieur <dlesieur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/26 13:25:26 by syzygy            #+#    #+#             */
/*   Updated: 2025/10/26 15:35:47 by dlesieur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "lexer.h"

__attribute__((weak))
t_token_type
identifier_type(t_scanner *scan)
{
    int length;
    char first;

    length = (int)(scan->current - scan->start);
    if (length < 2 || length > 6)
        return TOKEN_IDENTIFIER;
    first = scan->start[0];
    switch (first * 10 + length)
    {
    case 'a' * 10 + 3: // "and"
        if (ft_memcmp(scan->start, "and", 3) == 0)
            return TOKEN_AND;
        break;
    case 'c' * 10 + 5: // "class"
        if (ft_memcmp(scan->start, "class", 5) == 0)
            return TOKEN_CLASS;
        break;
    case 'e' * 10 + 4: // "else"
        if (ft_memcmp(scan->start, "else", 4) == 0)
            return TOKEN_ELSE;
        break;
    case 'f' * 10 + 3: // "for" or "fun"
        if (ft_memcmp(scan->start, "for", 3) == 0)
            return TOKEN_FOR;
        if (ft_memcmp(scan->start, "fun", 3) == 0)
            return TOKEN_FUN;
        break;
    case 'f' * 10 + 5: // "false"
        if (ft_memcmp(scan->start, "false", 5) == 0)
            return TOKEN_FALSE;
        break;
    case 'i' * 10 + 2: // "if"
        if (ft_memcmp(scan->start, "if", 2) == 0)
            return TOKEN_IF;
        break;
    case 'n' * 10 + 3: // "nil"
        if (ft_memcmp(scan->start, "nil", 3) == 0)
            return TOKEN_NIL;
        break;
    case 'o' * 10 + 2: // "or"
        if (ft_memcmp(scan->start, "or", 2) == 0)
            return TOKEN_OR;
        break;
    case 'p' * 10 + 5: // "print"
        if (ft_memcmp(scan->start, "print", 5) == 0)
            return TOKEN_PRINT;
        break;
    case 'r' * 10 + 6: // "return"
        if (ft_memcmp(scan->start, "return", 6) == 0)
            return TOKEN_RETURN;
        break;
    case 's' * 10 + 5: // "super"
        if (ft_memcmp(scan->start, "super", 5) == 0)
            return TOKEN_SUPER;
        break;
    case 't' * 10 + 4: // "this" or "true"
        if (ft_memcmp(scan->start, "this", 4) == 0)
            return TOKEN_THIS;
        if (ft_memcmp(scan->start, "true", 4) == 0)
            return TOKEN_TRUE;
        break;
    case 'v' * 10 + 3: // "var"
        if (ft_memcmp(scan->start, "var", 3) == 0)
            return TOKEN_VAR;
        break;
    case 'w' * 10 + 5: // "while"
        if (ft_memcmp(scan->start, "while", 5) == 0)
            return TOKEN_WHILE;
        break;
    }
    return TOKEN_IDENTIFIER;
}