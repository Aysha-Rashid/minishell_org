/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   unset.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ayal-ras <ayal-ras@student.42abudhabi.a    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/11 21:03:21 by ayal-ras          #+#    #+#             */
/*   Updated: 2024/02/25 19:13:52 by ayal-ras         ###   ########.fr       */
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

int	check_unset_arg(char *str, char *token, t_data *data, int size_of_env)
{

	if (ft_strncmp(str, token, size_of_env) == 0)
	{
		if (!data->envp)
			return (0);
		data->no_path = 1;
		// free_env_list(data->envp);
		// data->envp = NULL;
		return (0);
	}
	return (1);
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

int	ft_unset(char *str, t_data *data)
{
	char	**token;
	t_env	*current;
	t_env	*prev;
	t_env	*remove;

	prev = NULL;
	current = data->envp;
	token = ft_split(str, ' ');
	if (ft_strlen(token[0]) != 5)
		return (free_array(token), 1);
	remove = search_env_variable(data->envp, token[1]);
	if (!validate_input(token, current, "unset"))
		return (0);
	else if (!remove || check_unset_arg("PATH", token[1], data,
			len_of_values(current)) == 0)
		return (free_array(token));
	while (current && current != remove)
	{
		prev = current;
		current = current->next;
	}
	if (remove_env_variable(data, remove, prev) || ft_strncmp(current->key, token[1], len_of_values(data->envp)))
		return (0);
	return (1);
}
