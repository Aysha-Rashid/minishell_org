/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ayal-ras <ayal-ras@student.42abudhabi.a    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/11 14:52:34 by ayal-ras          #+#    #+#             */
/*   Updated: 2024/02/29 15:32:58 by ayal-ras         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	check_and_write(char *str)
{
	int	i;
	int	len;

	i = 0;
	len = ft_strlen(str);
	if (str[0] == '\'' || str[0] == '\"')
		i = 1;
	while (str[i])
	{
		while (str[i] && (i < (len - 1)))
		{
			ft_putchar_fd(str[i], 1);
			i++;
		}
		if ((str[len - 1] != '\'' && str[len - 1] != '\"'))
		{
			ft_putchar_fd(str[len - 1], 1);
			i++;
		}
		else
			break ;
	}
}

int	quote(char *l)
{
	size_t	i;
	char	is_quote;

	i = 0;
	while (l[i] != '\0')
	{
		if (l[i] == '\'' || l[i] == '\"')
		{
			is_quote = l[i];
			i++;
			while (l[i] != is_quote && l[i])
				i++;
			if (l[i] == '\0')
				return (0);
		}
		i++;
	}
	return (1);
}

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
		check_and_write(token[i]);
		if (token[i + 1] && token[i][0] != '\0')
			write(1, " ", 1);
		i++;
	}
	if (n_option == 0)
		write(1, "\n", 1);
	return (0);
}
