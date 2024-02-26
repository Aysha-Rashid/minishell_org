/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strndup.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ayal-ras <ayal-ras@student.42abudhabi.a    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/22 13:09:30 by ayal-ras          #+#    #+#             */
/*   Updated: 2024/02/23 12:42:11 by ayal-ras         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strndup(const char *src, size_t n)
{
	size_t		len;
	char		*dst;
	const char	*ptr;
	size_t		i;

	ptr = src;
	len = 0;
	i = 0;
	while (len < n && *ptr != '\0')
	{
		len++;
		ptr++;
	}
	dst = (char *)malloc(sizeof(char) * (len + 1));
	if (dst == NULL)
		return (NULL);
	ptr = src;
	while (i < len)
	{
		dst[i] = ptr[i];
		i++;
	}
	dst[len] = '\0';
	return (dst);
}
