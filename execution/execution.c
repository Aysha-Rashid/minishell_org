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
	// data->executor->here_name = "home";
	data->executor->pipes = 0;
	data->executor->in = 0;
	data->executor->out = 0;
	// data->executor->heredoc = 0;
	data->executor->next = NULL;
	// data->executor->prev = NULL;
	return (data->executor);
}

t_executor	*parse_pipeline(char *cmd, t_data *data)
{
	t_executor	*head;
	t_executor	*tail;
	t_executor	*executor;
	char		**token;
	int			i;

	i = 0;
	head = NULL;
	tail = NULL;
	token = ft_split(cmd, '|');
	while (token[i])
	{
		executor = init_executor(data, token[i++]);
		if (head == NULL)
			head = executor;
		else
		{
			tail->next = executor;
			executor->prev = tail;
		}
		tail = executor;
	}
	free_array(token);
	return (head);
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

void	check_command(char *str, char *cmd, int *end, t_data *data)
{
	if (ft_strchr(cmd, '$') || ft_strchr(cmd, '?'))
	{
		ft_expansion3(data, str, 1);
		free(str);
		exit_and_free(data, end, 1);
	}
	else if (data->no_path)
	{
		free(str);
		ft_error(2, cmd, data->no_path);
		exit_and_free(data, end, 0);
	}
	if (check_builtin(str) >= 0)
	{
		if (builtin_command(str, data))
		{
			free(str);
			exit_and_free(data, end, 0);
		}
		free(str);
		exit_and_free(data, end, 1);
	}
}

// int	execution(t_executor *executor, t_data *data)
// {
// 	int		end[2];
// 	int		pid;

// 	executor->cmd = remove_redir_or_files(executor->cmd);
// 	while (executor)
// 	{
// 		pipe(end);
// 		pid = fork();
// 		if (pid < 0)
// 			return (ft_putstr_fd("fork error", 1), 1);
// 		else if (pid == 0)
// 		{
// 			if (executor->next)
// 				ft_dup_fd(executor->in, executor->out, end);
// 			execute_command(executor->cmd, data, end);
// 		}
// 		else
// 		{
// 			if (executor->next)
// 				close(end[1]);
// 			// if (executor->in != 0)
// 			// if (executor->out != 0)
// 			// 	close(executor->out);
// 		}
// 		executor->in = end[0];
// 		close(end[1]);
// 		executor = executor->next;
// 		while (wait(&data->status_code) > 0);
// 	}
// 	return (0);
// }

int	execution(t_executor *executor, t_data *data)
{
	// int		prev_end[2] = { STDIN_FILENO, STDOUT_FILENO };
	int		curr_end[2];
	int		pid;

	executor->cmd = remove_redir_or_files(executor->cmd);
	while (executor)
	{
		pipe(curr_end); // Create pipe for current command
		pid = fork();
		if (pid < 0)
		{
			perror("fork error");
			return 1;
		}
		else if (pid == 0)
		{
			if (executor->next)
			{
				close(curr_end[0]);
				dup2(curr_end[1], STDOUT_FILENO); // Redirect stdout to write end of current pipe
				// ft_dup_fd(executor->in, executor->out, curr_end);
				close(curr_end[1]);
				if (ft_strchr(data->cmd, '<'))
				{
					dup2(executor->in, STDIN_FILENO); // Redirect stdin
					close(executor->in); // Close input file descriptor
				}
				if (ft_strchr(data->cmd, '>'))
				{
					dup2(executor->out, STDOUT_FILENO); // Redirect stdin
					close(executor->out); // Close input file descriptor
				}
			}
			execute_command(executor->cmd, data, curr_end);
			exit(0); // Ensure child process exits after command execution
		}
		else
		{
			close(curr_end[1]);
			if (executor->in != STDIN_FILENO)
				close(executor->in);
			executor->in = curr_end[0];
			wait(NULL);
		}
		executor = executor->next;
	}
	return (0);
}

// int	execution(t_executor *executor, t_data *data)
// {
// 	int		curr_end[2];
// 	int		pid;

// 	while (executor)
// 	{
// 		pipe(curr_end); // Create pipe for current command
// 		pid = fork();
// 		if (pid < 0)
// 		{
// 			perror("fork error");
// 			return 1;
// 		}
// 		else if (pid == 0)
// 		{
// 			if (executor->next)
// 			{
// 				close(curr_end[0]);
// 				dup2(curr_end[1], STDOUT_FILENO); // Redirect stdout to write end of current pipe
// 				close(curr_end[1]);
// 			}

// 			// Check for input redirection '<' in the command
// 			char *input_file = NULL;
// 			if ((input_file = strchr(executor->cmd, '<')) != NULL)
// 			{
// 				// Extract input file name
// 				input_file = strtok(input_file + 1, " ");
// 				ft_putstr_fd(input_file, 2);
// 				// input_file = strtok(NULL, " ");
// 				if (input_file != NULL)
// 				{
// 					int input_fd = open(input_file, O_RDONLY);
// 					if (input_fd == -1)
// 					{
// 						perror("Error opening input file");
// 						exit(1); // Exit child process with an error
// 					}
// 					dup2(input_fd, STDIN_FILENO); // Redirect stdin
// 					close(input_fd); // Close input file descriptor
// 				}
// 			}

// 			// Check for output redirection '>' in the command
// 			char *output_file = NULL;
// 			if ((output_file = strchr(executor->cmd, '>')) != NULL)
// 			{
// 				// Extract output file name
// 				output_file = strtok(output_file + 1, " ");
// 				// output_file = strtok(NULL, " ");
// 				if (output_file != NULL)
// 				{
// 					int output_fd = open(output_file, O_WRONLY | O_CREAT | O_TRUNC, 0644);
// 					if (output_fd == -1)
// 					{
// 						perror("Error opening output file");
// 						exit(1); // Exit child process with an error
// 					}
// 					dup2(output_fd, STDOUT_FILENO); // Redirect stdout
// 					close(output_fd); // Close output file descriptor
// 				}
// 			}

// 			execute_command(executor->cmd, data, curr_end);
// 			exit(0); // Ensure child process exits after command execution
// 		}
// 		else
// 		{
// 			close(curr_end[1]);
// 			if (executor->in != STDIN_FILENO)
// 				close(executor->in);
// 			executor->in = curr_end[0];
// 			wait(NULL);
// 		}
// 		executor = executor->next;
// 	}
// 	return 0;
// }

void	redir_and_execute(t_data *data, t_executor *executor)
{
	if (is_redir(data->lexer_list))
		redir(data);
	execution(executor, data);
}

void redir(t_data *data)
{
	char	**tokens;
	int		i;

	tokens = ft_split(data->cmd, ' ');
	i = 0;
	while (tokens[i])
	{
		if (!ft_strcmp(tokens[i], "<"))
		{
			data->executor->in = open(tokens[i + 1], O_RDONLY);
			if (data->executor->in == -1)
				perror("Error opening input file");
		}
		else if (!ft_strcmp(tokens[i], ">"))
		{
			data->executor->out = open(tokens[i + 1], O_CREAT | O_WRONLY
					| O_TRUNC, 0644);
			if (data->executor->out == -1)
				perror("Error opening output file");
		}
		else if (!ft_strcmp(tokens[i], ">>"))
		{
			data->executor->out = open(tokens[i + 1], O_CREAT | O_WRONLY
					| O_APPEND, 0644);
			if (data->executor->out == -1)
				perror("Error opening output file");
		}
		i++;
	}
	free_array(tokens);
}

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

char	*remove_redir_or_files(char *cmd)
{
	int		i;
	int		j;
	char	*dest;

	j = 0;
	i = 0;
	dest = (char *)malloc(sizeof(char ) * (ft_strlen(cmd) + 1));
	while (cmd[i])
	{
		if ((cmd[i] == '<' || cmd[i] == '>') && cmd[i + 1] == ' ')
		{
			i++;
			if (cmd[i] == ' ' && cmd[i])
				i++;
			while (cmd[i] && cmd[i] != ' ')
				i++;
		}
		dest[j++] = cmd[i++];
	}
	dest[j] = '\0';
	return (dest);
}
