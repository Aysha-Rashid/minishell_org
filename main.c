/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ayal-ras <ayal-ras@student.42abudhabi.a    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/11 08:32:56 by ayal-ras          #+#    #+#             */
/*   Updated: 2024/03/15 14:50:59 by ayal-ras         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	builtin_command(char *str, t_data *data)
{
	if (str && (!ft_strncmp(str, "echo", 4)
			|| (!ft_strncmp(str, "ECHO", 4))
			|| (!ft_strncmp(str, "echo -n", 7))))
		return (ft_echo(data->cmd, data), 0);
	if (str && (!ft_strcmp(str, "env")
			|| !ft_strcmp(str, "ENV")))
		return (ft_env(data, str), 0);
	else if (!ft_strncmp(str, "export", 6))
		return (ft_export(str, data), 0);
	else if (str && (!ft_strncmp(str, "pwd", 3)
			|| !ft_strncmp(str, "PWD", 3)))
		return (ft_pwd(data), 0);
	else if (str && (!(ft_strncmp(str, "cd", 2))))
		return (ft_cd(str, data), 0);
	else if (str && (!ft_strncmp(str, "unset", 5)))
		return (ft_unset(str, data), 0);
	else if (!*str)
		return (0);
	return (1);
}

void	prompt_loop(char *str, t_data *data)
{
	char	*trimmed_cmd;

	while (1)
	{
		ft_signals();
		str = readline("\033[96mminishell$ \033[0m");
		check_signal(str, data);
		trimmed_cmd = ft_strtrim(str, " ");
		data->cmd = ft_strdup(trimmed_cmd);
		free(trimmed_cmd);
		free(str);
		check_n_execute(data->cmd, data);
		add_history(data->cmd);
		free(data->cmd);
	}
}

int	main(int argc, char **argv, char **env)
{
	t_data	data;

	if (!env || env[0] == NULL)
	{
		ft_error(2, "env", 1);
		exit(0);
	}
	if (argv[1] || argc != 1)
	{
		ft_putendl_fd("invalid arguments", 2);
		exit(0);
	}
	data.pwd = NULL;
	data.old_pwd = NULL;
	data.no_path = 0;
	g_signal = 0;
	parse_env(&data, env);
	find_pwd(&data);
	prompt_loop(*argv, &data);
	ft_free_all(&data);
	return (0);
}
