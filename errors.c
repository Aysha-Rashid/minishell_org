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
			name_error(str, NULL, "No such file or directory");
		else
			name_error(str, NULL, "command not found");
	}
	else if (i == 3)
		ft_putstr_fd("unable to assign memory\n", 2);
	else if (i == 4)
		name_error(str, NULL, "Permission denied");
	else if (i == 5)
		name_error(str, NULL, "No options allowed");
	return (1);
}
