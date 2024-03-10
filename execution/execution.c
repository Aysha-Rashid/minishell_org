/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ayal-ras <ayal-ras@student.42abudhabi.a    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/08 17:27:11 by ayal-ras          #+#    #+#             */
/*   Updated: 2024/03/10 19:41:05 by ayal-ras         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	init_executor(t_data *data)
{
	data->executor = (t_executor *)malloc(sizeof(t_executor));
	if (!data->executor)
		return ;
	data->executor->pipes = 0;
	data->executor->heredoc = 0;
	data->executor->in = 0;
	data->executor->out = 0;
}

int	simple_cmd(char *cmd, t_data *data)
{
	char	**str;
	int		builtin_index;
	int		pid;

	pid = 0;
	str = ft_split(data->cmd, ' ');
	builtin_index = check_builtin(str);
	free_array(str);
	if (builtin_index)
		return (builtin_command(cmd, data));
	else if (cmd)
	{
		pid = fork();
		if (pid == -1)
			ft_error(3, NULL, 0);
		if (pid == 0)
		{
			cmd_file(data, data->envp->path);
			free(data->lexer_list);
			free(data->executor);
			ft_free_all(data);
			exit(data->status_code);
		}
		waitpid(pid, &data->status_code, 0);
	}
	return (1);
}

int	check_builtin(char **str)
{
	int			i;
	static char	*builtins[] = {
		"echo",
		"cd",
		"pwd",
		"export",
		"unset",
		"env",
		"exit"
	};

	i = 0;
	while (i < 7)
	{
		if (ft_strcmp(builtins[i++], str[0]) == 0)
			return (i);
	}
	return (0);
}
