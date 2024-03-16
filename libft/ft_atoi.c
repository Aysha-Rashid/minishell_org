/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_atoi.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ayal-ras <ayal-ras@student.42abudhabi.a    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/04 15:54:46 by ayal-ras          #+#    #+#             */
/*   Updated: 2024/03/14 14:14:47 by ayal-ras         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static long	overflow(long max, long r, int sign)
{
	if (r >= (max / 10))
	{
		if (sign == 1)
			return (-1);
		if (sign == -1)
			return (0);
	}
	return (r);
}

long	ft_atoi(const char	*str)
{
	long	r;
	int		sign;
	int		i;
	long	max;

	max = 9223372036854775807;
	i = 0;
	sign = 1;
	r = 0;
	while ((str[i] >= 9 && str[i] <= 13) || str[i] == 32)
		i++;
	if (str[i] == '-')
	{
		sign = -1;
		i++;
	}
	else if (str[i] == '+')
		i++;
	while (str[i] >= '0' && str[i] <= '9')
	{
		overflow(max, r, sign);
		r = (r * 10) + str[i++] - '0';
	}
	return (r * sign);
}
