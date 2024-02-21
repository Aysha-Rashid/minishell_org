/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_substr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ayal-ras <ayal-ras@student.42abudhabi.a    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/15 13:25:01 by ayal-ras          #+#    #+#             */
/*   Updated: 2023/12/30 18:43:43 by ayal-ras         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_substr(char	const	*s, unsigned int start, size_t	len)
{
	char	*sub;
	size_t	i;
	char	*src;
	size_t	reslen;

	i = 0;
	if (!s)
		return (NULL);
	src = (char *)s + start;
	if ((size_t)start >= ft_strlen(s))
		return (ft_strdup(""));
	if (len > ft_strlen(src))
		reslen = ft_strlen(src) + 1;
	else
		reslen = len + 1;
	sub = malloc (reslen * sizeof(char));
	if (!sub)
		return (NULL);
	while (s[start + i] != '\0' && i < len)
	{
		sub[i] = s[start + i];
		i++;
	}
	sub[i] = '\0';
	return (sub);
}
