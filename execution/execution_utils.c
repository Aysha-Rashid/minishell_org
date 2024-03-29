/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution_utils.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ayal-ras <ayal-ras@student.42abudhabi.a    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/21 14:40:08 by ayal-ras          #+#    #+#             */
/*   Updated: 2024/03/22 15:02:57 by ayal-ras         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

t_executor	*init_executor(t_data *data, char *cmd)
{
	data->executor = (t_executor *)malloc(sizeof(t_executor));
	data->executor->cmd = ft_strdup(cmd);
	data->executor->in = STDIN_FILENO;
	data->executor->out = STDOUT_FILENO;
	data->executor->next = NULL;
	data->executor->prev = NULL;
	return (data->executor);
}

void	check_command(char *str, char *cmd, int *end, t_data *data)
{
	if (ft_strchr(cmd, '$') || ft_strchr(cmd, '?'))
	{
		ft_expansion3(data, str, 0);
		free(str);
		exit_and_free(data, end, 1);
	}
	if (data->no_path)
	{
		free(str);
		ft_error(2, cmd, data->no_path);
		exit_and_free(data, end, 0);
	}
	if (check_builtin(str) >= 0)
	{
		if (builtin_command(str, data))
		{
			free(str);
			exit_and_free(data, end, 0);
		}
		free(str);
		exit_and_free(data, end, 1);
	}
}

int	check_builtin(char *str)
{
	int			i;
	char		**temp;
	static char	*builtins[] = {
		"echo",
		"cd",
		"pwd",
		"export",
		"unset",
		"env",
		"exit",
		"ENV",
		"PWD",
		"ECHO",
	};

	i = 0;
	temp = ft_split(str, ' ');
	while (i < 10)
	{
		if (ft_strcmp(builtins[i], temp[0]) == 0)
			return (free_array(temp), i);
		i++;
	}
	return (free_array(temp), -1);
}
