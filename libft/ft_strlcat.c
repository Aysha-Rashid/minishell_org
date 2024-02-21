/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strlcat.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ayal-ras <ayal-ras@student.42abudhabi.a    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/07 13:27:13 by ayal-ras          #+#    #+#             */
/*   Updated: 2023/12/24 19:45:04 by ayal-ras         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

size_t	ft_strlcat(char *dst, const char *src, size_t dstsize)
{
	unsigned int	d_len;
	unsigned int	j;
	unsigned int	len;
	unsigned int	s_len;

	j = 0;
	if (dstsize == '\0')
		return (ft_strlen(src));
	d_len = ft_strlen(dst);
	s_len = ft_strlen(src);
	len = dstsize - 1;
	if (dstsize > d_len)
	{
		while (src[j] != '\0' && j < (len - d_len))
		{
			dst[d_len + j] = src[j];
			j++;
		}
		dst[d_len + j] = '\0';
		return (s_len + d_len);
	}
	else
		return (s_len + dstsize);
}
