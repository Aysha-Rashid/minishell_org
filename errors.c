/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   errors.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ayal-ras <ayal-ras@student.42abudhabi.a    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/27 08:17:00 by zfiros-a          #+#    #+#             */
/*   Updated: 2024/02/27 13:59:07 by ayal-ras         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int ft_error(int i)
{
    if (i == 1)
        ft_putstr_fd("no closing quotation found\n", 2);
    else if (i == 2)
        ft_putstr_fd("ERROR HANDLING\n", 2);
	else if (i == 3)
		ft_putstr_fd("unable to assign memory\n", 2);
    return (1);
}