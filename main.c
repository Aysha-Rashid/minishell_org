/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ayal-ras <ayal-ras@student.42abudhabi.a    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/11 08:32:56 by ayal-ras          #+#    #+#             */
/*   Updated: 2024/03/12 14:18:50 by ayal-ras         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	builtin_command(char *str, t_data *data)
{
	if (!quote(str))
		return (ft_error(1, NULL, data->no_path));
	if (str && (!ft_strncmp(str, "env", 3)
			|| !ft_strncmp(str, "ENV", 3)))
		return (ft_env(data, str),free(data->executor), 0);
	else if (!ft_strncmp(str, "export", 6))
		return (ft_export(str, data),free(data->executor), 0);
	else if (str && (!ft_strncmp(str, "pwd", 4)
			|| !ft_strncmp(str, "PWD", 4)))
		return (ft_pwd(data),free(data->executor), 0);
	else if (str && (!ft_strncmp(str, "echo", 4)
			|| (!ft_strncmp(str, "ECHO", 4))
			|| !ft_strncmp(str, "echo -n", 7)))
		return (ft_echo(str),free(data->executor), 0);
	else if (str && (!(ft_strncmp(str, "cd", 2))))
		return (ft_cd(str, data),free(data->executor), 0);
	else if (str && (!ft_strncmp(str, "unset", 5)))
		return (ft_unset(str, data),free(data->executor), 0);
	else if (!*str)
		return (free(data->executor), 0);
	// execution(str, data, data->executor);
	return (0);
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

	if (env[0] == NULL || !env)
	{
		ft_putendl_fd("minishell: env: no such file or directory", 2);
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
	data.status_code = 0;
	parse_env(&data, env);
	find_pwd(&data);
	prompt_loop(*argv, &data);
	// free_data(&data);
	free(data.cmd);
	return (0);
}