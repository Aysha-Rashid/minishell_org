/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ayal-ras <ayal-ras@student.42abudhabi.a    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/20 19:53:44 by ayal-ras          #+#    #+#             */
/*   Updated: 2024/02/20 22:24:05 by ayal-ras         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	parse_env(t_data *data, char **env)
{
	int		i;
	char	*temp;

	data->commands->path = find_paths_and_split(env);
	data->envp = allocate_env(env);
	i = 0;
	while (data->commands->path[i])
	{
		if (ft_strncmp(&data->commands->path[i]
				[ft_strlen(data->commands->path[i]) - 1], "/", 1) != 1)
		{
			temp = ft_strjoin(data->commands->path[i], "/");
			free(data->commands->path[i]);
			data->commands->path[i] = temp;
		}
		i++;
	}
	return (0);
}

char	**find_paths_and_split(char **envp)
{
	char	*envp_path;
	char	**paths;
	int		i;

	if (!envp)
		return (NULL);
	envp_path = NULL;
	i = 0;
	while (envp[i])
	{
		if (ft_strncmp(envp[i], "PATH=", 5) == 0)
		{
			envp_path = ft_substr(envp[i], 5, ft_strlen(envp[i]) - 5);
			break ;
		}
		i++;
	}
	paths = ft_split(envp_path, ':');
	free(envp_path);
	if (!paths)
	{
		free(paths);
		return (NULL);
	}
	return (paths);
}
