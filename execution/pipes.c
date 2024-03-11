/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipes.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ayal-ras <ayal-ras@student.42abudhabi.a    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/28 17:03:36 by ayal-ras          #+#    #+#             */
/*   Updated: 2024/03/11 13:23:03 by ayal-ras         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	check_n_execute(char *str, t_data *data)
{
	if (!data->cmd || data->cmd[0] == '\0')
		return ;
	if (str && (!ft_strncmp(str, "exit", 5)))
	{
		ft_putendl_fd("\033[0;32msee you around 😮‍💨!\033[0m", 1);
		ft_putendl_fd("exit", 1);
		ft_free_all(data);
		exit(0);
	}
	if (!token_reader(data))
		ft_error(3, NULL, data->no_path);
	if (check_pipes_n_execute(data))
		return ;
}

void	free_lexer_list(t_lexer *list)
{
	t_lexer	*current;
	t_lexer	*next;

	current = list;
	while (current != NULL)
	{
		next = current->next;
		free(current->str);
		free(current);
		current = next;
	}
	list = NULL;
}

int	check_pipes_n_execute(t_data *data)
{
	t_lexer		*temp;

	temp = data->lexer_list;
	init_executor(data);
	count_pipes(temp, data);
	if (parsing_lexar(data, temp))
		return (1);
	if (data->lexer_list->token == 0 && data->executor->pipes == 0)
		return (simple_cmd(data->cmd, data), free(data->executor), 1);
	data->executor->pid = ft_calloc(sizeof(int), data->executor->pipes + 2);
	if (!data->executor->pid)
		return (ft_error(3, NULL, data->no_path), free(data->executor), 1);
	if (fork() == 0)
		execution(data->executor, data);
	waitpid(-1, &data->status_code, 0);
	free(data->executor->pid);
	return (free(data->executor), 0);
}

void	count_pipes(t_lexer *lexer, t_data *data)
{
	int	num_pipe;

	num_pipe = 0;
	while (lexer)
	{
		if (lexer->token == PIPE)
			num_pipe++;
		lexer = lexer->next;
	}
	data->executor->pipes = num_pipe;
}

static void	left_side(t_executor *nd, t_data *data, int pdes[2])
{
	close(STDOUT_FILENO);
	dup(pdes[1]);
	close(pdes[0]);
	close(pdes[1]);
	execution(nd, data);
}

static void	right_side(t_executor *nd, t_data *data, int pdes[2])
{
	close(STDIN_FILENO);
	dup(pdes[0]);
	close(pdes[0]);
	close(pdes[1]);
	execution(nd, data);
}

void execute_pipe(t_data *data, t_executor *pid)
{
// 	int pipe_fd[2];
// 	// int i = 0;

// 	write(1, "comes here", 10);
// 	if (pipe(pipe_fd) == -1)
// 	{
// 		perror("pipe");
// 		exit(1);
// 	}

// 	pid_t child_pid = fork();
// 	if (child_pid < 0)
// 	{
// 		perror("fork");
// 		exit(1);
// 	}
// 	else if (child_pid == 0)
// 	{
// 		close(pipe_fd[0]);
// 		dup2(pipe_fd[1], STDOUT_FILENO);
// 		close(pipe_fd[1]);
// 		execution(data, pid);
// 		exit(0);
// 	}
// 	else
// 	{
// 		close(pipe_fd[1]);
// 		dup2(pipe_fd[0], STDIN_FILENO);
// 		close(pipe_fd[0]);
// 		execution(data, pid);
// 		close(pipe_fd[0]);
// 		waitpid(child_pid, &data->status_code, 0);
// 	}
// }
	(void) pid;
	pid_t	child_pid;
	int		pipedes[2];
	int		temp_status;

	// node->operator = NONE;
	if (pipe(pipedes) == -1)
		return ;
	child_pid = fork();
	if (child_pid == -1)
		return ;
	if (child_pid == 0)
		left_side(data->executor, data, pipedes);
	right_side(data->executor->next, data, pipedes);
	close(pipedes[0]);
	close(pipedes[1]);
	waitpid(child_pid, &temp_status, 0);
}
