/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins_utils2.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zfiros-a <zfiros-a@student.42abudhabi.a    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/22 17:28:45 by zfiros-a          #+#    #+#             */
/*   Updated: 2024/05/22 17:28:45 by zfiros-a         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	check_unset_arg(char *token, t_data *data)
{
	t_env	*envp;

	envp = data->envp;
	while (envp)
	{
		if (ft_strcmp("PATH", token) == 0 && ft_strcmp("PATH", envp->key) == 0)
		{
			if (!envp)
				return (0);
			data->no_path = 1;
			return (0);
		}
		envp = envp->next;
	}
	return (1);
}

int	invalid_export_loop(char *token, char *name, t_data *data)
{
	int	i;
	int	error;

	error = 0;
	i = 0;
	if (!ft_strcmp(name, "export"))
	{
		while (token[i])
		{
			if (!(ft_isalpha(token[i])) && !ft_isdigit(token[i])
				&& token[i] != '_' && token[i] != '\"' && token[i] != '\''
				&& token[i] != '=')
			{
				error = 1;
				break ;
			}
			i++;
		}
	}
	if (error == 1)
		return (not_valid_message(token, name, data), 1);
	return (0);
}

void	print_after_equal(char *temp)
{
	char	*equal_pos;

	equal_pos = ft_strchr(temp, '=');
	ft_putstrn_fd(temp, equal_pos - temp + 1, 1);
	ft_putchar_fd('"', 1);
	ft_putstr_fd(equal_pos + 1, 1);
	ft_putendl_fd("\"", 1);
}

// hello=there 
//equal = 5;
// str = 11;
// 11 - 5 
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
	if (data->envp)
		free_env_list(data->envp);
	free(data->pwd);
	free(data->old_pwd);
	free(data->cmd);
}
