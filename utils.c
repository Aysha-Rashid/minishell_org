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
	free(cmd_file);
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
