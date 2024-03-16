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

static int	count_substrings(char const *s, char c)
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

static void	split_into_substrings(char	**str, char const	*s, char c)
{
	int	string_index;
	int	start;
	int	i;
	int	len;

	i = 0;
	start = 0;
	string_index = 0;
	len = ft_strlen(s);
	while (i < len)
	{
		while (i < len && s[i] == c)
			i++;
		start = i;
		while (i < len && s[i] != c)
			i++;
		if (i > start)
		{
			str[string_index] = malloc(i - start + 1);
			ft_strncpy(str[string_index], &s[start], i - start);
			str[string_index][i - start] = '\0';
			string_index++;
		}
	}
}

char	**ft_split(char const *s, char c)
{
	char	**str;
	int		count;
	int		i;

	if (!s)
		return (NULL);
	count = count_substrings(s, c);
	str = (char **)malloc(sizeof(char *) * (count + 1));
	if (!str)
		return (NULL);
	str[count] = NULL;
	i = count;
	split_into_substrings(str, s, c);
	while (i > 0)
	{
		if (str[i - 1] == NULL)
		{
			free(str[i - 1]);
			free(str);
			return (NULL);
		}
		i--;
	}
	return (str);
}
