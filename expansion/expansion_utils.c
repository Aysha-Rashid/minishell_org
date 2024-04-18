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
	char	*temp;
	int		len;
	int		i;
	int		j;

	i = 0;
	j = 0;
	len = ft_strlen(key);
	temp = malloc(sizeof(char) * (len + 1));
	while (i < len - 1)
		temp[j++] = key[i++];
	if (key[i] != '\"')
		temp[j++] = key[i++];
	temp[j] = '\0';
	while (envp)
	{
		if (ft_strcmp(envp->key, temp) == 0)
			return (free(temp), envp->value);
		envp = envp->next;
	}
	return (free(temp), NULL);
}

void	print_after_equal2(char *temp)
{
	char	*equal_pos;

	equal_pos = ft_strchr(temp, '=');
	ft_putstr_fd(equal_pos + 1, 2);
}

int	name_error2(char *name, char *str, char *message, int flag)
{
	if (!flag)
	{
		ft_putstr_fd("minishell: ", STDERR_FILENO);
		print_after_equal2(name);
		ft_putstr_fd(": ", STDERR_FILENO);
		ft_putstr_fd(str, STDERR_FILENO);
		ft_putendl_fd(message, STDERR_FILENO);
	}
	else
		print_after_equal2(name);
	return (0);
}
