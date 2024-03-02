/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ayal-ras <ayal-ras@student.42abudhabi.a    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/28 12:16:37 by ayal-ras          #+#    #+#             */
/*   Updated: 2024/03/02 19:57:48 by ayal-ras         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int g_sig_interrupt;

int	execution(char *str, t_data	*data)
{
	(void)data;
	(void)str;
	ft_putstr_fd("hello there, ready for execution?", 1);
	return (1);
}

int	buitin_command(char *str, t_data *data)
{
	if (!quote(str))
		return (ft_error(1, NULL, data));
	if (str && (!ft_strncmp(str, "env", 3)
			|| !ft_strncmp(str, "ENV", 3)))
		return (ft_env(data, str));
	else if (!ft_strncmp(str, "export", 6))
		return (ft_export(str, data));
	else if (str && (!ft_strncmp(str, "pwd", 4)
			|| !ft_strncmp(str, "PWD", 4)))
		return (ft_pwd(data));
	else if (str && (!ft_strncmp(str, "echo", 4)
			|| (!ft_strncmp(str, "ECHO", 4))
			|| !ft_strncmp(str, "echo -n", 7)))
		return (ft_echo(str));
	else if (str && ((!ft_strncmp(str, "cd", 2))))
		return (ft_cd(str, data));
	else if (str && (!ft_strncmp(str, "unset", 5)))
		return (ft_unset(str, data));
	else if (!*str)
		return (0);
	return (ft_error(2, str, data));
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

	if (argv[1] || argc != 1)
	{
		ft_putendl_fd("invalid arguments", 2);
		exit(0);
	}
	parse_env(&data, env);
	data.pwd = NULL;
	data.old_pwd = NULL;
	data.no_path = 0;
	data.status_code = 0;
	find_pwd(&data);
	prompt_loop(*argv, &data);
	free_data(&data);
	free(data.cmd);
	return (0);
}
