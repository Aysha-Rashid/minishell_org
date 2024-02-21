/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memcmp.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ayal-ras <ayal-ras@student.42abudhabi.a    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/12 16:05:43 by ayal-ras          #+#    #+#             */
/*   Updated: 2023/12/24 19:44:21 by ayal-ras         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int	ft_memcmp(const void	*s1, const void	*s2, size_t n)
{
	unsigned int		i;
	const unsigned char	*str1;
	const unsigned char	*str2;

	str1 = (const unsigned char *)s1;
	str2 = (const unsigned char *)s2;
	i = 0;
	while ((n > i))
	{
		if ((const unsigned char)str1[i] > (const unsigned char) str2[i])
		{
			return (str1[i] - str2[i]);
		}
		else if ((const unsigned char)str1[i] < (const unsigned char)str2[i])
		{
			return (str1[i] - str2[i]);
		}
		i++;
	}
	return (0);
}
