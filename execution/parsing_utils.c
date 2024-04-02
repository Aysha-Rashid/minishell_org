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

int	check_pipe_and_redir_quote(char *str, int i)
{
	if (str[i] == '\'' || str[i] == '"')
	{
		if (str[i + 1] && (str[i + 1] == '|'
				|| str[i + 1] == '>' || str[i + 1] == '<'))
			return (1);
		if (str[i + 2] && (str[i + 2] == '<'
				|| str[i + 2] == '>'))
			return (1);
	}
	return (0);
}

int	spec_char(const char str)
{
	if (str == '<' || str == '>' || str == '|')
		return (1);
	return (0);
}

char	*check_quotes_and_copy(char *str, int i, int j, char *result)
{
	char	quote;
	int		len;

	len = ft_strlen(str);
	quote = '\0';
	while (i < len && str[i])
	{
		if ((str[i] == '\'' || str[i] == '"') && str[i])
		{
			quote = str[i++];
			while (i < len && str[i] != quote && str[i])
			{
				if (spec_char(str[i]))
					return (NULL);
				result[j++] = str[i++];
			}
			if (i < len)
				i++;
		}
		result[j++] = str[i++];
	}
	result[j] = '\0';
	return (result);
}

char	*remove_quotes(char *str)
{
	int		i;
	int		j;
	char	*result;

	i = 0;
	j = 0;
	result = malloc(ft_strlen(str) + 1);
	if (check_pipe_and_redir_quote(str, i))
	{
		i++;
		result[j] = str[i];
		result[j + 1] = '\0';
		return (result);
	}
	if (!check_quotes_and_copy(str, i, j, result))
		return (free(result), str);
	return (result);
}

int	parse_com(char *cmd)
{
	int		i;
	char	*message;
	int		len;

	len = ft_strlen(cmd);
	message = "syntax error near unexpected token ";
	i = 0;
	while (cmd[i])
	{
		if (cmd[i] == ' ' || cmd[i] == '\t')
			i++;
		if (cmd[i] == '|' && (i == 0 || cmd[i + 1] == '\0'))
			return (name_error(NULL, message, "`|'", 0), 1);
		if (((cmd[i] == '>' && cmd[i + 1] == '>') || (cmd[i] == '<'
					&& cmd[i + 1] == '<')) && (cmd[i + 3]
				&& spec_char(cmd[i + 3])))
			return (name_error(NULL, message, &cmd[i + 3], 0), 1);
		if ((cmd[i] == '>' || cmd[i] == '<') && cmd[i + 1] != '\0'
			&& cmd[i + 2] != '\0' && spec_char(cmd[i + 2]))
			return (name_error(NULL, message, &cmd[i + 2], 0), 1);
		i++;
	}
	if ((cmd[len - 1] == '>' || cmd[len - 1] == '<'))
		return (name_error(NULL, message, "`newline'", 0), 1);
	return (0);
}
