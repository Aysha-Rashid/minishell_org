/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipes.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ayal-ras <ayal-ras@student.42abudhabi.a    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/28 17:03:36 by ayal-ras          #+#    #+#             */
/*   Updated: 2024/03/12 13:24:53 by ayal-ras         ###   ########.fr       */
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
	char		builtin_index;
	char		**str;

	temp = data->lexer_list;
	if (parsing_lexar(data, temp))
		return (1);
	init_executor(data);
	count_pipes(temp, data);
	str = ft_split(data->cmd, ' ');
	builtin_index = check_builtin(str);
	free_array(str);
	if (builtin_index)
		builtin_command(data->cmd, data);
	else
		execution(data);
	// data->executor->pid = ft_calloc(sizeof(int), data->executor->pipes + 2);
	// if (!data->executor->pid)
		// return (ft_error(3, NULL, data->no_path), free(data->executor), 1);
	// waitpid(-1, &data->status_code, 0);
	// free(data->executor->pid);
	return (0);
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

void left_side(t_data *data, int pdes[2])
{
    dup2(pdes[1], STDOUT_FILENO);
	cmd_file(data, data->envp->path);
	free(data->lexer_list);
	free(data->executor);
	ft_free_all(data);
    close(STDOUT_FILENO);
    close(pdes[0]);
    close(pdes[1]);
    execution(data);
}

void right_side(t_data *data, int pdes[2])
{
    dup2(pdes[0], STDIN_FILENO);
	cmd_file(data, data->envp->path);
	free(data->lexer_list);
	free(data->executor);
	ft_free_all(data);
    close(STDIN_FILENO);
    close(pdes[0]);
    close(pdes[1]);
    execution(data);
}

// void execute_pipe(t_data *data)
// {
// 	// (void) data;
//     pid_t child_pid;
//     int pipedes[2];
//     int temp_status;

//     if (pipe(pipedes) == -1) {
//         // Handle pipe creation error
//         return;
//     }
//     child_pid = fork();
//     if (child_pid == -1) {
//         // Handle fork error
//         return;
//     }
//     if (child_pid == 0) {
//         left_side(data, pipedes);
//     } else {
//         right_side(data, pipedes);
//     }
//     close(pipedes[0]);
//     close(pipedes[1]);
//     waitpid(child_pid, &temp_status, 0);
// }

void execute_pipe(t_data *data)
{
    int num_pipes = data->executor->pipes;
    pid_t child_pid;
    int pipedes[2];
    int temp_status;

    // Iterate over the number of pipes
    for (int i = 0; i < num_pipes; i++) {
        if (pipe(pipedes) == -1) {
            // Handle pipe creation error
            return;
        }

        child_pid = fork();
        if (child_pid == -1) {
            // Handle fork error
            return;
        }
        if (child_pid == 0) {
            // Child process

            // For the first command, redirect stdout to the write end of the pipe
            if (i == 0) {
                close(pipedes[0]); // Close read end of the pipe
                dup2(pipedes[1], STDOUT_FILENO); // Redirect stdout to the write end of the pipe
                close(pipedes[1]); // Close write end of the pipe
                cmd_file(data, data->envp->path); // Execute the command
                free(data->lexer_list);
                free(data->executor);
                ft_free_all(data);
                exit(0); // Exit the child process
            }
            // For subsequent commands, redirect stdin from the read end of the pipe
            else {
                close(pipedes[1]); // Close write end of the pipe
                dup2(pipedes[0], STDIN_FILENO); // Redirect stdin to the read end of the pipe
                close(pipedes[0]); // Close read end of the pipe
                // Execute the command
                cmd_file(data, data->envp->path);
                free(data->lexer_list);
                free(data->executor);
                ft_free_all(data);
				i--;
                exit(0); // Exit the child process
            }
        } else {
            // Parent process
            close(pipedes[0]); // Close read end of the pipe
            close(pipedes[1]); // Close write end of the pipe
        }
    }

    // Wait for the last child process to finish
    waitpid(child_pid, &temp_status, 0);
}
