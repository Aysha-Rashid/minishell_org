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

int	redir(t_executor *executor)
{
	int		i;
	char	*file;
	int		start;
	char	redir[3];

	start = 0;
	i = 0;
	file = NULL;
	while (executor->cmd[i])
	{
		if ((executor->cmd[i] == '>' || executor->cmd[i] == '<')
			&& executor->cmd[i] != ' ')
		{
			redir[0] = executor->cmd[i++];
			if (executor->cmd[i] == '>' || executor->cmd[i] == '<')
				redir[1] = executor->cmd[i++];
			while (executor->cmd[i] == ' ' || executor->cmd[i] == '\t')
				i++;
			start = i;
			while (executor->cmd[i] != ' ' && executor->cmd[i] != '\t'
				&& executor->cmd[i])
				i++;
			file = ft_strndup(executor->cmd + start, i - start);
			if (ft_open(executor, redir, file))
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
