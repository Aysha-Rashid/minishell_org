/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins_utils2.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zfiros-a <zfiros-a@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/29 21:11:04 by ayal-ras          #+#    #+#             */
/*   Updated: 2024/03/15 15:11:37 by zfiros-a         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	check_unset_arg(char *str, char *token, t_data *data, int size_of_env)
{
	if (ft_strncmp(str, token, size_of_env) == 0)
	{
		if (!data->envp)
			return (0);
		data->no_path = 1;
		return (0);
	}
	return (1);
}

t_env	*duplicate_env(t_env *env)
{
	t_env	*head;
	t_env	*temp;

	temp = NULL;
	head = NULL;
	while (env != NULL)
	{
		if (head == NULL)
		{
			head = malloc(sizeof(t_env));
			temp = head;
		}
		else
		{
			temp->next = malloc(sizeof(t_env));
			temp = temp->next;
		}
		if (temp == NULL)
			return (NULL);
		temp->value = ft_strdup(env->value);
		temp->next = NULL;
		env = env->next;
	}
	return (head);
}

void	print_after_equal(char *temp)
{
	char	*equal_pos;

	equal_pos = ft_strchr(temp, '=');
	// ft_putstrn_fd(temp, equal_pos - temp + 1, 1);
	// ft_putchar_fd('"', 1);
	ft_putstr_fd(equal_pos + 1, 1);
	// ft_putendl_fd("\"", 1);
}

char	*ft_strcpy(char	*dest, char *src)
{
	int	i;

	i = 0;
	while (src[i])
	{
		dest[i] = src[i];
		i++;
	}
	dest[i] = '\0';
	return (dest);
}

void	ft_free_all(t_data *data)
{
	free(data->pwd);
	free(data->old_pwd);
	free_env_list(data->envp);
	free(data->cmd);
}
