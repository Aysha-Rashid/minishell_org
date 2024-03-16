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
	if (!data->executor)
	{
		ft_putstr_fd("execution memory", 1);
		return (0);
	}
	data->executor->cmd = ft_strdup(cmd);
	// data->executor->here_name = "home";
	data->executor->pipes = 0;
	data->executor->heredoc = 0;
	data->executor->in = 0;
	data->executor->out = 0;
	data->executor->next = NULL;
	// data->executor->prev = NULL;
	return (data->executor);
}

t_executor	*parse_pipeline(char *cmd, t_data *data)
{
	t_executor	*head;
	t_executor	*tail;
	t_executor	*executor;
	char		**token;
	int			i;

	i = 0;
	head = NULL;
	tail = NULL;
	token = ft_split(cmd, '|');
	while (token[i])
	{
		executor = init_executor(data, token[i++]);
		if (head == NULL)
			head = executor;
		else
		{
			tail->next = executor;
			executor->prev = tail;
		}
		tail = executor;
	}
	free_array(token);
	return (head);
}

void	execute_command(char *cmd, t_data *data, int *end)
{
	if (ft_strchr(cmd, '$') || ft_strchr(cmd, '?'))
		exit (0);
	if (check_builtin(&cmd) >= 0)
	{
		builtin_command(cmd, data);
		exit(0);
	}
	if (data->no_path)
	{
		ft_error(2, cmd, data->no_path);
		close_and_free_all(data, end);
		exit(0);
	}
	else
		cmd_file(cmd, data->envp->path);
	close_and_free_all(data, end);
	exit(1);
}

int	execution(t_executor *executor, t_data *data)
{
	int		end[2];
	int		pid;

	while (executor)
	{
		pipe(end);
		pid = fork();
		if (pid < 0)
			return (ft_putstr_fd("fork error", 1), 1);
		else if (pid == 0)
		{
			if (executor->next)
				ft_dup_fd(end);
			execute_command(executor->cmd, data, end);
		}
		else
		{
			if (executor->next)
				close(end[1]);
		}
		executor = executor->next;
		close(end[0]);
		close(end[1]);
		while (wait(&data->status_code) > 0);
	}
	return (0);
}

int	check_builtin(char **str)
{
	int			i;
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
	while (i < 10)
	{
		if (ft_strcmp(builtins[i], str[0]) == 0)
			return (i);
		i++;
	}
	return (-1);
}
