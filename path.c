/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   path.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ayal-ras <ayal-ras@student.42abudhabi.a    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/09 14:03:16 by ayal-ras          #+#    #+#             */
/*   Updated: 2024/02/23 12:44:09 by ayal-ras         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

char	*cmd_file(char	*cmd, char **paths)
{
	char	*cmd_file;
	int		i;

	if (!cmd)
		return (NULL);
	cmd_file = given_path(cmd);
	if (cmd_file)
		return (cmd_file);
	paths = paths_add_slash(paths);
	if (!paths)
		return (NULL);
	i = 0;
	while (paths[i])
	{
		cmd_file = ft_strjoin(paths[i], cmd);
		if (access(cmd_file, F_OK | X_OK) == 0)
		{
			free_path(paths);
			return (cmd_file);
		}
		free(cmd_file);
		i++;
	}
	free_path(paths);
	return (NULL);
}

void	free_path(char **path)
{
	int	i;

	i = 0;
	while (path[i])
		free(path[i++]);
	free(path);
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

char	**paths_add_slash(char **env)
{
	int		i;
	char	**paths;
	char	*new_path;

	paths = find_paths_and_split(env);
	if (!paths)
		return (NULL);
	i = 0;
	while (paths[i])
	{
		new_path = ft_strjoin(paths[i], "/");
		free(paths[i]);
		paths[i] = new_path;
		i++;
	}
	return (paths);
}

char	*given_path(char *cmd)
{
	int	i;

	i = 0;
	while (cmd[i++])
	{
		if (cmd[i] == '/')
		{
			if (access(cmd, F_OK | X_OK) == 0)
				return (cmd);
			return (NULL);
		}
	}
	return (NULL);
}
