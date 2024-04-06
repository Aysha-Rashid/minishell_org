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
	else if (flag == 2)
		ft_putstr_fd(exit_status, 1);
}

void	check_and_write(char *str, t_data *data)
{
	int		i;
	int		len;
	char	quote;

	i = 0;
	len = ft_strlen(str);
	if (str[i] == '$')
	{
		ft_expansion3(data, str, 2);
		g_signal = 0;
	}
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
	int		n_option;
	int		i;
	int		j;
	char	*variable_name;

	j = 0;
	variable_name = NULL;
	(void)data;
	n_option = 0;
	i = 0;
	if ((!ft_strncmp(argv, "echo", 4) || !ft_strncmp(argv, "ECHO", 4)) && ft_strncmp(argv, "echo -n", 7))
		i += 4;
	else if (argv[i] && (ft_strncmp(argv, "echo -n", 7) == 0 || ft_strncmp(argv, "ECHO -n", 7) == 0))
	{
		n_option = 1;
		i += 7;
	}
	if (argv[i] == ' ' || argv[i] == '\t')
		i++;
	while (argv[i])
	{
		if (argv[i] == '\'' || argv[i] == '"')
		{
			char quote_char = argv[i++];
			while (argv[i] && argv[i] != quote_char)
			{
				if (argv[i] == '$')
				{
					j = i + 1;
					while (argv[j] && (isalnum(argv[j]) || argv[j] == '$'))
						j++;
					variable_name = strndup(argv + i, j - i);
					ft_expansion3(data, variable_name, 2);
					free(variable_name);
					i = j;
				}
				else
					ft_putchar_fd(argv[i++], 1);
			}
			if (argv[i] == quote_char)
				i++;
		}
		else if (argv[i] == ' ' || argv[i] == '\t')
		{
			while (argv[i] && (argv[i] == ' ' || argv[i] == '\t'))
				i++;
			ft_putchar_fd(' ', 1);
		}
		else if (argv[i] == '$')
		{
			j = i + 1;
			while (argv[j] && (isalnum(argv[j]) || argv[j] == '$' || argv[j] == '?'))
				j++;
			variable_name = strndup(argv + i, j - i);
			ft_expansion3(data, variable_name, 2);
			g_signal = 0;
			free(variable_name);
			i = j;
		}
		else
			ft_putchar_fd(argv[i++], 1);
	}
	if (n_option == 0)
		write(1, "\n", 1);
	return (0);
}
