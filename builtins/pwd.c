/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pwd.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ayal-ras <ayal-ras@student.42abudhabi.a    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/10 20:31:16 by ayal-ras          #+#    #+#             */
/*   Updated: 2024/02/26 17:20:42 by ayal-ras         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	find_pwd(t_data *data)
{
	t_env	*temp;

	data->pwd = NULL;
	data->old_pwd = NULL;
	temp = data->envp;
	while (temp)
	{
		if (!ft_strncmp(temp->value, "PWD=", 4))
		{
			if (data->pwd == NULL)
				data->pwd = ft_substr(temp->value, 4,
						ft_strlen(temp->value) - 4);
		}
		if (!ft_strncmp(temp->value, "OLDPWD=", 7))
		{
			if (data->old_pwd == NULL)
				data->old_pwd = ft_substr(temp->value, 7,
						ft_strlen(temp->value) - 7);
		}
		temp = temp->next;
	}
	if (!(data->pwd || data->old_pwd))
		return (1);
	return (0);
}

int	ft_pwd(t_data *data)
{
	(void)data;
	char	cwd[1024];
	if (getcwd(cwd, 1024))
	{
		ft_putendl_fd(cwd, 1);
		return (0);
	}
	else
		return (1);
}

void	change_pwd(t_data *tools)
{
	// char	*tmp;

	// tmp = ft_strdup(tools->pwd);
	// free(tools->old_pwd);
	// tools->old_pwd = tmp;
	// free(tools->pwd);
	// tools->pwd = getcwd(NULL, sizeof(NULL));
	char	cwd[1024];
	char	*oldpwd;

	if (getcwd(cwd, 1024) == NULL)
		return ;
	if (!(oldpwd = ft_strjoin("OLDPWD=", cwd)))
		return ;
	if (!env_add(oldpwd, tools))
	{
		free(oldpwd);
		return ;
	}
	free(oldpwd);
	return ;
}
