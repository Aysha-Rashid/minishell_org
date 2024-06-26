/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ayal-ras <ayal-ras@student.42abudhabi.a    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/10 15:27:38 by ayal-ras          #+#    #+#             */
/*   Updated: 2024/03/07 20:04:52 by ayal-ras         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

t_env	*duplicate_node(char *str)
{
	t_env	*new_node;
	size_t	key_length;
	char	*equal_sign_position;

	new_node = malloc(sizeof(t_env));
	if (!new_node)
		return (NULL);
	equal_sign_position = ft_strchr(str, '=');
	if (!equal_sign_position || equal_sign_position == str)
		return (free(new_node), NULL);
	key_length = equal_sign_position - str;
	new_node->key = malloc(key_length + 1);
	if (!new_node->key)
		return (free(new_node), NULL);
	ft_strncpy(new_node->key, str, key_length);
	new_node->key[key_length] = '\0';
	new_node->value = ft_strdup(str);
	new_node->next = NULL;
	return (new_node);
}

t_env	*allocate_env(char **env)
{
	t_env	*head;
	t_env	*temp;
	t_env	*node;
	int		i;

	i = 0;
	temp = NULL;
	head = NULL;
	node = NULL;
	while (env[i])
	{
		node = duplicate_node(env[i]);
		if (!node)
		{
			free_env_list(head);
			return (NULL);
		}
		if (!head)
			head = node;
		else
			temp->next = node;
		temp = node;
		i++;
	}
	return (head);
}

int	ft_env(t_data *data, char *str)
{
	t_env	*temp;

	temp = data->envp;
	if (!data->envp || data->no_path)
		return (name_error(str, NULL, "No such file or directory", 0), 1);
	if (ft_strlen(str) != 3)
		return (name_error(str, NULL, "command not found", 0));
	if (temp)
	{
		while (temp)
		{
			if (ft_strchr(temp->value, '='))
				ft_putendl_fd(temp->value, 1);
			temp = temp->next;
		}
		return (0);
	}
	else
		return (1);
}

int	free_env_list(t_env *head)
{
	t_env	*temp;

	temp = head;
	while (head)
	{
		temp = head;
		head = head->next;
		free(temp->key);
		free(temp->value);
		free(temp);
	}
	return (0);
}
