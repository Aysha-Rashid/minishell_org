/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pwd.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ayal-ras <ayal-ras@student.42abudhabi.a    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/10 20:31:16 by ayal-ras          #+#    #+#             */
/*   Updated: 2024/02/22 13:01:27 by ayal-ras         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	find_pwd(t_data *data)
{
	t_env	*temp;

	temp = data->envp;
	data->pwd = NULL;
	data->old_pwd = NULL;

	while (temp)
	{
		if (!ft_strncmp(temp->value, "PWD=", 4))
		{
			if (!data->pwd)
				data->pwd = ft_substr(temp->value, 4,
						ft_strlen(temp->value) - 4);
		}
		if (!ft_strncmp(temp->value, "OLDPWD=", 7))
			if (!data->old_pwd)
				data->old_pwd = ft_substr(temp->value, 7,
						ft_strlen(temp->value) - 7);
		temp = temp->next;
	}
	if (!(data->pwd || data->old_pwd))
		return (1);
	return (0);
}

int	ft_pwd(t_data *data)
{
	if (!data->pwd)
		return (1);
	else
		ft_putendl_fd(data->pwd, 1);
	return (0);
}

void	change_pwd(t_data *tools)
{
	char	*tmp;

	tmp = ft_strdup(tools->pwd);
	free(tools->old_pwd);
	tools->old_pwd = tmp;
	free(tools->pwd);
	tools->pwd = getcwd(NULL, sizeof(NULL));
}
