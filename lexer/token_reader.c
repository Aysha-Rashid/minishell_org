/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token_reader.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ayal-ras <ayal-ras@student.42abudhabi.a    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/27 08:35:24 by zfiros-a          #+#    #+#             */
/*   Updated: 2024/03/04 14:34:33 by ayal-ras         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	is_whitespace(char c)
{
	return (c == ' ' || (c > 8 && c < 14));
}

int	skip_spaces(char *str, int i)
{
	int	j;

	j = 0;
	while (is_whitespace(str[i + j]))
		j++;
	return (j);
}

int	add_node(char *str, t_tokens token, t_lexer **lexer_list)
{
	t_lexer	*node;

	node = ft_lexernew(str, token);
	if (!node)
		return (0);
	if ((ft_lexeradd_back(lexer_list, node)))
		return (0);
	return (1);
}

int	read_words(int i, char *str, t_lexer **lexer_list)
{
	char	*temp;
	int		j;

	j = 0;
	while (str[i + j] && !(check_token(str[i + j])))
	{
		j = j + handle_quotes(i + j, str, 34);
		j = j + handle_quotes(i + j, str, 39);
		if (is_whitespace(str[i + j]))
			break ;
		else
			j++;
	}
	temp = ft_substr(str, i, j);
	if (!temp)
		return (0);
	if ((add_node(temp, 0, lexer_list) == 0))
		return (free(temp), 0);
	free(temp);
	return (j);
}

int	token_reader(t_data *data)
{
	int	i;
	int	j;

	i = 0;
	data->lexer_list = NULL;
	while (data->cmd[i])
	{
		j = 0;
		i = i + skip_spaces(data->cmd, i);
		if (check_token(data->cmd[i]))
			j = handle_token(data->cmd, i, &data->lexer_list);
		else
			j = read_words(i, data->cmd, &data->lexer_list);
		if (j < 0)
			return (0);
		i = i + j;
	}
	return (1);
}
