/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_itoa.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ayal-ras <ayal-ras@student.42abudhabi.a    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/17 15:54:02 by ayal-ras          #+#    #+#             */
/*   Updated: 2023/12/24 19:44:01 by ayal-ras         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int	digit_count(int n)
{
	int	len;

	len = 0;
	if (n == 0)
		return (1);
	while (n != '\0')
	{
		n /= 10;
		len++;
	}
	return (len);
}

char	*ft_itoa(int n)
{
	char	*str;
	int		count;
	bool	sign;

	sign = n < 0;
	count = digit_count(n) + sign;
	str = (char *)malloc(sizeof(char) * (count + 1));
	if (str == NULL)
		return (NULL);
	str[count] = 0;
	if (sign)
	{
		*str = '-';
		count--;
		str[count] = -(n % 10) + '0';
		n = -(n / 10);
	}
	while (count > sign)
	{
		count--;
		str[count] = n % 10 + '0';
		n /= 10;
	}
	return (str);
}
