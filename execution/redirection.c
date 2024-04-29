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
		executor->in = open_and_check(executor->in, file, 1);
		if (executor->in == -1)
			return (1);
	}
	if (!ft_strncmp(redir, ">", 1) && ft_strncmp(redir, ">>", 2))
	{
		executor->out = open_and_check(executor->out, file, 2);
		if (executor->out == -1)
			return (1);
	}
	if (!ft_strncmp(redir, ">>", 2))
	{
		executor->out = open_and_check(executor->out, file, 3);
		if (executor->out == -1)
			return (1);
	}
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
		{
			dest[j++] = cmd[i++];
		}
	}
	dest[j] = '\0';
}

char	*remove_redir_or_files(char *cmd)
{
	char	*dest;
	int		i;
	// char	quote;
	i = 0;
	// if ((cmd[0] == '\'' || cmd[0] == '\"') && (ft_strchr(cmd, '<') || ft_strchr(cmd, '>')))
	// 	return (cmd);
	// quote = '\0';
	// while (cmd[i])
	// {
	// 	if (cmd[i] == '\'' || cmd[i] == '"')
	// 	{
	// 		if (quote == '\0')
	// 			quote = cmd[i];
	// 		else if (quote == cmd[i])
	// 			quote = '\0';
	// 	}
	// 	else if ((quote == '\'' || quote == '\"') && (cmd[i] == '<' || cmd[i] == '>'))
	// 	{
	// 		ft_putendl_fd("here", 2);
	// 		return cmd;
	// 	}
	// 	i++;
	// }
	// i = 0;
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
	if (ft_open(executor, redir, file) == 1)
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
	if (!temp)
		return (NULL);
	while (temp[i] != NULL)
	{
		if (ft_strchr(temp[i], '<') || ft_strchr(temp[i], '>'))
		{
			redir = ft_strdup(temp[i]);
			return (free_array(temp), redir);
		}
		i++;
	}
	return (free_array(temp), NULL);
}
