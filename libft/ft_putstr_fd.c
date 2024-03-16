/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_putstr_fd.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ayal-ras <ayal-ras@student.42abudhabi.a    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/16 13:28:42 by ayal-ras          #+#    #+#             */
/*   Updated: 2024/03/03 17:33:44 by ayal-ras         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	ft_putstr_fd(char	*s, int fd)
{
	if (!s)
	{
		return ;
	}
	write (fd, s, ft_strlen(s));
}

void	ft_putstrn_fd(const char *str, size_t n, int fd)
{
	size_t	i;

	i = 0;
	while (str[i] && n > i)
	{
		write (fd, &str[i], 1);
		i++;
	}
	return ;
}
