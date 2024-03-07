/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ayal-ras <ayal-ras@student.42abudhabi.a    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/20 19:53:44 by ayal-ras          #+#    #+#             */
/*   Updated: 2024/03/05 20:42:29 by ayal-ras         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	parse_env(t_data *data, char **env)
{
	int		i;
	char	*temp;

	data->envp = allocate_env(env);
	temp = find_paths_and_split(env);
	data->envp->path = ft_split(temp, ':');
	free(temp);
	i = 0;
	if (!data->envp || !data->envp->path)
	{
		data->no_path = 1;
		ft_error(2, "env", data);
		exit(0);
	}
	while (data->envp->path[i])
	{
		if (data->envp->path[i][ft_strlen(data->envp->path[i]) - 1] != '/')
		{
			temp = ft_strjoin(data->envp->path[i], "/");
			free(data->envp->path[i]);
			data->envp->path[i] = temp;
		}
		i++;
	}
	return (0);
}

char	*find_paths_and_split(char **envp)
{
	char	*envp_path;
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
	if (!envp_path)
		return (NULL);
	return (envp_path);
}

char	*env_str(t_env *env)
{
	char	*str;
	int		i;
	int		j;

	str = malloc(len_of_values(env) + 1);
	if (!str)
		return (NULL);
	i = 0;
	j = 0;
	while (env)
	{
		if (env->value != NULL)
		{
			while (env->value[j])
				str[i++] = env->value[j++];
		}
		if (env->next != NULL)
			str[i++] = '\n';
		env = env->next;
		j = 0;
	}
	str[i] = '\0';
	return (str);
}

char	*cmd_file(char	*cmd, char **paths)
{
	char	*cmd_file;
	int		i;

	if (!cmd)
		return (NULL);
	// cmd_file = given_path(cmd);
	// if (cmd_file)
	// 	return (cmd_file);
	// paths = paths_add_slash(paths);
	// if (!paths)
	// 	return (NULL);
	// write(1, "hello", 5);
	// ft_putendl_fd(*paths, 2);
	i = 0;
	while (paths[i])
	{
		cmd_file = ft_strjoin(paths[i], cmd);
		if (access(cmd_file, F_OK | X_OK) == 0)
		{
			free_array(paths);
			return (cmd_file);
		}
		free(cmd_file);
		i++;
	}
	free_array(paths);
	return (NULL);
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
