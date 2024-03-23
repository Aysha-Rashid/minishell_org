/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   errors.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ayal-ras <ayal-ras@student.42abudhabi.a    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/27 08:17:00 by zfiros-a          #+#    #+#             */
/*   Updated: 2024/03/14 16:38:23 by ayal-ras         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	ft_error(int i, char *str, int no_path)
{
	if (i == 1)
		ft_putstr_fd("no closing quotation found\n", 2);
	else if (i == 2)
	{
		if (no_path)
			name_error(str, NULL, "No such file or directory", 0);
		else
			name_error(str, NULL, "command not found", 0);
	}
	else if (i == 3)
		ft_putstr_fd("unable to assign memory\n", 2);
	else if (i == 4)
		name_error(str, NULL, "Permission denied", 0);
	else if (i == 5)
		name_error(str, NULL, "No options allowed", 0);
	return (1);
}

void	not_valid_message(char *token, char *name, t_data *data)
{
	ft_putstr_fd("minishell: ", 2);
	ft_putstr_fd(name, 2);
	ft_putstr_fd(": `", 2);
	if (token[0] == '$')
		ft_expansion3(data, token, 1);
	else
		ft_putstr_fd(token, 2);
	ft_putendl_fd("': not a valid identifier", 2);
}
