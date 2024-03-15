/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expansion_utils.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zfiros-a <zfiros-a@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/07 18:24:41 by zfiros-a          #+#    #+#             */
/*   Updated: 2024/03/15 15:12:03 by zfiros-a         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

char	*search_env_variable2(t_env *envp, char *key)
{
	while (envp)
	{
		if (ft_strcmp(envp->key, key) == 0)
			return (envp->value);
		envp = envp->next;
	}
	return (NULL);
}

void	print_after_equal2(char *temp)
{
	char	*equal_pos;

	equal_pos = ft_strchr(temp, '=');
	ft_putstrn_fd(temp, equal_pos - temp + 1, 1);
	ft_putstr_fd(equal_pos + 1, 1);
}

int	name_error2(char *name, char *str, char *message)
{
	ft_putstr_fd("minishell: ", STDERR_FILENO);
	print_after_equal(name);
	ft_putstr_fd(": ", STDERR_FILENO);
	ft_putstr_fd(str, STDERR_FILENO);
	ft_putendl_fd(message, STDERR_FILENO);
	return (0);
}
