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
	data->executor->in = STDIN_FILENO;
	data->executor->out = STDOUT_FILENO;
	data->executor->heredoc = NULL;
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

void	child_process(t_data *data, t_executor *executor, int *end)
{
	executor->cmd = remove_redir_or_files(executor->cmd);
	if (executor->next)
		ft_dup_fd(data, executor, end, 1);
	else
		ft_dup_fd(data, executor, end, 0);
	close(end[0]);
	close(end[1]);
	execute_command(executor->cmd, data, end);
}

int	execution(t_executor *executor, t_data *data)
{
	int	end[2];
	int	pid;
	int	here;

	end[0] = 3;
	end[1] = 4;
	here = 0;
	while (executor)
	{
		if (signal(SIGQUIT, ft_sig2))
			data->status_code = 131;
		if (signal(SIGINT, ft_sig2))
			data->status_code = 130;
		here = heredoc(data, executor, end);
		executor->cmd = remove_heredoc(executor->cmd);
		redir(executor);
		if (executor->next)
			pipe(end);
		pid = fork();
		if (pid < 0)
			return (perror("fork error"), 0);
		else if (pid == 0)
			child_process(data, executor, end);
		executor = executor->next;
	}
	close(end[1]);
	close(end[0]);
	while (waitpid(-1, &data->status_code, 0) >= 0);
	return (0);
}

// int execution(t_executor *executor, t_data *data) {
//     int prev_read_fd = STDIN_FILENO; // Initialize the read file descriptor with stdin
//     int fds[2]; // File descriptors for pipe
//     int pid;

    // while (executor) {
    //     // Create a pipe for communication between processes
    //     if (pipe(fds) == -1) {
    //         perror("pipe error");
    //         return -1;
    //     }

    //     // Fork a child process
    //     pid = fork();
    //     if (pid == -1) {
    //         perror("fork error");
    //         return -1;
    //     } else if (pid == 0) { // Child process
    //         // Redirect input to come from the previous process's output
    //         dup2(prev_read_fd, STDIN_FILENO);
    //         close(fds[0]); // Close unused read end of pipe

    //         // Redirect output to go to the next process's input
    //         if (executor->next) {
    //             dup2(fds[1], STDOUT_FILENO);
    //         }

    //         // Execute the command
    //         execute_command(executor->cmd, data, NULL);
    //         exit(1);
    //     } else { // Parent process
    //         close(fds[1]); // Close unused write end of pipe
    //         close(prev_read_fd); // Close previous process's output
    //         prev_read_fd = fds[0]; // Update previous read file descriptor for next process
    //         executor = executor->next;
    //     }
    // }

    // Wait for all child processes to finish
// 	while (waitpid(-1, &data->status_code, 0) > 0)
// 	;
//     return 0;
// }


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
