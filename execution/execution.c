/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ayal-ras <ayal-ras@student.42abudhabi.a    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/08 17:27:11 by ayal-ras          #+#    #+#             */
/*   Updated: 2024/03/14 13:44:02 by ayal-ras         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int 	g_sig_interrupt = 0;

t_executor	*init_executor(t_data *data, char *cmd)
{
	data->executor = (t_executor *) malloc(sizeof(t_executor));
	// data = (t_executor *)malloc(sizeof(t_executor));
	if (!data->executor)
		return (0);
	data->executor->cmd = ft_strdup(cmd);
	data->executor->here_name = "home";
	data->executor->pipes = 0;
	data->executor->heredoc = 0;
	data->executor->in = 0;
	data->executor->out = 0;
	data->executor->next = NULL;
	data->executor->prev = NULL;
	return (data->executor);
}

t_executor *parse_pipeline(char *cmd, t_data *data)
{
	t_executor	*head;
	t_executor	*tail;
	char		**token;
	char		*pipeline_copy;
	int			i;

	i = 0;
	pipeline_copy = ft_strdup(cmd);
	head = NULL;
	tail = NULL;
	token = ft_split(pipeline_copy, '|');
	while (token[i])
	{
		t_executor *executor = init_executor(data, token[i]);
		count_pipes(data->lexer_list, data);
		if (head == NULL)
			head = executor;
		else
		{
			tail->next = executor;
			executor->prev = tail;
		}
		tail = executor;
		i++;
	}
	return (free(pipeline_copy), head);
}

void	execute_command(char *cmd, t_data *data)
{
	if (data->executor->in != STDIN_FILENO)
	{
		dup2(data->executor->in, STDIN_FILENO);
		close(data->executor->in);
	}
	if (data->executor->out != STDOUT_FILENO)
	{
		dup2(data->executor->out, STDOUT_FILENO);
		close(data->executor->out);
	}
	cmd_file(cmd, data->envp->path);
}

void	execution(t_data *data)
{
	int	fd_in;
	int	end[2];
	pid_t	pid;

	fd_in = STDIN_FILENO;
	if (data->executor->pipes)
	{
		while (data->executor)
		{
			if (data->executor->next)
				pipe(end);
			pid = fork();
			if (pid < 0)
			{
				perror("fork");
				return ;
			}
			else if (pid == 0)
			{
				data->executor->out = (data->executor->next) ? end[1] : STDOUT_FILENO;
				fd_in = (data->executor->heredoc) ? open(data->executor->here_name, O_RDONLY) : STDIN_FILENO;
				execute_command(data->executor->cmd, data);
				write(2, "out", 3);
				exit(1);
			}
			else
			{
				if (data->executor->next)
					close(end[1]);
				if (data->executor->prev)
					close(fd_in);
				fd_in = (data->executor->heredoc) ? fd_in : end[0];
			}
			data->executor = data->executor->next;
		}
	}
	else
	{
		pid = fork();
		if (pid == 0)
		{
			cmd_file(data->cmd, data->envp->path);
			ft_free_all(data);
			exit(data->status_code);
		}
	}
	waitpid(pid, &data->status_code, 0);
	// exit(0);
	return ;
}
// void execution(t_data *data)
// {
//     static int i;

//     i = 0;
//     // ft_putnbr_fd(data->executor->pipes, 1);
//     if (data->executor->pipes > 0)
//     {
//         data->executor->pid = malloc(sizeof(int) * data->executor->pipes);
//         if (!data->executor->pid) {
//             // Handle allocation failure here
//             return;
//         }
//         while (i < data->executor->pipes)
//         {
//             data->executor->pid[i] = fork();
//             if (data->executor->pid[i] == -1) {
//                 ft_error(3, NULL, 0);
//                 // Handle fork failure here
//             }

//             if (data->executor->pid[i] == 0) {
//                 // Child process
//                 free(data->executor);
//                 cmd_file(data, data->envp->path);
//                 ft_free_all(data);
//                 exit(data->status_code);
//             }
//             i++;
//         }
//     }
//     else 
//     {
//         int pid;
//         pid = fork();
//         if (pid == 0) {
//             // Child process
//             free(data->executor);
//             cmd_file(data, data->envp->path);
//             ft_free_all(data);
//             exit(data->status_code);
//         }

//         // Parent process
//     }
//     waitpid(-1, &data->status_code, 0);
//     // exit(0);
// }

// void wait_pid(int *pid, int amount, t_data *data)
// {
//     int i;

//     i = 0;

//     while (i < amount)
//     {
//         waitpid(pid[i], &data->status_code, 0);
//         i++;
//     }
//     waitpid(pid[i], &data->status_code, 0);
//     exit(0);
// }

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
