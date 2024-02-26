/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strdup.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ayal-ras <ayal-ras@student.42abudhabi.a    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/13 18:46:36 by ayal-ras          #+#    #+#             */
/*   Updated: 2024/02/26 12:30:40 by ayal-ras         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strdup(const char	*s1)
{
	size_t	len;
	int		i;
	char	*ptr;

	len = ft_strlen(s1);
	i = 0;
	ptr = (char *) malloc(sizeof(*ptr) * (((sizeof(size_t) * len) + 1)));
	if (!ptr)
		return (NULL);
	if (ptr)
	{
		while (*s1)
			ptr[i++] = *s1++;
		ptr[i] = '\0';
	}
	return (ptr);
}
