/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ayal-ras <ayal-ras@student.42abudhabi.a    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/20 19:53:44 by ayal-ras          #+#    #+#             */
/*   Updated: 2024/03/04 15:54:27 by ayal-ras         ###   ########.fr       */
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

char	*dup_env_str(t_dupenv *env)
{
	char	*str;
	int		i;
	int		j;

	str = malloc(dup_len_of_values(env) + 1);
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
		// ft_putendl_fd(env->value, 1);
		env = env->next;
		j = 0;
	}
	str[i] = '\0';
	return (str);
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
