/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ayal-ras <ayal-ras@student.42abudhabi.a    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/20 19:53:44 by ayal-ras          #+#    #+#             */
/*   Updated: 2024/03/15 12:00:09 by ayal-ras         ###   ########.fr       */
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
		free_env_list(data->envp);
		ft_error(2, "env", 1);
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

char	*cmd_file(char *cmd, char **paths)
{
	char	*cmd_file;
	char	*temp;
	char	**str;
	int		i;
	if (!cmd)
		return (NULL);

	i = 0;
	temp = remove_all_qoutes(cmd);
	str = ft_split(temp, ' ');
	cmd_file = NULL;
	if (given_path(str[0]))
	{
		execve(cmd, str, paths);
		return (ft_error(2, str[0], 1), free_array(str), NULL);
	}
	while (paths[i])
	{
		cmd_file = ft_strjoin(paths[i], str[0]);
		if (!(access(cmd_file, F_OK | X_OK)))
		{
			// free(data->executor->cmd);
			free(cmd);
			execve(cmd_file, str, paths);
			free(cmd_file);
			free_array(str);
			exit(1);
		}
		free(cmd_file);
		i++;
	}
	return (ft_error(2, str[0], 0), free_array(str), NULL);
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
