/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ayal-ras <ayal-ras@student.42abudhabi.a    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/11 18:01:34 by ayal-ras          #+#    #+#             */
/*   Updated: 2024/03/02 21:10:01 by ayal-ras         ###   ########.fr       */
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
		if (!(ft_strncmp(variable, current->key, ft_strlen(current->key))))
		{
			if (ft_strchr(variable, '='))
			{
				if (!(ft_strncmp("PATH=", variable, ft_strlen(current->value))))
					data->no_path = 1;
				free(current->key);
				free(current->value);
				current->value = ft_strdup(variable);
				current->key = ft_strndup(variable,
						ft_strchr(variable, '=') - variable);
			}
			return (1);
		}
		current = current->next;
	}
	return (0);
}

t_env	*duplicate_env(t_env *env)
{
	t_env	*head;
	t_env	*temp;

	temp = NULL;
	head = NULL;
	while (env != NULL)
	{
		if (head == NULL)
		{
			head = malloc(sizeof(t_env));
			temp = head;
		}
		else
		{
			temp->next = malloc(sizeof(t_env));
			temp = temp->next;
		}
		if (temp == NULL)
			return (NULL);
		temp->value = ft_strdup(env->value);
		temp->next = NULL;
		env = env->next;
	}
	return (head);
}

int	env_add(char *variable, t_data *env)
{
	char	*key;
	char	*value;
	t_env	*new;
	t_env	*temp;
	// t_env	*copy_env;

	// copy_env = duplicate_env(env->envp);
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
	if (ft_strchr(variable, '='))
	{
		new->key = key;
		new->value = value;
		new->next = NULL;
		new->path = NULL;
		temp = env->envp;
		while (temp->next != NULL)
			temp = temp->next;
		temp->next = new;
	}
	return (0);
}

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
			ft_putendl_fd(temp[i++], 1);
		}
	}
	return (free(str), free_array(temp), 1);
}

int	ft_export(char *str, t_data *data)
{
	char	**token;
	int		len;
	int		i;
	// t_env	*copy_env;

	// copy_env = duplicate_env(data->envp);
	token = ft_split(str, ' ');
	if (ft_strlen(token[0]) != 6)
		return (free_array(token), ft_error(2, str, NULL));
	len = size_of_env(token);
	i = 1;
	if (token[1] == NULL)
		return (declare_sorted(data->envp, 0), free_array(token));
	else if (!validate_input(token, data->envp, "export"))
		return (0);
	while (len > i)
	{
		if (already_there(token[i], data))
			i++;
		else
		{
			env_add(token[i], data);
			i++;
		}
	}
	return (free_array(token), 1);
}
