/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ayal-ras <ayal-ras@student.42abudhabi.a    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/31 17:34:40 by ayal-ras          #+#    #+#             */
/*   Updated: 2024/03/31 22:56:56 by ayal-ras         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	heredoc_loop(char *delimiter, int fd)
{
	char	*line;
	line = NULL;
	while (1)
	{
		g_signal = IN_HERE;
		ft_putstr_fd("> ", STDOUT_FILENO);
		line = get_next_line(STDIN_FILENO);
		if (!ft_strcmp(line, delimiter))
		{
			free(line);
			break ;
		}
		ft_putstr_fd(line, fd);
		free(line);
	}
}

char	*find_delimiter(char *delimiter, t_data *data, int i, char *temp)
{
	int		word_start;

	word_start = 0;
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
			temp = ft_strndup(data->cmd + word_start, i - word_start);
			delimiter = ft_strjoin(temp, "\n");
			free(temp);
			break ;
		}
		i++;
	}
	return (delimiter);
}

int	heredoc(t_executor *executor, int *end, t_data *data)
{
	char	*temp;
	int		i;
	int		fd;
	char	*delimiter;

	i = 0;
	(void)end;
	temp = NULL;
	delimiter = NULL;
	delimiter = find_delimiter(delimiter, data, i, temp);
	if (!delimiter)
		return (0);
	fd = open(".heredoc_temp", O_CREAT | O_WRONLY | O_TRUNC, 0644);
	if (fd < 0)
		return (free(delimiter), 0);
	heredoc_loop(delimiter, fd);
	close(fd);
	executor->in = open(".heredoc_temp", O_RDONLY);
	if (executor->in < 0)
		return (free(delimiter), 0);
	return (free(delimiter), fd);
}
