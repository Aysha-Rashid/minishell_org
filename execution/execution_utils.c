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
	if (data->executor == NULL)
		return (NULL);
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
		if (ft_strchr(cmd, '?'))
			exit_and_free(data, end, 127);
		exit_and_free(data, end, 126);
	}
	if (data->no_path)
	{
		free(str);
		ft_error(2, cmd, data->no_path);
		exit_and_free(data, end, 127);
	}
	if (check_builtin(str) >= 0)
	{
		if (builtin_command(str, data))
		{
			free(str);
			exit_and_free(data, end, 127);
		}
		free(str);
		exit_and_free(data, end, 0);
	}
}

int	check_builtin(char *str)
{
	int			i;
	char		**temp;
	char	*builtins[] = {
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
	if (!str || str[0] == '\0')
		return (-1);
	temp = ft_split(str, ' ');
	while (i < 10)
	{
		if (ft_strcmp(builtins[i], temp[0]) == 0)
			return (free_array(temp), i);
		i++;
	}
	return (free_array(temp), -1);
}

int	only_tabs_and_space(char *str)
{
	int		i;
	size_t	count;

	i = 0;
	count = 0;
	while (str[i])
	{
		if (str[i] == '\"' || str[i] == '\'')
			count++;
		if (str[i] == ' ' || str[i] == '\t')
			count++;
		i++;
	}
	if (count == (ft_strlen(str)))
		return (1);
	return (0);
}

int	check_redir_pipe(char *cmd)
{
	int	i;

	i = 0;
	// ft_putendl_fd(cmd, 2);
	while (cmd[i] && cmd[i] != '\'' && cmd[i] != '\"')
	{
		if (cmd[i] == '<' || cmd[i] == '>' || cmd[i] == '|')
			return (1);
		i++;
	}
	return (0);
}
