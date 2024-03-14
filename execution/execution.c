/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ayal-ras <ayal-ras@student.42abudhabi.a    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/08 17:27:11 by ayal-ras          #+#    #+#             */
/*   Updated: 2024/03/13 14:35:12 by ayal-ras         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int 	g_sig_interrupt = 0;


// void execution(t_executor *executor, t_data *data)
// {
//     static int i;

//     i = 0;

//     // ft_putnbr_fd(executor->pipes, 1);
//     if (executor->pipes > 0)
//     {
// 		// write(1, "here", 4);
//         data->pid = malloc(sizeof(int) * executor->pipes + 2);
//         if (!data->pid) {
//             // Handle allocation failure here
//             return;
//         }
//         while (i < executor->pipes)
//         {
//             data->pid[i] = fork();
//             if (data->pid[i] == -1) {
//                 ft_error(3, NULL, 0);
//                 // Handle fork failure here
//             }

//             if (data->pid[i] == 0)
//             {
//                 cmd_file(data, data->envp->path);
//                 exit(0);
//             }
//             i++;
//             executor = executor->next;
//         }
//     }
//     else 
//     {
// 		// write(1, "hello", 5);
//         int pid;
//         pid = fork();
//         if (pid == 0) {
//             // Child process
//             // free(executor);
//             cmd_file(data, data->envp->path);
//             ft_free_all(data);
//             exit(data->status_code);
//         }

//         // Parent process
//     }
//     waitpid(-1, &data->status_code, 0);
//     // exit(0);
// }
t_executor *init_executor(t_data *data, char *cmd)
{
	// (void)data;
	// t_executor *executor;
    data->executor = (t_executor *)malloc(sizeof(t_executor));
	if (!data->executor)
		return (0);
	// executor = executor;
    data->executor->cmd = ft_strdup(cmd);
    data->executor->pipes = 0;
    data->executor->heredoc = 0;
    data->executor->in = 0;
    data->executor->out = 0;
    data->executor->hd_file_name = "home";
    // data->executor->pid = 0;
    data->executor->next = NULL;
    data->executor->prev = NULL;
    // ft_simple_cmdsadd_back(&data->executor, data->executor);
	return data->executor;
}

t_executor *parse_pipeline(char *cmd, t_data *data)
{
	 t_executor *head = NULL;
    t_executor *tail = NULL;
    char *token;
    char *pipeline_copy = strdup(cmd); // Duplicate the pipeline string

    // Tokenize the pipeline string using pipe symbol as delimiter
    token = strtok(pipeline_copy, "|");
    while (token != NULL) {
        // Create an executor node for the current command
        t_executor *executor = init_executor(data, token);
        count_pipes(data->lexer_list, data);
        // ft_putstr_fd(executor->cmd, 1);
        // write(1, "\n", 1);
        // Link the executor node to the pipeline
        if (head == NULL) {
            head = executor;
        } else {
            tail->next = executor;
            executor->prev = tail;
        }
        tail = executor;

        // Move to the next command in the pipeline
        token = strtok(NULL, "|");
    }

    // Free the duplicated pipeline string
    free(pipeline_copy);

    return head;

}

void execute_command(char *command, int in_fd, int out_fd, t_data *data) {
    // Redirect stdin/stdout if necessary
    if (in_fd != STDIN_FILENO) {
        dup2(in_fd, STDIN_FILENO);
        close(in_fd);
    }
    if (out_fd != STDOUT_FILENO) {
        dup2(out_fd, STDOUT_FILENO);
        close(out_fd);
    }
    // (void)command;
    (void)data;

    // the node has space in them ; it wont work if the input is { ls | pwd}
    // but it will work if its {ls|pwd}
    cmd_file(command, data->envp->path);
}

void execution(t_executor *executor, t_data *data)
{
    (void)executor;
    int fd_in = STDIN_FILENO;

    // ft_putstr_fd(data->executor->cmd, 1);
    while (data->executor) {
        int end[2];
        if (data->executor->next)
            pipe(end);

        // Fork a child process
        pid_t pid = fork();
        if (pid < 0) {
            perror("fork");
            return ;
        } else if (pid == 0)
        {
            // Child process
            // ft_putstr_fd(data->executor->cmd, 2);
            int fd_out = (data->executor->next) ? end[1] : STDOUT_FILENO;
            int fd_in = (data->executor->heredoc) ? open(data->executor->hd_file_name, O_RDONLY) : STDIN_FILENO;
            execute_command(data->executor->cmd, fd_in, fd_out, data);
            // Should not reach here
            exit(1);
        } else {
            // Parent process
            if (data->executor->next)
                close(end[1]);
            if (data->executor->prev)
                close(fd_in);
            fd_in = (data->executor->heredoc) ? fd_in : end[0];
        } 
        data->executor = data->executor->next;
    }
    int status;
    while (wait(&status) > 0);
    return ;
}

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
