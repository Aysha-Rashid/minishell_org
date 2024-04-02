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

int	ft_open(t_executor *executor, char *redir, char *file)
{
	if (!ft_strncmp(redir, "<", 1) && ft_strncmp(redir, "<<", 2))
	{
		executor->in = open(file, O_RDONLY);
		if (executor->in == -1)
			return (printf("%s\n", strerror(errno)), 1);
	}
	if (!ft_strncmp(redir, ">", 1) && ft_strncmp(redir, ">>", 2))
	{
		executor->out = open(file, O_CREAT | O_WRONLY
				| O_TRUNC, 0644);
		if (executor->out == -1)
			return (printf("%s\n", strerror(errno)), 1);
	}
	if (!ft_strncmp(redir, ">>", 2))
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

void	remov_redir_and_files(char *cmd, char *dest, int i)
{
	int	j;

	j = 0;
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
		else
			dest[j++] = cmd[i++];
	}
	dest[j] = '\0';
}

char	*remove_redir_or_files(char *cmd)
{
	char	*dest;
	int		i;

	i = 0;
	if ((ft_strchr(cmd, '<') || ft_strchr(cmd, '>'))
		&& (cmd[0] == '\'' || cmd[0] == '\"'))
		return (cmd);
	dest = (char *)malloc(sizeof(char) * (ft_strlen(cmd) + 1));
	if (!dest)
		return (NULL);
	remov_redir_and_files(cmd, dest, i);
	return (dest);
}

int	open_files(char *cmd, char *redir, int i, t_executor *executor)
{
	int		start;
	char	*file;

	start = 0;
	file = NULL;
	while (cmd[i] == '>' || cmd[i] == '<')
		i++;
	while (cmd[i] == ' ' || cmd[i] == '\t')
		i++;
	start = i;
	while (cmd[i] != ' ' && cmd[i] != '\t'
		&& cmd[i])
		i++;
	file = strndup(cmd + start, i - start);
	if (ft_open(executor, redir, file))
		return (free(file), 0);
	return (free(file), 1);
}

char	*get_redir_and_files(char *cmd)
{
	char	*redir;
	int		i;
	char	**temp;

	i = 0;
	redir = NULL;
	temp = ft_split(cmd, ' ');
	while (temp[i])
	{
		if (ft_strchr(temp[i], '<') || ft_strchr(temp[i], '>'))
			redir = ft_strdup(temp[i]);
		i++;
	}
	return (free_array(temp), redir);
}
