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
	// data->executor->in = 0;
	// data->executor->out = 0;
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

int	execution(t_executor *executor, t_data *data)
{
	int		end[2];
	int		pid;

	while (executor)
	{
		pipe(end);
		pid = fork();
		if (pid < 0)
			return (ft_putstr_fd("fork error", 1), 1);
		else if (pid == 0)
		{
			if (executor->next)
				ft_dup_fd(end);
			execute_command(executor->cmd, data, end);
		}
		else
		{
			if (executor->next)
				close(end[1]);
		}
		executor = executor->next;
		close(end[0]);
		close(end[1]);
		while (wait(&data->status_code) > 0);
	}
	return (0);
}

void	redir_and_execute(t_data *data, t_executor *executor)
{
	if (is_redir(data->lexer_list))
		redir(data);
	else
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
