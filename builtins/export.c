/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ayal-ras <ayal-ras@student.42abudhabi.a    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/11 18:01:34 by ayal-ras          #+#    #+#             */
/*   Updated: 2024/02/28 13:07:57 by ayal-ras         ###   ########.fr       */
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

int	already_there(char *variable, t_data *data)
{
	t_env	*current;

	current = data->envp;
	if (!data->envp)
		return (0);
	while (current)
	{
		if (ft_strncmp(variable, current->key, ft_strlen(current->key)) == 0)
		{
			if (!ft_strncmp("PATH=", variable, ft_strlen(current->key)))
				data->no_path = 1;
			free(current->key);
			free(current->value);
			current->value = ft_strdup(variable);
			if (!current->value)
				return (free(current->value), 1);
			current->key = ft_strndup(variable,
					ft_strchr(variable, '=') - variable);
			if (!current->key)
				return (free(current->key), 1);
			return (1);
		}
		current = current->next;
	}
	return (0);
}

int	env_add(char *variable, t_data *env)
{
	char	*key;
	char	*value;
	t_env	*new;
	t_env	*temp;

	if (!env->envp)
		return (1);
	value = ft_strdup(variable);
	if (!value)
		return (1);
	key = ft_strndup(variable, ft_strchr(variable, '=') - variable);
	if (!key)
		return (free(value), 1);
	new = malloc(sizeof(t_env));
	if (!new)
		return (1);
	new->key = key;
	new->value = value;
	new->next = NULL;
	new->path = NULL;
	temp = env->envp;
	while (temp->next != NULL)
		temp = temp->next;
	temp->next = new;
	return (0);
}

// int env_add(char *variable, t_env *env)
// {
// 	char	*key;
// 	char	*value;
// 	t_env	*new;

// 	if (already_there(variable, env))
// 		return (0);
// 	if (!env)
// 		return (1);
// 	new = malloc(sizeof(t_env));
// 	if (!new)
// 		return (1);
// 	value = ft_strdup(variable);
// 	key = ft_strndup(variable, ft_strchr(variable, '=') - variable);
// 	while (env != NULL && env->next != NULL)
// 		env = env->next;
// 	new->key = key;
// 	new->value = value;
// 	new->next = NULL;
// 	env->next = new;
// 	return (0);
// }

int	declare_sorted(t_env *head, int flag)
{
	char	**temp;
	char	*str;
	int		i;
	size_t	env_len;

	if (!head->value && head->next == NULL)
		return (0);
	str = env_str(head);
	if (!str)
		return (1);
	temp = ft_split(str, '\n');
	if (!temp)
		return (1);
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
	free(str);
	free_array(temp);
	return (0);
}

int	ft_export(char *str, t_data *data)
{
	char	**token;

	token = ft_split(str, ' ');
	if (ft_strlen(token[0]) != 6)
	{
		free_array(token);
		return (name_error(str, NULL, "command not found"));
	}
	if (token[1] == NULL)
		return (declare_sorted(data->envp, 0), free_array(token));
	else if (!validate_input(token, data->envp, "export"))
		return (0);
	else if (already_there(token[1], data))
		return (free_array(token));
	else if (!env_add(token[1], data))
		return (free_array(token));
	free_array(token);
	return (1);
}
