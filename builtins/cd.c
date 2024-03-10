/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ayal-ras <ayal-ras@student.42abudhabi.a    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/11 14:42:46 by ayal-ras          #+#    #+#             */
/*   Updated: 2024/03/10 17:33:28 by ayal-ras         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	find_current_path(t_data *data, char *str)
{
	t_env	*temp_env;
	char	*temp;
	int		ret;

	ret = -1;
	temp = NULL;
	if (data->no_path)
		return (0);
	temp_env = data->envp;
	while (temp_env)
	{
		if ((ft_strncmp(temp_env->value, str, ft_strlen(str)) == 0))
		{
			temp = ft_substr(temp_env->value, ft_strlen(str),
					ft_strlen(temp_env->value) - ft_strlen(str));
			break ;
		}
		temp_env = temp_env->next;
	}
	ret = chdir(temp);
	if (ret != 0)
		return (name_error("cd", data->old_pwd,
				": No such file or directory"), free(temp), 1);
	return (free(temp), ret);
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
	if (str && *str == '-')
		ft_putendl_fd(data->pwd, 1);
}

int	ft_cd(char *str, t_data *data)
{
	char	**temp;
	int		changed;

	changed = 0;
	temp = ft_split(str, ' ');
	if (!temp || ft_strlen(temp[0]) != 2)
		return (free_array(temp), ft_error(2, str, data->no_path));
	temp[1] = remove_all_qoutes(temp[1]);
	if (temp[1] == NULL)
		changed = find_current_path(data, "HOME=");
	else if (!(ft_strncmp(temp[1], "-", 1)) && ft_strlen(temp[1]) == 1)
		changed = find_current_path(data, "OLDPWD=");
	else
	{
		changed = chdir(temp[1]);
		if (changed != 0)
		{
			name_error("cd", temp[1],
				": No such file or directory ");
			return (free_array(temp), 0);
		}
	}
	change_pwd(data);
	add_pwd_to_env(data, temp[1]);
	return (free_array(temp), 0);
}
