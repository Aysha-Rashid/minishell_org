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

void	name_error3(char *exit_status, char *message, int flag)
{
	if (!flag)
	{
		ft_putstr_fd("minishell: ", STDERR_FILENO);
		ft_putstr_fd(exit_status, STDERR_FILENO);
		ft_putstr_fd(": ", STDERR_FILENO);
		ft_putendl_fd(message, STDERR_FILENO);
	}
	else if (flag == 2 || flag == 1)
		ft_putstr_fd(exit_status, 1);
	if (g_signal == 1)
		g_signal = 0;
}

int	expansion_or_no(char quote, char index, char *str, t_data *data)
{
	if (quote == '\"' && index == '$')
		return (ft_expansion3(data, str, 2), 1);
	else if (!ft_strchr(str, '$'))
		ft_putchar_fd(index, 1);
	return (0);
}

int	check_and_write(char *str, t_data *data)
{
	size_t	i;
	char	quote;

	i = 0;
	quote = '\0';
	while (i < ft_strlen(str))
	{
		if (str[i] == '$' && quote == '\0')
			return (ft_expansion3(data, str, 2), 1);
		else if (str[i] == '\'' || str[i] == '"')
		{
			quote = str[i++];
			while (i < ft_strlen(str) && str[i] != quote)
			{
				if (expansion_or_no(quote, str[i], str, data))
					return (1);
				i++;
			}
		}
		else
			ft_putchar_fd(str[i], 1);
		i++;
	}
	return (0);
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
			{
				g_signal = 127;
				return (0);
			}
		}
		i++;
	}
	return (1);
}

int	ft_echo(char *argv, t_data *data)
{
	int		n_option;
	int		i;
	char	**str;

	n_option = 0;
	i = 1;
	str = ft_split(argv, ' ');
	if (ft_strcmp(str[0], "echo"))
		return (free_array(str), 1);
	if (ft_strncmp(str[1], "-n", 2) == 0)
	{
		n_option = 1;
		i++;
	}
	while (str[i] != NULL)
	{
		check_and_write(str[i], data);
		ft_putchar_fd(' ', 1);
		i++;
	}
	if (n_option == 0)
		write(1, "\n", 1);
	return (free_array(str), 0);
}
