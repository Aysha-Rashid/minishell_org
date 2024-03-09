/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ayal-ras <ayal-ras@student.42abudhabi.a    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/28 12:16:37 by ayal-ras          #+#    #+#             */
/*   Updated: 2024/03/09 08:35:47 by ayal-ras         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int 	g_sig_interrupt = 0;

int	execution(char *str, t_data	*data, t_executor *executor)
{
	// (void)data;
	// (void)str;
	(void)executor;
	// int pipe_fd[2];
	// static int i;
	char	**temp;
	char	*path;
	// (void)executor;
	// if (executor->pid == 0)
	// 	buitin_command(data->cmd, data);
	// else
	// {
		// char *argv = "ls";
		// executor->in = STDIN_FILENO;
		// int len = len_of_values(data->cmd);
		// executor->pid[0] = fork();
		// executor->pid = ft_calloc(sizeof(int), executor->pipes + 2);
		// if (!executor->pid)
		// 	return (ft_error(3, NULL, data), free(executor), 1);
		temp = ft_split(str, ' ');
		// pipe(pipe_fd);
		int hello;
		hello = fork();
		// write(2, "1", 1);
		if (hello == 0)
		{
			// dup2(executor->in, STDIN_FILENO);
			// close(pipe_fd[0]);
			// dup2(pipe_fd[1], STDOUT_FILENO);
			// close(pipe_fd[1]); 
			path = cmd_file(*temp, data->envp->path);
			// i++;
			if (!path)
				return (ft_error(2, *temp));
			if (execve(path, temp, NULL) == -1)
				write(1, "doesnt work", 11);
			free_array(temp);
			free_env_list(data->envp);
			free(data->old_pwd);
			free(data->pwd);
			free(data->cmd);
			free(executor->pid);
			free(executor);
			exit(0);
			// write(1, "comes", 5);
			// exit(0);
		}
		// else
		wait(NULL);
		exit(0);
		// write(2, "comes here?", 11);
		// prompt_loop(str, data);
		// check_n_execute(str, data);
		// wait(NULL);
		// exit(0);
	// }
	return (0);
}

int	builtin_command(char *str, t_data *data)
{
	if (!quote(str))
		return (ft_error(1, NULL));
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
	execution(str, data, data->executor);
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
		ft_expansion(data);
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
