/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pwd.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ayal-ras <ayal-ras@student.42abudhabi.a    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/10 20:31:16 by ayal-ras          #+#    #+#             */
/*   Updated: 2024/03/02 20:52:36 by ayal-ras         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	find_pwd(t_data *data)
{
	t_env	*temp;

	temp = data->envp;
	while (temp)
	{
		if (!(ft_strncmp(temp->value, "PWD=", 4)))
		{
			if (data->pwd == NULL)
				data->pwd = ft_substr(temp->value, 4,
						ft_strlen(temp->value) - 4);
		}
		else if (!(ft_strncmp(temp->value, "OLDPWD=", 7)))
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
	char	cwd[1024];

	(void)data;
	if (getcwd(cwd, 1024))
	{
		ft_putendl_fd(cwd, 1);
		return (0);
	}
	else
		return (1);
}

// void	change_pwd(t_data *tools)
// {
// 	char	cwd[1024];
// 	char	*OLDPWD;

// 	if (getcwd(cwd, 1024) == NULL)
// 		return ;
// 	OLDPWD = ft_strjoin("OLDPWD=", cwd);
// 	if (!(OLDPWD))
// 		return ;
// 	if (!already_there(OLDPWD, tools))
// 	{
// 		free(OLDPWD);
// 		return ;
// 	}
// 	if (!already_there(ft_pwd(), tools))
// 	{
// 		free(OLDPWD);
// 		return ;
// 	}
// 	free(OLDPWD);
// 	return ;
// }

void	change_pwd(t_data *tools)
{
	char	*tmp;

	find_pwd(tools);
	tmp = ft_strdup(tools->pwd);
	if (!tmp)
		return ;
	free(tools->old_pwd);
	tools->old_pwd = tmp;
	free(tools->pwd);
	tools->pwd = getcwd(NULL, sizeof(NULL));
}
