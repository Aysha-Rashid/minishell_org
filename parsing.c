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
	data->path = ft_split(temp, ':');
	free(temp);
	i = 0;
	if (!data->envp || !data->path)
	{
		free_env_list(data->envp);
		ft_error(2, "env", 1);
		exit(0);
	}
	while (data->path[i])
	{
		if (data->path[i][ft_strlen(data->path[i]) - 1] != '/')
		{
			temp = ft_strjoin(data->path[i], "/");
			free(data->path[i]);
			data->path[i] = temp;
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

char	*cmd_file(char *cmd, char **paths, t_data *data)
{
	char	*cmd_file;
	char	**str;
	char	*trimmed;

	if (!cmd || cmd[0] == '\0')
		return (NULL);
	trimmed = ft_strtrim(cmd, " ");
	if (!trimmed || trimmed[0] == '\0')
		return (NULL);
	str = ft_split(trimmed, ' ');
	free(trimmed);
	modified_based_quote(str);
	if (str[0] == NULL || str[0][0] == '\0')
		return (ft_error(2, str[0], data->no_path), free_array(str), NULL);
	cmd_file = NULL;
	if (given_path(str[0]))
	{
		execve(str[0], str, paths);
		failed_execve(cmd_file, str);
	}
	execute_binary_files(paths, str, cmd_file);
	return (ft_error(2, str[0], data->no_path), free_array(str), NULL);
}

char	*given_path(char *cmd)
{
	int	i;

	i = 0;
	while (cmd[i])
	{
		if (cmd[i] == '/')
		{
			if (access(cmd, F_OK | X_OK) == 0)
				return (cmd);
			return (NULL);
		}
		i++;
	}
	return (NULL);
}
