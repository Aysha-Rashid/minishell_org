/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution_utils.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zuhafirosali <zuhafirosali@student.42.f    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/21 14:40:08 by ayal-ras          #+#    #+#             */
/*   Updated: 2024/03/21 16:32:55 by zuhafirosal      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	check_command(char *str, char *cmd, int *end, t_data *data)
{
	if (ft_strchr(cmd, '$') || ft_strchr(cmd, '?'))
	{
		ft_expansion3(data, str, 1);
		free(str);
		exit_and_free(data, end, 1);
	}
	if (data->no_path)
	{
		free(str);
		ft_error(2, cmd, data->no_path);
		exit_and_free(data, end, 0);
	}
	if (check_builtin(str) >= 0)
	{
		if (builtin_command(str, data))
		{
			free(str);
			exit_and_free(data, end, 0);
		}
		free(str);
		exit_and_free(data, end, 1);
	}
}
