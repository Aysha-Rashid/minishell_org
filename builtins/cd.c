/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ayal-ras <ayal-ras@student.42abudhabi.a    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/11 14:42:46 by ayal-ras          #+#    #+#             */
/*   Updated: 2024/02/26 17:16:24 by ayal-ras         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	find_current_path(t_data *data, char *str)
{
	t_env	*temp_env;
	char	*temp = NULL;
	int		ret = -1;

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
	free(temp);
	if (ret != 0)
		name_error("cd", str, " not set");
	return (ret);
}

void	add_pwd_to_env(t_data *data, char *str)
{
	t_env	*temp_env;
	char	*tmp;

	temp_env = data->envp;
	tmp = NULL;
	while (temp_env)
	{
		if (!ft_strncmp(temp_env->value, "PWD=", 4))
		{
			tmp = ft_strjoin("PWD=", data->pwd);
			write(1, "1", 1);
			free(temp_env->value);
			temp_env->value = tmp;
			// free(tmp);
		}
		else if (!ft_strncmp(temp_env->value, "OLDPWD=", 7))
		{
			tmp = ft_strjoin("OLDPWD=", data->old_pwd);
			free(temp_env->value);
			temp_env->value = tmp;
			break ;
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
	if (!temp)
		return (1);
	if (temp[1] == NULL)
		changed = find_current_path(data, "HOME=");
	else if (!ft_strncmp(temp[1], "-", 1))
		changed = find_current_path(data, "OLDPWD=");
	else
	{
		change_pwd(data);
		changed = chdir(temp[1]);
		if (changed != 0)
		{
			name_error("cd", temp[1],
				": No such file or directory ");
			free_array(temp);
			return (0);
		}
	}
	free_array(temp);
	return (0);
}
