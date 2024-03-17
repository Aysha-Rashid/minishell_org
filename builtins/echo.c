/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ayal-ras <ayal-ras@student.42abudhabi.a    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/11 14:52:34 by ayal-ras          #+#    #+#             */
/*   Updated: 2024/03/13 08:06:34 by ayal-ras         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	check_and_write(char *str, t_data *data)
{
	int		i;
	int		len;
	char	quote;

	i = 0;
	len = ft_strlen(str);
	if (str[i] == '$')
		ft_expansion3(data, str, 1);
	while (i < len && str[i] != '$')
	{
		if (str[i] == '\'' || str[i] == '"')
		{
			quote = str[i];
			i++;
			while (i < len && str[i] != quote)
				ft_putchar_fd(str[i++], 1);
			if (i < len)
				i++;
		}
		else
			ft_putchar_fd(str[i++], 1);
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

int	ft_echo(char *argv, t_data *data)
{
	char	**token;
	int		n_option;
	int		i;

	token = ft_split(argv, ' ');
	n_option = 0;
	i = 1;
	if ((ft_strlen(token[0]) != 4))
		return (ft_error(2, token[0], 0), free_array(token), 1);
	while (token[i] && ft_strncmp(token[i], "-n", 7) == 0)
	{
		n_option = 1;
		i++;
	}
	while (token[i])
	{
		check_and_write(token[i], data);
		if (token[i + 1] && token[i][0] != '\0')
			write(1, " ", 1);
		i++;
	}
	if (n_option == 0)
		write(1, "\n", 1);
	free_array(token);
	return (0);
}
