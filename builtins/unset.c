/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   unset.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ayal-ras <ayal-ras@student.42abudhabi.a    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/11 21:03:21 by ayal-ras          #+#    #+#             */
/*   Updated: 2024/02/29 21:15:29 by ayal-ras         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	invalid_identifier(char **str_arr, char *name)
{
	char	*str;
	int		error;

	error = 0;
	while (*str_arr != NULL)
	{
		str = *str_arr;
		if (!ft_isalpha(str[0]) && str[0] != '_')
		{
			name_error(name, str, " : not a valid identifier");
			error = 1;
		}
		str_arr++;
	}
	if (error == 1)
		return (1);
	return (0);
}

int	validate_input(char **token, t_env *current, char *name)
{
	if (ft_strcmp(name, "unset"))
	{
		if (!token[1] || invalid_identifier(token, name) || !current)
		{
			free_array(token);
			free_env_list(current);
			return (0);
		}
	}
	else
	{
		if (invalid_identifier(token, name) || !current)
		{
			free_array(token);
			free_env_list(current);
			return (0);
		}
	}
	return (1);
}

int	remove_env_variable(t_data *data, t_env *to_remove, t_env *prev)
{
	if (to_remove)
	{
		if (prev->next)
			prev->next = to_remove->next;
		else
			data->envp = to_remove->next;
		if (data->envp && !data->envp->key)
			return (1);
		free(to_remove->value);
		free(to_remove->key);
		free(to_remove);
		return (1);
	}
	return (0);
}

int	ft_unset(char *str, t_data *data)
{
	char	**token;
	t_env	*current;
	int		len;

	current = data->envp;
	token = ft_split(str, ' ');
	if (ft_strlen(token[0]) != 5)
		return (free_array(token), 1);
	len = size_of_env(token);
	if (!validate_input(token, current, "unset"))
		return (0);
	if (!unset_loop(data, current, len, token))
		return (0);
	return (1);
}

int	unset_loop(t_data *data, t_env *current, int len, char **token)
{
	int		i;
	t_env	*remove;
	t_env	*prev;

	i = 1;
	prev = NULL;
	while (len > i)
	{
		remove = search_env_variable(data->envp, token[i]);
		current = data->envp;
		prev = NULL;
		while (current && current != remove)
		{
			check_unset_arg("PATH", token[i], data, len_of_values(current));
			check_unset_arg("PATH=", token[i], data, len_of_values(current));
			prev = current;
			current = current->next;
		}
		if (!(remove_env_variable(data, remove, prev))
			|| (remove && ft_strncmp(remove->key, token[i],
					len_of_values(data->envp))))
			return (0);
		i++;
	}
	return (1);
}
