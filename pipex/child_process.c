/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   child_process.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ayal-ras <ayal-ras@student.42abudhabi.a    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/25 14:20:17 by ayal-ras          #+#    #+#             */
/*   Updated: 2024/02/08 14:28:09 by ayal-ras         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

void	child_process(t_data data, char **env, int *pipe_fd)
{
	dup2(data.infile, STDIN_FILENO);
	dup2(pipe_fd[1], STDOUT_FILENO);
	dup2_error(data.infile, pipe_fd[1]);
	close(pipe_fd[0]);
	close(pipe_fd[1]);
	close(data.infile);
	error_close_fd(data, pipe_fd);
	data.cmd_path1 = cmd_file(*data.cmd1, env);
	ft_cmd_not_found(data.cmd_path1, data, data.outfile);
	if ((execve(data.cmd_path1, data.cmd1, NULL)) == -1)
	{
		free(data.cmd_path1);
		free_path(data.cmd2);
		free_path(data.cmd1);
		free(data.cmd_path2);
		write(2, strerror(errno), ft_strlen(strerror(errno)));
		write(2, "\n", 1);
		exit(1);
	}
}

void	another_child_process(t_data data, char **env, int *pipe_fd)
{
	dup2(pipe_fd[0], STDIN_FILENO);
	dup2(data.outfile, STDOUT_FILENO);
	dup2_error(data.outfile, pipe_fd[0]);
	close(pipe_fd[0]);
	close(pipe_fd[1]);
	close(data.outfile);
	error_close_fd(data, pipe_fd);
	data.cmd_path2 = cmd_file(*data.cmd2, env);
	ft_cmd_not_found(data.cmd_path2, data, data.infile);
	if ((execve(data.cmd_path2, data.cmd2, NULL)) == -1)
	{
		free(data.cmd_path2);
		free_path(data.cmd2);
		free_path(data.cmd1);
		free(data.cmd_path1);
		write(2, strerror(errno), ft_strlen(strerror(errno)));
		write(2, "\n", 1);
		exit(1);
	}
}

void	parent_process(t_data data, int *pipe_fd)
{
	int	i;

	i = 0;
	while (data.cmd1[i])
		free(data.cmd1[i++]);
	free(data.cmd1);
	i = 0;
	while (data.cmd2[i])
		free(data.cmd2[i++]);
	free(data.cmd2);
	close(pipe_fd[1]);
	close(pipe_fd[0]);
	close(data.infile);
	close(data.outfile);
	close(STDERR_FILENO);
	close(STDIN_FILENO);
	close(STDOUT_FILENO);
	error_close_fd(data, pipe_fd);
	wait(NULL);
	wait(NULL);
}

void	error_close_fd(t_data data, int *pipe_fd)
{
	if (pipe_fd[1] == -1)
		close_error();
	if (pipe_fd[0] == -1)
		close_error();
	if (data.outfile == -1)
		close_error();
	if (data.outfile == -1)
		close_error();
}
