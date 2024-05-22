/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution_utils2.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zfiros-a <zfiros-a@student.42abudhabi.a    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/22 17:26:00 by zfiros-a          #+#    #+#             */
/*   Updated: 2024/05/22 17:26:00 by zfiros-a         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */



#include "../minishell.h"

void	exit_properly(t_data *data, char **cmd, int exit_status)
{
	if (data->path)
		free_array(data->path);
	ft_free_all(data);
	if (!ft_strncmp(cmd[1], "-", 1) && !ft_strchr(cmd[1] + 1, '-'))
		exit(256 + ft_atoi(cmd[1]));
	free_array(cmd);
	exit(exit_status);
}

void	check_dollar_or_no(char **cmd, t_data *data)
{
	if (ft_strchr(cmd[1], '$'))
	{
		ft_putstr_fd("minishell: exit: ", 2);
		ft_expansion3(data, cmd[1], 1);
		ft_putendl_fd(": numberic argument required", 2);
	}
	else if ((!ft_isdigit(cmd[1][0]) && cmd[1][0] != '-' && cmd[1][0] != '+') || ft_strchr(cmd[1] + 1, '-') || ft_strchr(cmd[1] + 1, '+'))
		name_error(cmd[0], cmd[1], ": numeric argument required", 0);
	if (cmd[1][0] != '+' || ft_strchr(cmd[1] + 1, '+'))
		exit_properly(data, cmd, 255);
}

void	exit_conditions(char **cmd, char *str, t_data *data)
{
	int	i;

	i = 0;
	while (cmd[1] && cmd[1][i] != '\0')
	{
		if (ft_isdigit(cmd[1][i]) && cmd[2])
		{
			name_error(cmd[0], NULL, "too many arguments", 0);
			free_array(cmd);
			free(data->cmd);
			g_signal = 1;
			prompt_loop(str, data);
		}
		else if (!ft_isdigit(cmd[1][i]))
			check_dollar_or_no(cmd, data);
		i++;
	}
}

void	check_exit(t_data *data, char *cmd)
{
	char	**split;

	if (cmd && (!(ft_strncmp(cmd, "exit", 4))))
	{
		split = ft_split(cmd, ' ');
		if (ft_strlen(split[0]) == 4)
		{
			exit_conditions(split, cmd, data);
			free(cmd);
			if (split[1])
				exit_properly(data, split, ft_atoi(split[1]));
			if (data->executor)
				free_executor(data->executor);
			exit_properly(data, split, 0);
		}
		free_array(split);
	}
}

void	new_variable(t_env *new, t_env *temp, char *key, char *value)
{
	new->key = key;
	new->value = value;
	temp->next = new;
	new->next = NULL;
}
