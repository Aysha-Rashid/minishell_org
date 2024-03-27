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

void closing_execution(int pid)
{
	int status;

	status = 0;
	pid = waitpid(-1, &status, 0);
	while (pid > 0) {
		pid = waitpid(-1, &status, 0);
		WIFEXITED(status);
		// WIFSIGNALED(status);
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
	// heredoc(data, executor, prev, int *cur);
	// int check;
	// check = 0;
	executor->cmd = remove_redir_or_files(executor->cmd);
	if (prev[0] != STDIN_FILENO)
	{
		dup_check(prev[0], STDIN_FILENO);
		close(prev[1]);
	}
	if (executor->next &&cur != NULL && cur[1] != STDOUT_FILENO)
	{
		dup_check(cur[1], STDOUT_FILENO);
		close(cur[0]);
	}
	execute_command(executor->cmd, data, cur);
}

int	execution(t_executor *executor, t_data *data)
{
	int	cur_pipe[2];
	int prev_pipe[2];
	int	pid;

	prev_pipe[0] = STDIN_FILENO;
	prev_pipe[1] = STDOUT_FILENO;
	while (executor)
	{
		signal(SIGQUIT, ft_sig2);
		signal(SIGINT, ft_sig2);
		redir(executor);
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
