/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_split.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ayal-ras <ayal-ras@student.42abudhabi.a    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/26 15:44:15 by ayal-ras          #+#    #+#             */
/*   Updated: 2024/02/18 10:43:53 by ayal-ras         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static int count_substrings(const char *s, char c) {
    int count = 0;
    char in_quotes = 0;

    for (int i = 0; s[i]; i++) {
        if (s[i] == '\'' || s[i] == '\"') {
            if (!in_quotes)
                in_quotes = s[i];
            else if (in_quotes == s[i])
                in_quotes = 0;
        } else if (!in_quotes && s[i] == c && s[i + 1] != c && s[i + 1] != '\0') {
            count++;
        }
    }

    return count + 1; // Always count at least one substring
}

char	*ft_strncpy(char *dest, const char *src, unsigned int n)
{
	unsigned int	i;

	i = 0;
	while (i < n && src[i])
	{
		dest[i] = src[i];
		i++;
	}
	dest[i] = '\0';
	return (dest);
}

char **split_into_substrings(char **str, const char *s, char c) {
    size_t start = 0, index = 0;
    char in_quotes = 0;

    for (size_t i = 0; s[i]; i++) {
        if (s[i] == '\'' || s[i] == '\"') {
            if (!in_quotes)
                in_quotes = s[i]; // Begin quote
            else if (in_quotes == s[i])
                in_quotes = 0; // End quote
        }
        
        // Check for delimiter or end of string
        if (!in_quotes && (s[i] == c || s[i + 1] == '\0')) {
            if (i >= start) {
                str[index++] = ft_strndup(s + start, s[i + 1] == '\0' ? i - start + 1 : i - start);
            }
            start = i + 1;
        }
    }

    // Handle case where the last part of the string does not end in a delimiter
    if (start < ft_strlen(s)) {
        str[index++] = ft_strndup(s + start, ft_strlen(s) - start);
    }

    str[index] = NULL;
    return str;
}


char	**ft_split(char const *s, char c)
{
	int		count;
	char	**str;

	if (!s)
		return (NULL);
	count = count_substrings(s, c);
	str = (char **)malloc(sizeof(char *) * (count + 1));
	if (!str)
		return (NULL);
	str = split_into_substrings(str, s, c);
	return (str);
}

// #include <stdio.h>
// int main()
// {
// 	char str[] = "hi";
// 	char **split = ft_split(str, ' ');
// 	int i = 0;
// 	while (split[i] != NULL)
// 	{
// 		printf("%s\n", split[i]);
//         i++;
// 	}
// }