/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   unset.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ayal-ras <ayal-ras@student.42abudhabi.a    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/11 21:03:21 by ayal-ras          #+#    #+#             */
/*   Updated: 2024/03/14 14:31:24 by ayal-ras         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	invalid_identifier(t_data *data, char *str, char *name)
{
	int	error;

	error = 0;
	if (!(ft_isalpha(str[0])) && str[0] != '_'
		&& str[0] != '\"' && str[0] != '\'')
	{
		not_valid_message(str, name, data);
		error = 1;
	}
	return (error);
}

int	invalid_unset_loop(char *token, char *name, t_data *data)
{
	int	i;
	int	error;

	error = 0;
	i = 0;
	if (!ft_strcmp(name, "unset"))
	{
		while (token[i])
		{
			if (!(ft_isalpha(token[i])) && !ft_isdigit(token[i])
				&& token[i] != '_' && token[i] != '\"' && token[i] != '\'')
			{
				error = 1;
				break ;
			}
			i++;
		}
	}
	if (error == 1)
	{
		not_valid_message(token, name, data);
		return (1);
	}
	return (0);
}

int	remove_env_variable(t_data *data, t_env *to_remove, t_env *prev)
{
	if (to_remove)
	{
		if (prev)
			prev->next = to_remove->next;
		else
			data->envp = to_remove->next;
		if (data->envp && !data->envp->key)
			return (1);
		free(to_remove->key);
		free(to_remove->value);
		free(to_remove);
		return (1);
	}
	return (0);
}

int	ft_unset(char *str, t_data *data)
{
	char	**token;
	t_env	*current;

	token = ft_split(str, ' ');
	current = data->envp;
	if (ft_strlen(token[0]) != 5)
		return (free_array(token), ft_error(2, str, data->no_path), 1);
	if (!unset_loop(data, current, token))
		return (free_array(token), 0);
	return (free_array(token), 1);
}

int	unset_loop(t_data *data, t_env *current, char **token)
{
	int		i;
	t_env	*remove;
	t_env	*prev;

	i = 1;
	while (token[i])
	{
		if (!validate_input(data, token[i], current, "unset"))
			i++;
		if (token[i] == NULL)
			return (1);
		remove = search_env_variable(data->envp, token[i]);
		current = data->envp;
		prev = NULL;
		check_unset_arg(token[i], data);
		while (current && current != remove)
		{
			prev = current;
			current = current->next;
		}
		remove_env_variable(data, remove, prev);
		i++;
	}
	return (free(token[i]), 1);
}
