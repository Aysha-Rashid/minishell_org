/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strjoin.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ayal-ras <ayal-ras@student.42abudhabi.a    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/21 16:28:29 by ayal-ras          #+#    #+#             */
/*   Updated: 2024/03/02 15:05:48 by ayal-ras         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strjoin(char const *s1, char const *s2)
{
	char	*result;
	size_t	i;
	size_t	j;
	size_t	length1;
	size_t	length2;

	i = 0;
	length1 = 0;
	length2 = 0;
	result = NULL;
	if (s1)
		length1 = ft_strlen(s1);
	if (s2)
		length2 = ft_strlen(s2);
	result = (char *)malloc((length1 + length2 + 1) * sizeof(char));
	if (!result)
		return (NULL);
	j = 0;
	while (j < length1)
		result[i++] = s1[j++];
	j = 0;
	while (j < length2)
		result[i++] = s2[j++];
	result[i] = '\0';
	return (result);
}
