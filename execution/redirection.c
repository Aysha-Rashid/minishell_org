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

// void	heredoc(t_data *data, t_executor *executor, int *end)
// {
// 	if (!ft_strcmp(executor->cmd, "<<"))
// 	{
// 		// with pipe and redirection and without pipe and redirection
// 	}
// }

int	parse_command(char **token)
{
	int		i;
	char	*message;
	char	check;

	message = "syntax error near unexpected token ";
	i = 0;
	while (token[i] && token[i + 1])
	{
		check = ft_strchr(token[i + 1], '<') || ft_strchr(token[i + 1], '>')
			|| ft_strchr(token[i + 1], '|');
		// if (ft_strcmp(token[0], ">") && check)
		// 	return (name_error(NULL, message, token[i + 1], 0), 1); // if the greater then sign is in the beginning
		if (ft_strchr(token[i], '<') && check)
			return (name_error(NULL, message, token[i + 1], 0), 1);
		if ((!ft_strcmp(token[i], "<<") || !ft_strcmp(token[i], ">>")) && check)
			return (name_error(NULL, message, token[i + 1], 0), 1);
		if (!ft_strcmp(token[i], "|") && !ft_strcmp(token[i + 1], "|"))
			return (name_error(NULL, message, " `|'", 0), 1);
		i++;
	}
	if (token[i + 1] == NULL && !ft_strncmp(token[i], "||", 2))
		return (name_error(NULL, message, "||", 0), 1);
	if ((token[i + 1] == NULL) && (ft_strchr(token[i], '<')
			|| ft_strchr(token[i], '>') || ft_strchr(token[i], '|')))
		return (name_error(NULL, message, "`newline'", 0), 1);
	return (0);
}
