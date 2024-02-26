/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strjoin.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ayal-ras <ayal-ras@student.42abudhabi.a    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/21 16:28:29 by ayal-ras          #+#    #+#             */
/*   Updated: 2024/02/26 15:50:13 by ayal-ras         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include <stddef.h>
#include <string.h>

char	*ft_str_init(void)
{
	char	*str;

	str = (char *)malloc(1 * sizeof(char));
	str[0] = 0;
	return (str);
}

// char	*ft_strjoin(char const *s1, char const *s2)
// {
// 	char	*result;
// 	size_t	i;
// 	size_t	length1;
// 	size_t	length2;

// 	i = 0;
// 	length1 = 0;
// 	length2 = 0;
// 	if (s1)
// 		length1 = ft_strlen(s1);
// 	if (s2)
// 		length1 = ft_strlen(s2);
// 	result = (char *)malloc((length1 + length2 + 1) * sizeof(char));
// 	if (!result)
// 		return (NULL);
// 	if (s1 && s2)
// 	{
// 		while (s1 && *s1)
// 			result[i++] = *s1++;
// 		while (s2 && *s2)
// 			result[i++] = *s2++;
// 	}
// 	result[i] = '\0';
// 	return (result);
// }

// #include <string.h>

char *ft_strjoin(const char *s1, const char *s2) {
    size_t len1 = (s1 != NULL) ? strlen(s1) : 0;
    size_t len2 = (s2 != NULL) ? strlen(s2) : 0;
    char *s3 = (char *)malloc((len1 + len2 + 1) * sizeof(char)); // Allocate memory for both strings and the null terminator
    if (!s3)
        return NULL;

    size_t i = 0;
    for (size_t j = 0; j < len1; ++j)
        s3[i++] = s1[j];
    for (size_t j = 0; j < len2; ++j)
        s3[i++] = s2[j];
    s3[i] = '\0'; // Null-terminate the concatenated string

    return s3;
}