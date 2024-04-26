/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expansion1.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zfiros-a <zfiros-a@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/07 18:08:51 by zfiros-a          #+#    #+#             */
/*   Updated: 2024/03/15 15:27:37 by zfiros-a         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

size_t	dollar_sign(char *str)
{
	size_t	i;

	i = 0;
	while (str[i])
	{
		if (str[i] == '$')
			return (i + 1);
		i++;
	}
	return (0);
}

int	ft_expansion(t_data *data)
{
	int		i;
	char	**splitted;
	int		j;

	i = 0;
	splitted = ft_split(data->cmd, ' ');
	j = dollar_sign(splitted[0]);
	if (j == 0)
		return (ft_error(2, splitted[0], 0), free_array(splitted), 0);
	while (splitted[i])
	{
		ft_expansion3(data, splitted[i], 0);
		i++;
	}
	free_array(splitted);
	return (0);
}

int	ft_specified_error(char *str, int flag)
{
	if (!flag)
	{
		if (!ft_strncmp(str, "PATH", 4))
			name_error2(str, NULL, "No such file or directory", 0);
		else if (!ft_strncmp(str, "PWD", 3)
			|| !ft_strncmp(str, "OLDPWD", 6) || !ft_strncmp(str, "HOME", 4))
			name_error2(str, NULL, "is a directory", 0);
		else
			name_error2(str, NULL, "command not found", 0);
	}
	else
		name_error2(str, NULL, "command not found", flag);
	return (0);
}

int init_expan(char *store, char *str, int j)
{
	// j = dollar_sign(str);
	// store = (char *)malloc((j + 1) * sizeof(char));
	if (store == NULL)
		return (0);
	store = ft_strncpy(store, str, j);
	store[j] = '\0';
}

int	ft_expansion3(t_data *data, char *str, int flag)
{
	char	*exp;
	char	*exit_status;
	int		j;
	char	*temp;
	char	*store;

	temp = NULL;
	exit_status = ft_itoa(g_signal);
	j = dollar_sign(str);
	store = (char *)malloc((j + 1) * sizeof(char));
	if (!init_expan(store, str, j))
		return (free(exit_status), 0);
	if (j != 0 && str[j] != '\0')
	{
		if (str[j] == '?')
			return (name_error3(exit_status, "command not found", flag),
				free(exit_status), free(store), 1);
		else
		{
			exp = search_env_variable2(data->envp, str + j);
			if (!exp)
				return (free(store), free(exit_status), 0);
			temp = ft_strjoin(store, exp);
			return (ft_specified_error(temp, flag), free(exp),
				free(store), free(temp), free(exit_status), 1);
		}
	}
	return (free(store), free(exit_status), 0);
}
