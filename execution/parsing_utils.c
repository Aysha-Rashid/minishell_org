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
			if (i < len)
				i++;
		}
		result[j++] = str[i++];
	}
	result[j] = '\0';
	return (result);
}

// int	check_pipe_and_redir_quote(char *str)
// {

//     int i = 0;
//     char quote = '\0';

//     while (str[i] != '\0')
//     {
//         if ((str[i] == '\'' || str[i] == '"') && !quote)
//             quote = str[i];
//         else if (quote && str[i] == quote)
//             quote = '\0';
//         else if (!quote && (str[i] == '|' || str[i] == '<' || str[i] == '>'))
//             return 1;
//         i++;
//     }
//     return 0;
// }
int	check_pipe_and_redir_quote(char *str, int i, int j, char *result)
{
	if ((str[i] == '\'' || str[i] == '"'))
	{
		if(str[i + 1] == '|')
			result[j++] = str[i++];
		result[j++] = str[i++];
	}
	return (0);
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
		if (str[i] == '\'' || str[i] == '"')
		{
			result[j++] = str[i++];
			if (str[i + 1] == '|' || str[i + 1] == '<' || str[i + 1] == '>')
				result[j++] = str[i++];
		}
		if (str[i] == '\'' || str[i] == '"')
		{
			quote = str[i++];
			while (i < len && str[i] != quote)
				result[j++] = str[i++];
			if (i < len)
				i++;
		}
		result[j++] = str[i++];
	}
	result[j] = '\0';
	return (result);
}
