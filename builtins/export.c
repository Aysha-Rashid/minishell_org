/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ayal-ras <ayal-ras@student.42abudhabi.a    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/11 18:01:34 by ayal-ras          #+#    #+#             */
/*   Updated: 2024/03/11 11:05:28 by ayal-ras         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	sorted_env(char **env)
{
	size_t	i;
	size_t	env_len;
	int		ordered;
	char	*tmp;

	ordered = 0;
	env_len = size_of_env(env);
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
		if (!(ft_strncmp(current->key, variable, ft_strlen(current->key))))
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

int	env_add(char *variable, t_data *env)
{
	char	*key;
	char	*value;
	t_env	*new;
	t_env	*temp;

	if (ft_strcmp(variable, "PATH=") == 0)
	{
		env->no_path = 1;
		return (0);
	}
	value = ft_strdup(variable);
	if (!value)
		return (1);
	key = ft_strndup(variable, ft_strchr(variable, '=') - variable);
	if (!key)
		return (free(value), 1);
	new = malloc(sizeof(t_env));
	if (!new)
		return (1);
	if (env->envp == NULL)
	{
		env->envp = new;
		temp = new;
	}
	else
	{
		temp = env->envp;
		while (temp->next != NULL)
			temp = temp->next;
	}
	new->key = key;
	new->value = value;
	temp->next = new;
	new->next = NULL;
	// new->path = NULL;
	return (0);
}

int	declare_sorted(t_env *head)
{
	char	**temp;
	char	*str;
	int		i;

	if (!head)
		return (1);
	str = env_str(head);
	temp = ft_split(str, '\n');
	if (!temp || !str)
		return (1);
	sorted_env(temp);
	i = 0;
	if (!temp)
		return (1);
	while (temp[i])
	{
		ft_putstr_fd("declare -x ", 1);
		if (ft_strchr(temp[i], '='))
			print_after_equal(temp[i]);
		else
			ft_putendl_fd(temp[i], 1);
		i++;
	}
	return (free(str), free_array(temp), 1);
}

int	ft_export(char *str, t_data *data)
{
	char	**token;
	int		i;

	// if (data->envp == NULL)
	// 	return (0);
	token = ft_split(str, ' ');
	if (!token)
		return (1);
	if (ft_strlen(token[0]) != 6)
		return (free_array(token), ft_error(2, str, 0));
	i = 1;
	if (token[1] == NULL)
		return (declare_sorted(data->envp), free_array(token));
	while (token[i])
	{
		if (!already_there(token[i], data)
			&& validate_input(data, token[i], data->envp, "export"))
			{
				// ft_putendl_fd("here", 2);
				env_add(token[i], data);
			}
		i++;
	}
	return (free_array(token), 0);
}
