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

int	g_sig_interrupt = 0;

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

void	closing_execution(int pid)
{
	int	status;

	status = 0;
	pid = waitpid(-1, &status, 0);
	while (pid > 0)
	{
		if (g_signal == 10)
		{
			g_signal = 1;
		}
		else if (WIFEXITED(status))
		{
			status = WEXITSTATUS(status);
			if (status == 1 && g_signal != IN_HERE)
				status = 127;
			if (status == 3)
				status = 1;
			else if (g_signal == 2)
				status = 1;
			g_signal = status;
		}
		pid = waitpid(-1, &status, 0);
	}
}

void	parent_process(t_executor *executor, int *prev_pipe, int *cur_pipe)
{
	if (executor->prev)
	{
		close(prev_pipe[0]);
		close(prev_pipe[1]);
	}
	prev_pipe[0] = cur_pipe[0];
	prev_pipe[1] = cur_pipe[1];
}

void	child_process(t_data *data, t_executor *executor, int *prev, int *cur)
{
	char *temp;
	heredoc(executor, cur, data);
	// signal(SIGQUIT, ft_sig2);
	if (ft_strchr(data->cmd, '<') && executor->in != STDIN_FILENO)
		dup_check(executor->in, STDIN_FILENO);
	else if (prev[0] != STDIN_FILENO)
	{
		dup_check(prev[0], STDIN_FILENO);
		close(prev[1]);
	}
	if (ft_strchr(data->cmd, '>') && executor->out != STDOUT_FILENO)
		dup_check(executor->out, STDOUT_FILENO);
	else if (executor->next && cur != NULL && cur[1] != STDOUT_FILENO)
	{
		dup_check(cur[1], STDOUT_FILENO);
		close(cur[0]);
	}
	temp = remove_redir_or_files(executor->cmd);
	executor->cmd = temp;
	// free(temp);
	execute_command(executor->cmd, data, cur);
}

int	execution(t_executor *executor, t_data *data)
{
	int	cur_pipe[2];
	int	prev_pipe[2];
	int	pid;

	prev_pipe[0] = STDIN_FILENO;
	prev_pipe[1] = STDOUT_FILENO;
	signal(SIGQUIT, ft_sig2);
	signal(SIGINT, ft_sig2);
	while (executor)
	{
		if (redir(executor) == 0)
			return (1);
		if (executor->next)
			pipe(cur_pipe);
		pid = fork();
		if (pid < 0)
			return (perror("fork error"), 0);
		else if (pid == 0)
			child_process(data, executor, prev_pipe, cur_pipe);
		else
			parent_process(executor, prev_pipe, cur_pipe);
		executor = executor->next;
	}
	closing_execution(pid);
	return (0);
}
