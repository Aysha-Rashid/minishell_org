/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ayal-ras <ayal-ras@student.42abudhabi.a    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/28 12:16:37 by ayal-ras          #+#    #+#             */
/*   Updated: 2024/02/20 22:23:12 by ayal-ras         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	main(int argc, char **argv, char **env)
{
	t_data	data;

	if (argv[1] || argc != 1)
	{
		ft_putendl_fd("invalid arguments", 2);
		exit(0);
	}
	data.commands = (t_commands *)malloc(sizeof(t_commands));
	if (data.commands == NULL)
		exit(EXIT_FAILURE);
	parse_env(&data, env);
	find_pwd(&data);
	prompt_loop(*argv, &data);
}

void	prompt_loop(char *str, t_data *data)
{
	char	*trimmed_cmd;

	while (1)
	{
        str = readline("\033[96mminishell$ \033[0m");
        data->cmd = ft_strdup(str);
        trimmed_cmd = ft_strtrim(str, " ");
		free(str);
		if (trimmed_cmd && ft_strncmp(trimmed_cmd, "exit", 5) == 0)
		{
			ft_putendl_fd("\033[0;32msee you around 😮‍💨!\033[0m", 1);
			ft_putendl_fd("exit", 1);
			free(trimmed_cmd);
			exit(0);
		}
		if (valid_command(trimmed_cmd, data) == 1)
			ft_putendl_fd("\033[31mERROR HANDLING", 2);
		add_history(trimmed_cmd);
		free(trimmed_cmd);
	}
}

int	valid_command(char *str, t_data *data)
{
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
	else if (str && (!ft_strncmp(str, "cd", 2)))
		return (ft_cd(str, data)); // handle single qoutes and double qoute
	else if (str && (!ft_strncmp(str, "unset", 5)))
		return (ft_unset(str, data));
	else if (!*str)
		return (0);
	return (1);
}
