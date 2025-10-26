/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_putnbr_fd.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dlesieur <dlesieur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/17 16:55:26 by dlesieur          #+#    #+#             */
/*   Updated: 2025/09/08 21:30:57 by dlesieur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
//#include "TDD/test.h"
/**
 * Writes the integer `nb` to the given file descriptor `fd`.
 * 
 * @param nb The integer to write.
 * @param fd The file descriptor to write to.
 * @return The number of characters written.
 * 
 * @note 
 * This function converts the integer `nb` to a string 
 * representation and writes it   
 * to the specified file descriptor. Negative numbers 
 * are handled by writing the    
 * minus sign first, followed by the absolute value.
 */

void	ft_putnbr_fd(int nb, int fd)
{
	char	digit;
	int		size;

	if (nb == -2147483648)
	{
		size = write(fd, "-2147483648", 11);
		return ;
	}
	if (nb < 0)
	{
		size = write(fd, "-", 1);
		nb = -nb;
	}
	if (nb > 9)
		ft_putnbr_fd(nb / 10, fd);
	digit = (nb % 10) + '0';
	size = write(fd, &digit, 1);
	(void)size;
}
//int main(void)
//{
//	test_putnbr_fd();
//	return 0;
//}
