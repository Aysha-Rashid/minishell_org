/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ayal-ras <ayal-ras@student.42abudhabi.a    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/08 17:27:11 by ayal-ras          #+#    #+#             */
/*   Updated: 2024/03/12 14:27:37 by ayal-ras         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"
int 	g_sig_interrupt = 0;

void	init_executor(t_data *data)
{
	data->executor = (t_executor *)malloc(sizeof(t_executor));
	if (!data->executor)
		return ;
	data->executor->pipes = 0;
	data->executor->heredoc = 0;
	data->executor->in = 0;
	data->executor->out = 0;
	data->executor->next = NULL;
}

void	child_signals(int signum)
{
	if (signum == SIGINT)
	{
		ft_putchar_fd('\n', STDOUT_FILENO);
		// g_exit_status = 130;
		exit(130);
	}
}

void	execution(t_data *data)
{
	static int	i;

	i = 0;
	data->executor->pid = malloc((sizeof(int) * data->executor->pipes) + 2);
	data->executor->pid[i] = 0;
	data->executor->pid[i] = fork();
	if (data->executor->pid[i] == -1)
		ft_error(3, NULL, 0);
	if (data->executor->pid[i] == 0)
	{
		cmd_file(data, data->envp->path);
		free(data->lexer_list);
		ft_free_all(data);
		free(data->executor);
		i++;
		exit(data->status_code);
	}
	waitpid(-1, &data->status_code, 0);
}

// void execution(t_data *data) {
//     int pid;
//     static int i = 0;

//     pid = fork();
//     if (pid == -1) {
//         ft_error(3, NULL, 0);
//         return; // Exit function on fork failure
//     }

//     if (pid == 0) {
//         // Child process
//         cmd_file(data, data->envp->path);
//         free(data->lexer_list);
//         ft_free_all(data);
//         free(data->executor->pid); // Free memory allocated for pid array
//         exit(data->status_code);
//     } else {
//         // Parent process
//         // Allocate memory for pid array if not already allocated
//         if (data->executor->pid == NULL) {
//             data->executor->pid = malloc(sizeof(int) * (i + 2));
//             if (data->executor->pid == NULL) {
//                 // Memory allocation failed, handle error
//                 ft_error(3, NULL, 0);
//                 return;
//             }
//         }
//         data->executor->pid[i] = pid; // Store process ID
//         i++;
//         waitpid(pid, &data->status_code, 0);
//     }
// }


// int	simple_cmd(char *cmd, t_data *data)
// {
// 	char	**str;
// 	int		builtin_index;
// 	static int i;
// 	i = 0;
// 	// pid = 0;
// 	data->executor->pid = malloc(sizeof(int) * data->executor->pipes + 2);
// 	str = ft_split(data->cmd, ' ');
// 	builtin_index = check_builtin(str);
// 	free_array(str);
// 	if (builtin_index)
// 		return (builtin_command(cmd, data), 1);
// 	else
// 	{
// 		data->executor->pid[i] = fork();
// 		if (data->executor->pid[i] == -1)
// 			ft_error(3, NULL, 0);
// 		if (data->executor->pid[i] == 0)
// 		{
// 			cmd_file(data, data->envp->path);
// 			free(data->lexer_list);
// 			free(data->executor);
// 			ft_free_all(data);
// 			i++;
// 			// exit(data->status_code);
// 		}
// 		waitpid(-1, &data->status_code, 0);
// 		return (0);
// 	}
// 	ft_error(2, data->cmd, 0);
// 	return (1);
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
		"exit"
	};

	i = 0;
	while (i < 7)
	{
		if (ft_strcmp(builtins[i++], str[0]) == 0)
			return (i);
	}
	return (0);
}
