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
	t_executor	*executor;

	(void)data;
	executor = (t_executor *)malloc(sizeof(t_executor));
	executor->cmd = NULL;
	if (executor == NULL)
		return (NULL);
	executor->cmd = ft_strdup(cmd);
	executor->in = STDIN_FILENO;
	executor->out = STDOUT_FILENO;
	executor->next = NULL;
	executor->prev = NULL;
	return (executor);
}

void	check_command(char *str, char *cmd, t_data *data)
{
	if (ft_strchr(cmd, '$') || ft_strchr(cmd, '?'))
	{
		if (ft_expansion3(data, str, 0))
		{
			free(str);
			exit_and_free(data, 126);
		}
		if (ft_strchr(cmd, '?'))
		{
			free(str);
			exit_and_free(data, 127);
		}
	}
	if (check_builtin(str) >= 0)
	{
		if (builtin_command(str, data))
		{
			free(str);
			exit_and_free(data, 0);
		}
		free(str);
		exit_and_free(data, 127);
	}
}

int	check_builtin(char *str)
{
	int		i;
	char	**temp;
	char	*builtins[11];

	builtins[0] = "echo";
	builtins[1] = "cd";
	builtins[2] = "pwd";
	builtins[3] = "export";
	builtins[4] = "unset";
	builtins[5] = "env";
	builtins[6] = "exit";
	builtins[7] = "ENV";
	builtins[8] = "PWD";
	builtins[9] = "ECHO";
	builtins[10] = NULL;
	i = -1;
	temp = ft_split(str, ' ');
	if (temp == NULL || temp[0] == NULL)
		return (free_array(temp), -1);
	while (++i < 10)
	{
		if (ft_strcmp(builtins[i], temp[0]) == 0)
			return (free_array(temp), i);
	}
	return (free_array(temp), -1);
}

int	only_tabs_and_space(char *str)
{
	int		i;
	size_t	count;
	// char	quote;

	i = 0;
	count = 0;
	// quote = 0;
	while (str[i])
	{
		// if (str[i] == '\"' || str[i] == '\'')
		// 	count++;
		if (str[i] == ' ' || str[i] == '\t')
		{

			count++;
		}
		if (count == (ft_strlen(str)))
		{
			return (1);
		}
		i++;
	}
	return (0);
}

int	check_redir_pipe(char *cmd)
{
	int	i;

	i = 0;
	while (cmd[i])
	{
		if (cmd[i] == '<' || cmd[i] == '>' || cmd[i] == '|')
			return (1);
		i++;
	}
	return (0);
}
