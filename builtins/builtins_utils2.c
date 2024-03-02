/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins_utils2.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ayal-ras <ayal-ras@student.42abudhabi.a    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/29 21:11:04 by ayal-ras          #+#    #+#             */
/*   Updated: 2024/02/29 21:15:00 by ayal-ras         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	check_unset_arg(char *str, char *token, t_data *data, int size_of_env)
{
	if (ft_strncmp(str, token, size_of_env) == 0)
	{
		if (!data->envp)
			return (0);
		data->no_path = 1;
		return (0);
	}
	return (1);
}
