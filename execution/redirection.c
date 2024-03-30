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

char	*remove_redir_or_files(char *cmd)
{
	char	*dest;
	int		i;
	int		j;

	i = 0;
	j = 0;
	dest = (char *)malloc(sizeof(char) * (ft_strlen(cmd) + 1));
	if (!dest)
		return (NULL);
	while (cmd[i])
	{
		if ((cmd[i] == '<' || cmd[i] == '>') && cmd[i] != ' ')
		{
			i++;
			if (cmd[i] == '>' || cmd[i] == '<')
				i++;
			while (cmd[i] && (cmd[i] == ' ' || cmd[i] == '\t'))
				i++;
			while (cmd[i] && cmd[i] != ' ' && cmd[i] != '\t')
				i++;
		}
		dest[j++] = cmd[i++];
	}
	dest[j] = '\0';
	return (dest);
}

int	redir(t_executor *executor)
{
	int		i;
	char	*file;

	i = 0;
	file = NULL;
	while(executor->cmd[i])
	{
		if ((executor->cmd[i] == '>' || executor->cmd[i] == '<') && executor->cmd[i] != ' ')
		{
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
		signal(SIGINT, sig_handlers);
	while (1)
	{
		ft_putstr_fd("> ", STDOUT_FILENO);
		line = get_next_line(STDIN_FILENO);
		if (!ft_strcmp(line, delimiter))
		{
			free(line);
			break ;
		}
		if (executor->next)
			ft_putstr_fd(line, end[1]);
		free(line);
	}
}

int	heredoc(t_executor *executor, int *end, t_data *data)
{
	char	*temp;
	int		i;
	char	*delimiter;

	i = 0;
	temp = NULL;
	delimiter = NULL;
	while (data->cmd[i])
	{
		if (data->cmd[i] == '<' && data->cmd[i + 1] == '<')
		{
			i += 2;
			if ((data->cmd[i] == ' ' || data->cmd[i] == '\t') && data->cmd[i])
				i++;
			temp = data->cmd + i;
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

void remove_whitespace(char *str)
{
	int	i;

	i = 0;
	while (str[i] && (str[i] == ' ' || str[i] == '\t'))
		i++;
}

int contains_special_chars(const char *str) {
    while (*str) {
        if (*str == '<' || *str == '>' || *str == '|') {
            return 1;
        }
        str++;
    }
    return 0;
}

int	parse_command(char **token)
{
	int	i;
	int	len;
	char	*message;

	message = "syntax error near unexpected token ";
	i = 0;
	while (token[i])
	{
		len = ft_strlen(token[i]);
		if (token[i + 1])
		{
			if (contains_special_chars(token[i]) && contains_special_chars(token[i + 1]))
			    return (name_error(NULL, message, token[i], 0), 1);
		}
		if (token[i + 1] == NULL && (token[i][len - 1] == '>' || token[i][len - 1] == '<' || token[i][len - 1] == '|'))
			return (name_error(NULL, message, "`newline'", 0), 1);
        i++;
    }
	return (0);
}
