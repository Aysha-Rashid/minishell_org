/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ayal-ras <ayal-ras@student.42abudhabi.a    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/11 18:01:34 by ayal-ras          #+#    #+#             */
/*   Updated: 2024/02/22 22:08:58 by ayal-ras         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	sorted_env(char **env, size_t env_len)
{
	int		ordered;
	size_t	i;
	char	*tmp;

	ordered = 0;
	while (env && ordered == 0)
	{
		ordered = 1;
		i = 0;
		while (i < env_len - 1)
		{
			if (ft_strcmp(env[i], env[i + 1]) > 0)
			{
				tmp = env[i];
				env[i] = env[i + 1];
				env[i + 1] = tmp;
				ordered = 0;
			}
			i++;
		}
		env_len--;
	}
}

int	already_there(char *variable, t_env	*env)
{
	int	i;

	i = 0;
	if (!env)
		return (0);
	while (env)
	{
		if (ft_strncmp(variable, env->value, ft_strlen(env->key)) == 0)
		{
			free(env->value);
			free(env->key);
			env->value = ft_strdup(variable);
			env->key = ft_strndup(variable,
					ft_strchr(variable, '=') - variable);
			return (1);
		}
		env = env->next;
	}
	return (0);
}

int env_add(char *variable, t_env *env)
{
	char	*key;
	char	*value;
	t_env	*new;

	if (already_there(variable, env))
		return (0);
	if (!env)
		return (1);
	new = malloc(sizeof(t_env));
	if (!new)
		return (1);
	value = ft_strdup(variable);
	key = ft_strndup(variable, ft_strchr(variable, '=') - variable);
	while (env != NULL && env->next != NULL)
		env = env->next;
	new->key = key;
	new->value = value;
	new->next = NULL;
	env->next = new;
	return (0);
}

int	ft_export(char *str, t_data *data)
{
	// t_env	*env_duplicate;
	char	**token;

	// env_duplicate = duplicate_env(data->envp);
	token = ft_split(str, ' ');
	if (ft_strlen(token[0]) != 6)
	{
		free_array(token);
		return (1);
	}
	if (token[1] == NULL)
		return (declare_sorted(data->envp, 0));
	else
	{
		if (!validate_input(token, data->envp, "export"))
			return (0);	
		else if (!env_add(token[1], data->envp))
			return (0);
		return (1);
	}
	return (1);
}

int	declare_sorted(t_env *head, int flag)
{
	char	**temp;
	char	*str;
	int		i;
	size_t	env_len;

	str = env_str(head);
	temp = ft_split(str, '\n');
	free(str);
	env_len = size_of_env(temp);
	sorted_env(temp, env_len);
	i = 0;
	if (flag == 0)
	{
		while (temp[i])
		{
			ft_putstr_fd("declare -x ", 1);
			ft_putendl_fd(temp[i], 1);
			i++;
		}
	}
	free_array(temp);
	return (0);
}

char *env_str(t_env *env)
{
	char *str;
	int i;
	int j;

	i = 0;
	j = 0;
	str = malloc(sizeof(char) * len_of_values(env) + 1);
	if (!str)
		return (NULL);
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
