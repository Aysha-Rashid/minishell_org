/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zfiros-a <zfiros-a@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/08 17:27:11 by ayal-ras          #+#    #+#             */
/*   Updated: 2024/03/15 15:29:51 by zfiros-a         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"
int 	g_sig_interrupt = 0;

t_executor	*init_executor(t_data *data, char *cmd)
{
	data->executor = (t_executor *)malloc(sizeof(t_executor));
	data->executor->cmd = ft_strdup(cmd);
	data->executor->pipes = 0;
	data->executor->in = STDIN_FILENO;
	data->executor->out = STDOUT_FILENO;
	data->executor->next = NULL;
	return (data->executor);
}

void	execute_command(char *cmd, t_data *data, int *end)
{
	char	*str;

	str = ft_strtrim(cmd, " ");
	check_command(str, cmd, end, data);
	free(str);
	cmd_file(cmd, data->envp->path);
	close_and_free_all(data, end);
	exit(1);
}

void	check_command(char *str, char *cmd, int *end, t_data *data)
{
	if (ft_strchr(cmd, '$') || ft_strchr(cmd, '?'))
	{
		ft_expansion3(data, str, 1);
		free(str);
		exit_and_free(data, end, 1);
	}
	else if (data->no_path)
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

int	execution(t_executor *executor, t_data *data)
{
	int	end[2];
	int	pid;

	while (executor)
	{
		redir(executor);
		pipe(end);
		pid = fork();
		if (pid < 0)
			return (perror("fork error"), 0);
		else if (pid == 0)
		{
			executor->cmd = remove_redir_or_files(executor->cmd);
			if (executor->next)
				ft_dup_fd(data, executor, end, 1);
			else
				ft_dup_fd(data, executor, end, 0);
			execute_command(executor->cmd, data, end);
		}
		if (!executor->next)
			close(end[0]);
		executor = executor->next;
		close(end[1]);
		wait(NULL);
	}
	return (0);
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
