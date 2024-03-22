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
	// data->executor->pipes = 0;
	data->executor->in = STDIN_FILENO;
	data->executor->out = STDOUT_FILENO;
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
	// close(data->executor->out);
	// close(data->executor->in);
	exit(1);
}

void	child_process(t_data *data, t_executor *executor, int *end)
{
	// executor->cmd = remove_redir_or_files(executor->cmd);
	// heredoc
	// ft_putendl_fd(executor->cmd, 2);
	if (executor->next)
		ft_dup_fd(data, executor, end, 1);
	else
		ft_dup_fd(data, executor, end, 0);
	execute_command(executor->cmd, data, end);
}

int	execution(t_executor *executor, t_data *data)
{
	int	end[2];
	int	pid;

	while (executor)
	{
		redir(executor);
		executor->cmd = remove_redir_or_files(executor->cmd);
		if (executor->next)
			pipe(end);
		pid = fork();
		if (pid < 0)
			return (perror("fork error"), 0);
		else if (pid == 0)
			child_process(data, executor, end);
		executor = executor->next;
		// ft_putnbr_fd(end[0], 2);
		// ft_putnbr_fd(end[1], 2);
		waitpid(pid , &data->status_code, 0);
	}
	// wait(NULL);
	ft_putstr_fd("comes here", 2);
	return (0);
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

char	*remove_quotes(char *str)
{
	int		i;
	char	quote;
	int		len;
	char	*ptr;
	int		j;

	i = 0;
	j = 0;
	len = ft_strlen(str);
	ptr = malloc(len + 1);
	while (i < len)
	{
		if (str[i] == '\'' || str[i] == '"')
		{
			quote = str[i++];
			while (ft_isalpha(str[i]) && i < len && str[i] != quote)
				ptr[j++] = str[i++];
			if (i < len)
				i++;
		}
		else
			ptr[j++] = str[i++];
	}
	ptr[j] = '\0';
	return (ptr);
}
