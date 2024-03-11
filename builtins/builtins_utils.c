/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins_utils.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ayal-ras <ayal-ras@student.42abudhabi.a    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/20 19:47:18 by ayal-ras          #+#    #+#             */
/*   Updated: 2024/03/11 12:15:30 by ayal-ras         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

t_env	*search_env_variable(t_env *envp, char *key)
{
	while (envp)
	{
		if (ft_strcmp(envp->key, key) == 0)
			return (envp);
		envp = envp->next;
	}
	return (NULL);
}

int	free_array(char **str)
{
	int	i;

	i = 0;
	while (str[i])
		free(str[i++]);
	free(str);
	return (0);
}

size_t	len_of_values(t_env *lst)
{
	t_env	*current;
	int		total_length;

	total_length = 0;
	current = lst;
	while (current)
	{
		if (current->value)
			total_length += ft_strlen(current->value);
		if (current->next)
			total_length++;
		current = current->next;
	}
	return (total_length);
}

size_t	size_of_env(char **head)
{
	int	i;

	i = 0;
	while (head[i])
		i++;
	return (i);
}

int	name_error(char *name, char *str, char *message)
{
	ft_putstr_fd("minishell: ", STDERR_FILENO);
	ft_putstr_fd(name, STDERR_FILENO);
	ft_putstr_fd(": ", STDERR_FILENO);
	ft_putstr_fd(str, STDERR_FILENO);
	ft_putendl_fd(message, STDERR_FILENO);
	return (0);
}
