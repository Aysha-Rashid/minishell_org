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

// int	is_redir(char *cmd)
// {
// 	if (ft_strchr(cmd, '<') || ft_strchr(cmd, '>'))
// 		return (1);
// 	return (0);
// }

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
			if (cmd[i] == '<' || cmd[i] == '>')
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

void	redir(t_executor *executor)
{
	char	**tokens;
	int		i;

	i = 0;
	tokens = ft_split(executor->cmd, ' ');
	while (tokens[i])
	{
		ft_open(executor, tokens[i], tokens[i + 1]);
		i++;
	}
	free_array(tokens);
}

int	ft_open(t_executor *executor, char *redir, char *file)
{
	if (!ft_strcmp(redir, "<"))
	{
		executor->in = open(file, O_RDONLY);
		if (executor->in == -1)
			return (perror("Error opening input file"), 1);
	}
	else if (!ft_strcmp(redir, ">"))
	{
		executor->out = open(file, O_CREAT | O_WRONLY
				| O_TRUNC, 0644);
		if (executor->out == -1)
			return (perror("Error opening output file"), 1);
	}
	else if (!ft_strcmp(redir, ">>"))
	{
		executor->out = open(file, O_CREAT | O_WRONLY
				| O_APPEND, 0644);
		if (executor->out == -1)
			return (perror("Error opening output file"), 1);
	}
	return (0);
}

// void	count_pipes(t_data *data)
// {
// 	int	i;

// 	i = 0;
// 	while (data->cmd[i])
// 	{
// 		if (data->cmd[i] == '|')
// 			data->executor->pipes++;
// 		i++;
// 	}
// }
