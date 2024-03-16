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

int	invalid_identifier(char *str, char *name)
{
	int	error;

	error = 0;
		// write(1, "hello", 5);
	if (!(ft_isalpha(str[0])) && str[0] != '_'
		&& str[0] != '\"' && str[0] != '\'')
	{
		ft_putstr_fd("minishell: ", 2);
		ft_putstr_fd(name, 2);
		ft_putstr_fd(": `", 2);
		ft_putstr_fd(str, 2);
		ft_putendl_fd("': not a valid identifier", 2);
		error = 1;
	}
	return (error);
}

int	validate_input(char *token, t_env *current, char *name)
{
	// if (ft_strcmp(name, "unset"))
	// {
		if (invalid_identifier(token, name) || !current)
			return (0);
	// }
	// else
	// {
	// 	if (invalid_identifier(token, name) || !current)
	// 		return (0);
	// }
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

	current = data->envp;
	token = ft_split(str, ' ');
	if (ft_strlen(token[0]) != 5)
		return (free_array(token), ft_error(2, str, data->no_path), 1);
	if (!unset_loop(data, current, token))
		return (0);
	return (1);
}

int	unset_loop(t_data *data, t_env *current, char **token)
{
	int		i;
	t_env	*remove;
	t_env	*prev;

	i = 1;
	while (token[i])
	{
		token[i] = remove_all_qoutes(token[i]);
		if (!validate_input(token[i], current, "unset"))
			i++;
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
		remove_env_variable(data, remove, prev);
		i++;
	}
	return (free_array(token), 1);
}
