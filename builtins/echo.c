/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ayal-ras <ayal-ras@student.42abudhabi.a    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/11 14:52:34 by ayal-ras          #+#    #+#             */
/*   Updated: 2024/02/20 19:57:02 by ayal-ras         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	ft_echo(char *argv)
{
	char	**token;
	int		n_option;
	int		i;

	token = ft_split(argv, ' ');
	n_option = 0;
	i = 1;
	while (token[i] && ft_strncmp(token[i], "-n", 7) == 0)
	{
		n_option = 1;
		i++;
	}
	while (token[i])
	{
		ft_putstr_fd(token[i], 1);
		if (token[i + 1] && token[i][0] != '\0')
			write(1, " ", 1);
		i++;
	}
	if (n_option == 0)
		write(1, "\n", 1);
	return (0);
}
