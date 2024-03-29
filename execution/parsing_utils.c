/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ayal-ras <ayal-ras@student.42abudhabi.a    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/29 20:28:57 by ayal-ras          #+#    #+#             */
/*   Updated: 2024/03/29 20:49:32 by ayal-ras         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

char	*remove_all_qoutes(char *str) // to remove the qoutes for all the character even pipes and redirections
{
	int		i;
	int		j;
	char	quote;
	char	*result;
	int		len;

	len = ft_strlen(str);
	i = 0;
	j = 0;
	result = malloc(len + 1);
	if (!result)
		return (NULL);
	while (i < len && str[i] != '$')
	{
		if (str[i] == '\'' || str[i] == '"')
		{
			quote = str[i++];
			while (i < len && str[i] != quote)
				result[j++] = str[i++];
			i++;
		}
		result[j++] = str[i++];
	}
	result[j] = '\0';
	return (result);
}

char	*check_pipe_and_redir_quote(char *str)
{
	int	i;

	i = 0;
	if (((str[i] == '\'' || str[i] == '"')
			&& ((str[i + 1] == '|' || str[i - 1] == '|')
				|| (str[i - 1] == '>' || str[i + 1] == '>')
				|| (str[i + 1] == '<' || str[i - 1] == '<'))))
		return (str);
	return (NULL);
}

char	*remove_quotes(char *str)
{
	int		i;
	int		j;
	char	quote;
	char	*result;
	int		len;

	len = ft_strlen(str);
	i = 0;
	j = 0;
	result = malloc(len + 1);
	while (i < len)
	{
		while (check_pipe_and_redir_quote(str))
			result[j++] = str[i++];
		if ((str[i] == '\'' || str[i] == '"'))
		{
			quote = str[i++];
			while (i < len && str[i] != quote)
				result[j++] = str[i++];
			i++;
		}
		result[j++] = str[i++];
	}
	result[j] = '\0';
	return (result);
}
