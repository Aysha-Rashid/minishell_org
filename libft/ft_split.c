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

int	count_substrings(char const *s, char c)
{
	int	count;
	int	len;
	int	i;
	int	old_i;

	i = 0;
	len = ft_strlen(s);
	old_i = 0;
	count = 0;
	while (i < len)
	{
		while (i < len && s[i] == c)
			i++;
		old_i = i;
		while (i < len && s[i] != c)
			i++;
		if (i > old_i)
			count++;
	}
	return (count);
}

char	*ft_strncpy(char *dest, const char *src, unsigned int n)
{
	unsigned int	i;

	i = 0;
	while (src[i] != '\0' && i < n)
	{
		dest[i] = src[i];
		i++;
	}
	while (i < n)
	{
		dest [i] = '\0';
		i++;
	}
	return (dest);
}

int	ft_copy_string_index(char **str, const char *s, int i, int start, int index)
{
	int	substr_len;

	substr_len = i - start;
	if (i > start)
	{
		str[index] = (char *)malloc(substr_len + 1);
		if (str[index] != NULL)
		{
			ft_strncpy(str[index], &s[start], substr_len);
			str[index][substr_len] = '\0';
			return (index + 1);
		}
	}
	return (index);
}

void	split_into_substrings(char **str, const char *s, char c)
{
	int		string_index;
	int		start;
	size_t	i;
	int		in_quotes;

	i = 0;
	start = 0;
	in_quotes = 0;
	string_index = 0;
	while (i < ft_strlen(s))
	{
		if (s[i] == '\'' || s[i] == '\"')
			in_quotes = !in_quotes;
		if (!in_quotes && s[i] == c)
		{
			string_index = ft_copy_string_index(str, s, i, start, string_index);
			start = i + 1;
		}
		i++;
	}
	string_index = ft_copy_string_index(str, s, i, start, string_index);
	str[string_index] = NULL;
}

char	**ft_split(char const *s, char c)
{
	char	**str;
	int		count;

	str = NULL;
	if (!s)
		return (NULL);
	count = count_substrings(s, c);
	str = (char **)malloc(sizeof(char *) * (count + 1));
	if (!str)
		return (NULL);
	str[count] = NULL;
	split_into_substrings(str, s, c);
	return (str);
}

// #include <stdio.h>
// int main()
// {
// 	char *str = " hello       'there  is' you and me";
// 	char **result = ft_split(str, ' ');
// 		int i = 0;
// 	if (result)
// 	{
// 		while (result[i] != NULL)
// 		{
// 			printf("%s\n", result[i]);
// 			i++;
// 		}
// 	}
// 	i = 0;
// 	while (result[i] != NULL)
// 		free(result[i++]);
// 	free(result);
// }