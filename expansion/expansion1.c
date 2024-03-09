/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expansion1.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zfiros-a <zfiros-a@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/07 18:08:51 by zfiros-a          #+#    #+#             */
/*   Updated: 2024/03/08 14:48:44 by zfiros-a         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

char	*expander2(t_data *data, char *str)
{
	int		i;
	char	*tmp;

	i = 0;
	tmp = NULL;
	while (str[i])
	{
		if (str[dollar_sign(str[i]) - 2] != '\'' && dollar_sign(str[i]) != 0
			&& str[dollar_sign(str[i])] != '\0')
		{
			tmp = detect_dollar_sign(data, str);
			free(str);
			str = tmp;
		}
		if (strncmp(str, "export"), ft_strlen(str - 1) != 0)
		{
			str = delet_quotes(str, "\"");
			str = delete_quotes(str, "\'");
		}
		i++;
	}
	return (str);
}

char	*expander(t_data *data, char *str)
{
	t_lexer	*start;

	data->cmd = expander2(data, data->cmd);
	start = data->lexer_list;
	while (data->lexer_list)
	{
		if (data->lexer_list->token != HEREDOC)
			data->lexer_list
	}
}

void	single_cmd(char *cmd, t_data *data)
{
	data->cmd = expander(data, data->cmd);
}

int	ft_expansion(t_data *data)
{
	signal(SIGQUIT, sigint_handler);
	if (data->executor->pipes == 0)
		single_cmd(data->cmd, data);
}
