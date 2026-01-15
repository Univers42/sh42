# include "libft.h"

/*
** Check if character is valid as first char of variable name
** POSIX: must be letter or underscore
*/
bool	is_var_name_p1(char c)
{
	return (ft_isalpha(c) || c == '_');
}

/*
** Check if character is valid in variable name (after first char)
** POSIX: must be letter, digit, or underscore
*/
bool	is_var_name_p2(char c)
{
	return (ft_isalnum(c) || c == '_');
}
