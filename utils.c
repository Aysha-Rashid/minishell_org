/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ayal-ras <ayal-ras@student.42abudhabi.a    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/02 20:06:13 by ayal-ras          #+#    #+#             */
/*   Updated: 2024/04/02 20:16:28 by ayal-ras         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	failed_execve(char *cmd_file, char **str)
{
	(void)cmd_file;
	free_array(str);
	ft_error(2, str[0], 1);
	exit(1);
}

void	execute_binary_files(char **paths, char **str, char *cmd_file)
{
	int	i;

	i = 0;
	while (paths[i])
	{
		cmd_file = ft_strjoin(paths[i], str[0]);
		if (!(access(cmd_file, F_OK | X_OK)))
		{
			execve(cmd_file, str, paths);
			failed_execve(cmd_file, str);
		}
		free(cmd_file);
		i++;
	}
}

void	modified_based_quote(char **str)
{
	int		i;
	char	*modified_str;

	i = 0;
	modified_str = NULL;
	while (str[i])
	{
		if (ft_strchr(str[i], '\'') || ft_strchr(str[i], '\"'))
		{
			modified_str = remove_quotes(str[i]);
			if (!modified_str)
			{
				free_array(str);
				return ;
			}
			free(str[i]);
			str[i] = modified_str;
		}
		i++;
	}
}

int	open_and_check(int struct_file, char *file_name, int flag)
{
	if (flag == 1)
	{
		struct_file = open(file_name, O_RDONLY);
		if (struct_file == -1)
			return (printf("%s\n", strerror(errno)), 1);
	}
	if (flag == 2)
	{
		struct_file = open(file_name, O_CREAT | O_WRONLY | O_TRUNC, 0644);
		if (struct_file == -1)
			return (printf("%s\n", strerror(errno)), 1);
	}
	if (flag == 3)
	{
		struct_file = open(file_name, O_CREAT | O_WRONLY | O_APPEND, 0644);
		if (struct_file == -1)
			return (printf("%s\n", strerror(errno)), 1);
	}
	return (struct_file);
}

void	close_exec_files(int fdin, int fdout)
{
	if (fdin > 2)
		close(fdin);
	if (fdout > 2)
		close(fdout);
}