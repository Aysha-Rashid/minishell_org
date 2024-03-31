/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirection.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ayal-ras <ayal-ras@student.42abudhabi.a    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/20 16:59:09 by ayal-ras          #+#    #+#             */
/*   Updated: 2024/03/21 14:54:33 by ayal-ras         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	sig_handlers(int signum)
{
	signum++;
	g_signal = 2;
	exit(1);
}

int	redir(t_executor *executor)
{
	int		i;
	char	*file;

	i = 0;
	file = NULL;
	while (executor->cmd[i])
	{
		if ((executor->cmd[i] == '>' || executor->cmd[i] == '<')
			&& executor->cmd[i] != ' ')
		{
			i++;
			if (executor->cmd[i] == '>' || executor->cmd[i] == '<')
				i++;
			if (executor->cmd[i] == ' ' || executor->cmd[i] == '\t')
				i++;
			file = executor->cmd + i;
			if (ft_open(executor, executor->cmd, file))
				return (0);
		}
		i++;
	}
	return (1);
}

int	ft_open(t_executor *executor, char *redir, char *file)
{
	if (!ft_strncmp(redir, "<", 1) && ft_strncmp(redir, "<<", 2))
	{
		executor->in = open(file, O_RDONLY);
		if (executor->in == -1)
			return (printf("%s\n", strerror(errno)), 1);
	}
	else if (!ft_strncmp(redir, ">", 1) && ft_strncmp(redir, ">>", 2))
	{
		executor->out = open(file, O_CREAT | O_WRONLY
				| O_TRUNC, 0644);
		if (executor->out == -1)
			return (printf("%s\n", strerror(errno)), 1);
	}
	else if (!ft_strncmp(redir, ">>", 2))
	{
		executor->out = open(file, O_CREAT | O_WRONLY
				| O_APPEND, 0644);
		if (executor->out == -1)
			return (printf("%s\n", strerror(errno)), 1);
	}
	else
		return (0);
	return (0);
}

void	heredoc_loop(char *delimiter, t_executor *executor, int *end)
{
	char	*line;

	line = NULL;
	while (1)
	{
		g_signal = IN_HERE;
		// signal(SIGINT, ft_sig2);
		// signal(SIGQUIT, SIG_IGN);
		// ft_putnbr_fd(g_signal, 2);
		ft_putstr_fd("> ", STDOUT_FILENO);
		line = get_next_line(STDIN_FILENO);
		if (!ft_strcmp(line, delimiter))
		{
			free(line);
			break ;
		}
		if (executor->next)
			ft_putstr_fd(line, end[0]);
		free(line);
	}
}

int	heredoc(t_executor *executor, int *end, t_data *data)
{
	char	*temp;
	int		i;
	int		word_length;
	int		word_start;
	char	*delimiter;

	i = 0;
	temp = NULL;
	delimiter = NULL;
	while (data->cmd[i])
	{
		if (data->cmd[i] == '<' && data->cmd[i + 1] == '<')
		{
			i += 2;
			while ((data->cmd[i] == ' ' || data->cmd[i] == '\t')
				&& data->cmd[i])
				i++;
			word_start = i;
			while ((data->cmd[i] != ' ' && data->cmd[i] != '\t')
				&& data->cmd[i])
				i++;
			word_length = i - word_start;
			temp = ft_strndup(data->cmd + word_start, word_length);
			delimiter = ft_strjoin(temp, "\n");
			break ;
		}
		i++;
	}
	if (!delimiter)
		return (0);
	heredoc_loop(delimiter, executor, end);
	return (free(delimiter), 1);
}
