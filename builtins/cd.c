/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ayal-ras <ayal-ras@student.42abudhabi.a    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/11 14:42:46 by ayal-ras          #+#    #+#             */
/*   Updated: 2024/02/17 19:09:59 by ayal-ras         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	find_current_path(t_data *data, char *str)
{
	t_env	*temp_env;
	char	*temp;
	int		ret;

	temp_env = data->envp;
	while (temp_env)
	{
		if (!ft_strncmp(temp_env->value, str, ft_strlen(str)))
			temp = ft_substr(temp_env->value, ft_strlen(str),
					ft_strlen(temp_env->value) - ft_strlen(str));
		temp_env = temp_env->next;
	}
	ret = chdir(temp);
	free(temp);
	if (ret != 0)
	{
		str = ft_substr(str, 0, ft_strlen(str) - 1);
		ft_putstr_fd(str, 2);
		free(str);
		ft_putendl_fd("not set", 2);
	}
	return (ret);
}

int	ft_cd(char *str, t_data *data)
{
	char	**temp;
	int		changed;

	changed = 0;
	temp = ft_split(str, ' ');
	if (temp[1] == NULL)
		changed = find_current_path(data, "HOME=");
	else if (!ft_strncmp(temp[1], "-", 2))
		changed = find_current_path(data, "OLDPWD=");
	else
	{
		changed = chdir(temp[1]);
		if (changed != 0)
		{
			ft_putstr_fd("minishell: ", STDERR_FILENO);
			ft_putstr_fd(temp[1], STDERR_FILENO);
			perror(" ");
		}
	}
	change_pwd(data);
	add_pwd_to_env(data, temp[1]);
	if (changed != 0)
		return (1);
	return (0);
}

void	add_pwd_to_env(t_data *data, char *str)
{
	t_env	*temp_env;
	char	*tmp;

	temp_env = data->envp;
	while (temp_env)
	{
		if (!ft_strncmp(temp_env->value, "PWD=", 4))
		{
			tmp = ft_strjoin("PWD=", data->pwd);
			free(temp_env->value);
			temp_env->value = tmp;
		}
		else if (!ft_strncmp(temp_env->value, "OLDPWD=", 7) && data->old_pwd)
		{
			tmp = ft_strjoin("OLDPWD=", data->old_pwd);
			free(temp_env->value);
			temp_env->value = tmp;
		}
		temp_env = temp_env->next;
	}
	if (*str == '-')
		ft_putendl_fd(data->pwd, 1);
}
