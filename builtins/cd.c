/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ayal-ras <ayal-ras@student.42abudhabi.a    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/11 14:42:46 by ayal-ras          #+#    #+#             */
/*   Updated: 2024/03/02 20:52:23 by ayal-ras         ###   ########.fr       */
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
		name_error("cd", str, " not set");
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
		return (free_array(temp), ft_error(2, str, NULL), 1);
	if (temp[1] == NULL)
		changed = find_current_path(data, "HOME=");
	else if (!(ft_strncmp(temp[1], "-", 1)))
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

// int ft_cd(char *str, t_data *data)
// {
//     char **temp;
//     int changed;

//     changed = 0;
//     find_pwd(data);
//     temp = ft_split(str, ' ');
//     if (!temp || ft_strlen(temp[0]) != 2)
//         return (free_array(temp), ft_error(2, str, NULL), 1);
//     if (temp[1] == NULL)
//         changed = find_current_path(data, "HOME=");
//     else if (!(ft_strncmp(temp[1], "-", 1)))
//     {
//         // char *old_pwd = get_env_value("OLDPWD=", data->envp); // Get the value of OLDPWD from environment
//         if (data->old_pwd)
//         {
//             changed = chdir(data->old_pwd); // Change to the old working directory
//             free(data->old_pwd);
//         }
//         else
//         {
//             name_error("cd", "OLDPWD", " not set");
//             return (free_array(temp), 1);
//         }
//     }
//     else
//     {
//         changed = chdir(temp[1]);
//         if (changed != 0)
//         {
//             name_error("cd", temp[1], ": No such file or directory ");
//             return (free_array(temp), 1);
//         }
//     }
//     add_pwd_to_env(data, temp[1]);
//     change_pwd(data);
//     return (free_array(temp), 0);
// }

