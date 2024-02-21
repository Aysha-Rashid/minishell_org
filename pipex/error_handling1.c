/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error_handling1.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ayal-ras <ayal-ras@student.42abudhabi.a    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/30 20:00:25 by ayal-ras          #+#    #+#             */
/*   Updated: 2024/02/08 14:28:03 by ayal-ras         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

void	arg_error(void)
{
	write(2, "invalid arguments\n", 19);
	close(STDERR_FILENO);
	close(STDIN_FILENO);
	close(STDOUT_FILENO);
	exit (1);
}

void	in_file_error(int infile)
{
	if (infile == -1)
	{
		write(2, strerror(errno), ft_strlen(strerror(errno)));
		write(2, "\n", 1);
		exit(0);
	}
}

void	ft_pipe_error(int *pipe_fd, t_data data)
{
	close(pipe_fd[1]);
	close(pipe_fd[0]);
	error_close_fd(data, pipe_fd);
	exit (1);
}

void	check_env(char **env)
{
	if (env[0] == NULL)
	{
		write(2, "no env\n", 8);
		exit(0);
	}
}
