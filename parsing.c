/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ayal-ras <ayal-ras@student.42abudhabi.a    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/20 19:53:44 by ayal-ras          #+#    #+#             */
/*   Updated: 2024/02/25 19:46:47 by ayal-ras         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	parse_env(t_data *data, char **env)
{
	int		i;
	char	*temp;

	if (!env)
		return (0);
	data->no_path = 0;
	data->envp = allocate_env(env);
	data->commands->path = NULL;
	data->commands->path = find_paths_and_split(env);
	i = 0;
	while (data->commands->path[i])
	{
		if (ft_strncmp(&data->commands->path[i]
				[ft_strlen(data->commands->path[i]) - 1], "/", 1) != 1)
		{
			temp = ft_strjoin(data->commands->path[i], "/");
			free(data->commands->path[i]);
			data->commands->path[i] = temp;
			free(temp);
		}
		i++;
	}
	if (!data->commands->path)
		return (free_array(data->commands->path), 1);
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
	if (!envp_path)
		return (NULL);
	paths = ft_split(envp_path, ':');
	if (!paths)
		return (NULL);	
	free(envp_path);
	return (paths);
}

char	*env_str(t_env *env)
{
	char	*str;
	int		i;
	int		j;

	str = malloc(sizeof(char) * ((sizeof(size_t)) * len_of_values(env)) + 1);
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
