/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zfiros-a <zfiros-a@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/28 12:16:37 by ayal-ras          #+#    #+#             */
/*   Updated: 2024/02/27 08:38:04 by zfiros-a         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int g_sig_interrupt;

int	valid_command(char *str, t_data *data)
{
	if (!quote(str))
		return (ft_error(1));
	if (str && (!ft_strncmp(str, "env", 4)
			|| !ft_strncmp(str, "ENV", 4)))
		return (ft_env(data));
	else if (!ft_strncmp(str, "export", 6))
		return (ft_export(str, data));
	else if (str && (!ft_strncmp(str, "pwd", 4)
			|| !ft_strncmp(str, "PWD", 4)))
		return (ft_pwd(data));
	else if (str && (!ft_strncmp(str, "echo", 4)
			|| (!ft_strncmp(str, "ECHO", 4))
			|| !ft_strncmp(str, "echo -n", 7)))
		return (ft_echo(str)); // handle single qoutes and double qoute
	else if (str && ((!ft_strncmp(str, "cd", 2))))
		return (ft_cd(str, data)); // handle single qoutes and double qoute
	else if (str && (!ft_strncmp(str, "unset", 5)))
		return (ft_unset(str, data));
	else if (!*str)
		return (0);
	// free(str);
	return (ft_error(2));
}

void	prompt_loop(char *str, t_data *data)
{
	char	*trimmed_cmd;

	while (1)
	{
		ft_signals();
		str = readline("\033[96mminishell$ \033[0m");
		check_signal(str, data);
		data->cmd = ft_strdup(str);
		trimmed_cmd = ft_strtrim(str, " ");
		free(str);
		if (trimmed_cmd && ft_strncmp(trimmed_cmd, "exit", 5) == 0)
		{
			ft_putendl_fd("\033[0;32msee you around 😮‍💨!\033[0m", 1);
			ft_putendl_fd("exit", 1);
			free(trimmed_cmd);
			free_env_list(data->envp);
			exit(0);
		}
		valid_command(trimmed_cmd, data);
		if (!token_reader(data))
			ft_error(3);
		add_history(trimmed_cmd);
		free(trimmed_cmd);
	}
}

int	main(int argc, char **argv, char **env)
{
	t_data	*data;

	// (void)env;
	if (argv[1] || argc != 1)
	{
		ft_putendl_fd("invalid arguments", 2);
		exit(0);
	}
	data = (t_data *)malloc(sizeof(t_data));
	if (data == NULL)
		exit(EXIT_FAILURE);
	data->commands = (t_commands *)malloc(sizeof(t_commands));
	if (data->commands == NULL)
		exit(EXIT_FAILURE);
	parse_env(data, env);
	// find_pwd(data);
	prompt_loop(*argv, data);
	free_data(data);
	return (0);
}
