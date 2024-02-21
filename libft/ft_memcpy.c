/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memcpy.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ayal-ras <ayal-ras@student.42abudhabi.a    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/10 18:48:18 by ayal-ras          #+#    #+#             */
/*   Updated: 2023/12/24 19:44:25 by ayal-ras         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_memcpy(void *dst, const void *src, size_t len)
{
	size_t			i;
	char			*dst1;
	const char		*src1;

	dst1 = (char *)dst;
	src1 = (const char *)src;
	if (len == 0 || dst1 == src1)
		return (dst1);
	i = 0;
	while (len > i)
	{
		dst1[i] = src1[i];
		i++;
	}
	return (dst);
}
