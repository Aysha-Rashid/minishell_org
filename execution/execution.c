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

void	execute_command(char *cmd, t_data *data, t_executor *executor)
{
	char	*str;

	if (only_tabs_and_space(cmd, 0))
	{
		close_and_free_all(data);
		exit (0);
	}
	str = ft_strtrim(cmd, " ");
	if (!only_tabs_and_space(cmd, 1))
		check_command(str, cmd, executor, data);
	if (data->no_path)
	{
		ft_error(2, cmd, data->no_path);
		exit_and_free(data, 127, str);
	}
	cmd_file(cmd, data->envp->path, data);
	free(str);
	close_and_free_all(data);
	exit(127);
}

void	closing_execution(int pid, t_data *data)
{
	int	status;

	(void)data;
	status = 0;
	while (pid > 0)
	{
		if (!g_signal && !WIFEXITED(status) && WIFSIGNALED(status)
			&& g_signal != IN_HERE)
		{
			if (WTERMSIG(status) == SIGINT)
				g_signal = 130;
			if (WTERMSIG(status) == SIGQUIT)
			{
				printf("Quit: 3\n");
				g_signal = 131;
			}
		}
		if (WIFEXITED(status))
		{
			status = WEXITSTATUS(status);
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
	char	*temp;

	exec_quote_redir(executor, data);
	if (ft_strstr(executor->cmd, "<<"))
		executor->heredoc = heredoc(executor, data);
	if (ft_strchr(executor->cmd, '<'))
		dup_check(executor->in, STDIN_FILENO);
	else if (prev[0] != STDIN_FILENO)
	{
		dup_check(prev[0], STDIN_FILENO);
		close(prev[1]);
	}
	if (ft_strchr(executor->cmd, '>'))
		dup_check(executor->out, STDOUT_FILENO);
	else if (executor->next && cur != NULL && cur[1] != STDOUT_FILENO)
	{
		dup_check(cur[1], STDOUT_FILENO);
		close(cur[0]);
	}
	temp = remove_redir_or_files(executor->cmd);
	free(executor->cmd);
	executor->cmd = temp;
	execute_command(executor->cmd, data, executor);
}

int	execution(t_executor *executor, t_data *data)
{
	int	cur_pipe[2];
	int	prev_pipe[2];
	int	pid;

	init_pipe_n_signal(prev_pipe);
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
		close_exec_files(executor->in, executor->out);
		executor = executor->next;
	}
	closing_execution(pid, data);
	return (0);
}
