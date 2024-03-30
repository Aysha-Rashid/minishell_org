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

char	*remove_all_qoutes(char *str)
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

int	check_pipe_and_redir_quote(char *str, int i, int j, char *result)
{
	if (str[i] == '\'' || str[i] == '"')
	{
		if (str[i + 1] == '|')
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

int	contains_special_chars(const char str)
{
	if (str == '<' || str == '>' || str == '|')
		return (1);
	return (0);
}

int	parse_com(char *cmd)
{
	int		i;
	char	*message;
	int		len;

	message = "syntax error near unexpected token ";
	len = ft_strlen(cmd);
	i = 0;
	while (cmd[i])
	{
		if (cmd[i] == ' ' && cmd[i] == '\t')
			i++;
		if (cmd[i] == '|' && (cmd[i - 1] == '\0' || cmd[i + 1] == '\0'))
			return (name_error(NULL, message, "`|'", 0), 1);
		if (contains_special_chars(cmd[i])
			&& contains_special_chars(cmd[i + 2]))
			return (printf("minishell : %s", message),
				printf("`%c'\n", cmd[i + 2]), 1);
		if (cmd[i + 1] == '\0' && (cmd[len - 1] == '>'
				|| cmd[len - 1] == '<' || cmd[len - 1] == '|'))
			return (name_error(NULL, message, "`newline'", 0), 1);
		i++;
	}
	return (0);
}
