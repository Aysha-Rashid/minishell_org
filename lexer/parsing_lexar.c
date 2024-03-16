/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing_lexar.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ayal-ras <ayal-ras@student.42abudhabi.a    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/06 21:00:41 by ayal-ras          #+#    #+#             */
/*   Updated: 2024/03/14 14:56:18 by ayal-ras         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int parsing_lexar(t_data *data, t_lexer *lexar)
{
	char	**str;
	size_t	len;

	str = ft_split(data->cmd, ' ');
	len = size_of_env(str);
	if (len == 1 && (ft_strncmp(str[0], "<<", 2) == 0))
    	return (double_token_error("newline"), free_array(str), 1);
	if (ft_strcmp(str[0], "|") == 0)
		return (double_token_error(str[0]), free_array(str), 1);
	if (len >= 1 && ((ft_strcmp(str[0], "|") == 0 && ft_strlen(str[0]) != 1)
			// || (ft_strcmp(str[0], "<<") == 0)
			|| (ft_strcmp(str[0], "||") == 0)))
		return (double_token_error(str[0]), free_array(str), 1);
	if ((ft_strcmp(str[0], "<") == 0) && len >= 1)
	{
		if (str[1] == NULL)
		{
			if (double_token_error("<"))
				return (free_array(str), 1);
		}
		else
		{
			if (double_token_error(str[1]))
				return (free_array(str),1);
			else
			{
				data->executor->in = open(str[1], O_RDONLY);
				if (data->executor->in == -1)
					return (ft_error(2, str[1], 1), free_array(str), 1);
			}
		}
	}
	else if ((ft_strcmp(str[len - 2], ">") == 0 && len >= 1) || (ft_strcmp(str[len - 2], ">>") == 0 && len >= 2))
	{
		if (str[len - 1] != NULL)
		{
			if (double_token_error(str[len - 1]))
				return (free_array(str), 1);
			else
			{
				if (double_token_error(str[1]))
					return (free_array(str),1);
				else
				{
					int flags = O_CREAT | O_RDWR | (lexar->token == OUTEOF ? O_APPEND : O_TRUNC);
					data->executor->out = open(str[len - 1], flags, 0644);
					if (data->executor->out == -1)
						return (ft_error(4, str[len - 1], 0), free_array(str), 1);
				}
			}
		}
	}
	return (free_array(str), 0);
}

int	double_token_error(char *str)
{
	char	*error;

	error = "";
	if (ft_strcmp(str, "<") == 0 || ft_strcmp(str, ">") == 0
		|| ft_strcmp(str, "<<") == 0 || ft_strcmp(str, "|") == 0
		|| ft_strcmp(str, "||") == 0 || ft_strcmp(str, ">>") == 0
		|| ft_strcmp(str, "newline") == 0)
	{
		if (ft_strcmp(str, "<") == 0)
			error = "`<'";
		else if (ft_strcmp(str, ">") == 0)
			error = "`>'";
		else if (ft_strcmp(str, "<<") == 0)
			error = "`<<'";
		else if (ft_strcmp(str, ">>") == 0)
			error = "`>>'";
		else if (ft_strcmp(str, "|") == 0)
			error = "`|'";
		else if (ft_strcmp(str, "||") == 0)
			error = "`||'";
		else if (ft_strcmp(str, "newline") == 0)
			error = "`newline'";
		name_error("syntax error near unexpected token ", NULL, error);
		return (1);
	}
	return (0);
}

void	ft_lexerclear(t_lexer **lst)
{
	t_lexer	*tmp;

	if (!*lst)
		return ;
	while (*lst)
	{
		tmp = (*lst)->next;
		if ((*lst)->str)
			free((*lst)->str);
		free(*lst);
		*lst = tmp;
	}
	*lst = NULL;
}
