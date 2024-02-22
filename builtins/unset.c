/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   unset.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ayal-ras <ayal-ras@student.42abudhabi.a    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/11 21:03:21 by ayal-ras          #+#    #+#             */
/*   Updated: 2024/02/22 20:36:26 by ayal-ras         ###   ########.fr       */
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
			name_error(name);
			ft_putstr_fd(str, STDERR_FILENO);
			ft_putendl_fd(": not a valid identifier", STDERR_FILENO);
			error = 1;
		}
		str_arr++;
	}
	if (error != 0)
		return (1);
	return (0);
}

int	check_unset_arg(char *str, char *token, t_data *data, int size_of_env)
{
	if (ft_strncmp(str, token, size_of_env) == 0)
	{
		if (!data->envp)
			return (0);
		free_env_list(data->envp);
		data->envp = NULL;
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
		// if (alread_there(token, current))
		// 	retrun (0);
		if (invalid_identifier(token, name) || !current)
		{
			free_array(token);
			free_env_list(current);
			return (0);
		}
	}
	return (1);
}

void	remove_env_variable(t_data *data, t_env *to_remove, t_env *prev)
{
	if (prev)
		prev->next = to_remove->next;
	else
		data->envp = to_remove->next;
	if (data->envp && !data->envp->key)
		return ;
	free(to_remove->key);
	free(to_remove->value);
	free(to_remove);
	return ;
}

int	ft_unset(char *str, t_data *data)
{
	char	**token;
	t_env	*current;
	t_env	*prev;
	t_env	*remove;

	prev = NULL;
	token = ft_split(str, ' ');
	if (ft_strlen(token[0]) != 5)
		return (1);
	current = data->envp;
	if (!validate_input(token, current, "unset"))
		return (0);
	remove = search_env_variable(data->envp, token[1]);
	if (!remove || check_unset_arg("PATH", token[1], data,
			len_of_values(current)) == 0)
		return (0);
	while (current && current != remove)
	{
		prev = current;
		current = current->next;
	}
	remove_env_variable(data, remove, prev);
	if (ft_strncmp(current->key, token[1], len_of_values(data->envp)) != 0)
		return (0);
	return (1);
}
