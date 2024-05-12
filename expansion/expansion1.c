/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expansion1.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zfiros-a <zfiros-a@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/07 18:08:51 by zfiros-a          #+#    #+#             */
/*   Updated: 2024/03/15 15:27:37 by zfiros-a         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

size_t	dollar_sign(char *str)
{
	size_t	i;

	i = 0;
	while (str[i])
	{
		if (str[i] == '$')
			return (i + 1);
		i++;
	}
	return (0);
}

int	ft_expansion(t_data *data)
{
	int		i;
	char	**splitted;
	int		j;

	i = 0;
	splitted = ft_split(data->cmd, ' ');
	j = dollar_sign(splitted[0]);
	if (j == 0)
		return (ft_error(2, splitted[0], 0), free_array(splitted), 0);
	while (splitted[i])
	{
		ft_expansion3(data, splitted[i], 0);
		i++;
	}
	free_array(splitted);
	return (0);
}

int	init_expan(char *store, char *str, int j)
{
	if (store == NULL)
		return (0);
	store = ft_strncpy(store, str, j);
	store[j] = '\0';
	return (1);
}

int	do_expansion(t_data *data, char *str, int j, int flag)
{
	char	*temp;
	char	*old;
	char	*exp;
	char	*withoutquote;

	temp = NULL;
	old = (char *)malloc((j + 1) * sizeof(char));
	if (!init_expan(old, str, j))
		return (0);
	exp = search_env_variable2(data->envp, str + j);
	if (exp == NULL || exp[0] == '\0')
		return (free(old), 0);
	withoutquote = NULL;
	withoutquote = remove_quotes(old);
	temp = ft_strjoin(withoutquote, exp);
	return (ft_specified_error(temp, flag), free(exp),
		free(old), free(temp), free(withoutquote),1);
}

int	ft_expansion3(t_data *data, char *str, int flag)
{
	char	*status;
	char	**cmd;
	int		j;

	cmd = ft_split(str, ' ');
	status = ft_itoa(g_signal);
	j = dollar_sign(cmd[0]);
	if (j != 0 && cmd[0][j] != '\0')
	{
		if (cmd[0][j] == '?')
		{
			if (data->no_path)
				return (name_error3(status, "No such file or directory", flag),
					free(status), free_array(cmd), 1);
			return (name_error3(status, "command not found", flag),
				free(status), free_array(cmd), 1);
		}
		else
		{
			if (do_expansion(data, cmd[0], j, flag))
				return (free(status), free_array(cmd), 1);
		}
	}
	return (free(status), free_array(cmd), 0);
}
