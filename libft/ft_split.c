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

char	check_quote(char s, char quote)
{
	if (s == quote)
		return (0);
	else
		return (s);
}

static int	count_substrings(const char *s, char c)
{
	int		count;
	int		i;
	char	quote;

	i = 0;
	quote = 0;
	count = 0;
	while (s[i])
	{
		while (s[i] == c && quote == 0)
			i++;
		while (s[i] && (s[i] != c || quote != 0))
		{
			count++;
			if ((s[i] == '\'' || s[i] == '\"') && (i == 0 || s[i - 1] != '\\'))
				quote = check_quote(s[i], quote);
			i++;
		}
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

static void	split_into_substrings(char **str, const char *s, char c)
{
	int		i;
	int		start;
	int		string_index;
	char	quote;

	start = 0;
	string_index = 0;
	i = 0;
	quote = 0;
	while (s[i])
	{
		while (s[i] == c && quote == 0)
			i++;
		start = i;
		while (s[i] && (s[i] != c || quote != 0))
		{
			if ((s[i] == '\'' || s[i] == '\"') && (i == 0 || s[i - 1] != '\\'))
				quote = check_quote(s[i], quote);
			i++;
		}
		if (i > start)
		{
			str[string_index] = ft_strndup(&s[start], i - start);
			string_index++;
		}
	}
	str[string_index] = NULL;
}

char	**ft_split(char const *s, char c)
{
	char	**str;
	int		count;

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
//     char *str;
//     str = "'     hello'";
//     char **split;
//     split = ft_split(str, ' ');
//     int i =0;
//     while(split[i])
//     {
//         printf("%s\n", split[i]);
//         i++;
//     }
//     i = 0;
//     while (split[i])
//         free(split[i++]);
//     free(split);
// }

// // //     while(split[i])
//         free(split[i++]);
//     free(split);
// // }