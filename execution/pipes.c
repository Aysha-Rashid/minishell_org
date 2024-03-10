/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipes.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ayal-ras <ayal-ras@student.42abudhabi.a    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/28 17:03:36 by ayal-ras          #+#    #+#             */
/*   Updated: 2024/03/10 21:22:10 by ayal-ras         ###   ########.fr       */
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
	// init_executor(data);
	data->executor = (t_executor *)malloc(sizeof(t_executor));
	// if (!data->executor)
	// 	return ;
	data->executor->pipes = 0;
	data->executor->heredoc = 0;
	data->executor->in = 0;
	data->executor->out = 0;
	count_pipes(temp, data);
	ft_putnbr_fd(data->executor->pipes, 1);
	if (parsing_lexar(data, temp))
		return (1);
	if (data->lexer_list->token == 0 && data->executor->pipes == 0)
		return (simple_cmd(data->cmd, data), free(data->executor), 1);
	data->executor->pid = ft_calloc(sizeof(int), data->executor->pipes + 2);
	if (!data->executor->pid)
		return (ft_error(3, NULL, data->no_path), free(data->executor), 1);
	else if (fork() == 0)
		execution(data, data->executor->pid);
	waitpid(-1, &data->status_code, 0);
	// free(data->executor->pid);
	return (free(data->executor), 0);
}

void	count_pipes(t_lexer *lexer, t_data *data)
{
	int num_pipe = 0;
	while (lexer)
	{
		if (lexer->token == PIPE)
			num_pipe++;
		lexer = lexer->next;
	}
	data->executor->pipes = num_pipe;
}

void execute_pipe(t_data *data, int *pid)
{
    int pipe_fd[2];
    // int i = 0;

    if (pipe(pipe_fd) == -1)
    {
        perror("pipe");
        exit(1); // Exit on pipe creation failure
    }

    pid_t child_pid = fork();
    if (child_pid < 0)
    {
        perror("fork");
        exit(1); // Exit on fork failure
    }
    else if (child_pid == 0)
    {
        close(pipe_fd[0]); // Close read end of pipe in child
        dup2(pipe_fd[1], STDOUT_FILENO); // Redirect stdout to pipe
        close(pipe_fd[1]); // Close write end of pipe
        execution(data, pid);
        exit(0); // Exit child process after execution
    }
    else
    {
        close(pipe_fd[1]); // Close write end of pipe in parent
        dup2(pipe_fd[0], STDIN_FILENO); // Redirect stdin to pipe
        close(pipe_fd[0]); // Close read end of pipe
        execution(data, pid);
        close(pipe_fd[0]); // Close remaining file descriptor
        waitpid(child_pid, &data->status_code, 0);
    }
}


// void	expansion(t_data *data, char *cmd)
// {
	
// }
// found a $ sign then print the environment variable of it
// found a ? sign then print the exit status of the previous command
