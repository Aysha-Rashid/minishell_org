/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   errors.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zfiros-a <zfiros-a@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/27 08:17:00 by zfiros-a          #+#    #+#             */
/*   Updated: 2024/02/27 08:34:35 by zfiros-a         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int ft_error(int i)
{
    if (i == 1)
        ft_putstr_fd("no closing quotation found\n", 2);
    if (i == 2)
        ft_putstr_fd("ERROR HANDLING\n", 2);
	if (i == 3)
		ft_putstr_fd("unable to assign memory\n", 2);
    return (1);
}