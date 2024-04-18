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
	(void)data;

	i = 0;
	len = ft_strlen(str);
	if (str[i + 1] == '$' || str[i] == '$')
	{
			// if (search_env_variable2)
		ft_putendl_fd("come here", 2);
		ft_expansion3(data, str, 2);
		g_signal = 0;
		}
	// ft_putchar_fd(str[i], 2);
	while (i < len && (str[i] != '$' || str[i + 1] != '$'))
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
	// char	quote;
	char	**str;

	n_option = 0;
	i = 1;
	str = ft_split(argv, ' ');
	// if (!ft_strcmp(str[0], "echo") || !ft_strcmp(str[0], "ECHO"))
	// 	i++;
	if (!ft_strcmp(str[i], "-n"))
	{
		n_option = 1;
		i++;
	}

	while(str[i])
	{
		// if (ft_strchr(str[i], '$'))
		// {
		// 	ft_expansion3(data, str[i], 2);
		// 	g_signal = 0;
		// 	// i++;
		// }
		check_and_write(str[i], data);
		ft_putchar_fd(' ', 1);
		i++;
	}
	// while ((argv[i] == ' ' || argv[i] == '\t')
	// 	&& (!ft_strchr(data->cmd, '\"') || !ft_strchr(data->cmd, '\'')))
	// 	i++;
	// while (argv[i])
	// {
	// 	while (argv[i] == ' ' || argv[i] == '\t')
	// 	{
	// 		quote = 1;
	// 		i++;
	// 	}
	// 	if (quote)
	// 	{
	// 		ft_putchar_fd(' ', 1);
	// 		quote = 0;
	// 	}
	// 	if (argv[i] != ' ' && argv[i] != '\t')
	// 	{
	// 		if (argv[i] == '$')
	// 			ft_expansion3(data, argv, 2);
	// 		else
	// 			ft_putchar_fd(argv[i], 1);
	// 	}
	// 	i++;
	// }
	if (n_option == 0)
		write(1, "\n", 1);
	return (0);
}
