/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipes.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zfiros-a <zfiros-a@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/28 17:03:36 by ayal-ras          #+#    #+#             */
/*   Updated: 2024/03/15 15:31:04 by zfiros-a         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	check_n_execute(char *str, t_data *data)
{
	if (!str || str[0] == '\0')
	{
		ft_putendl_fd("spaces or tabs", 2);
		prompt_loop(str, data);
	}
	if (str && (!(ft_strcmp(str, "exit"))))
	{
		ft_putendl_fd("\033[0;32msee you around 😮‍💨!\033[0m", 1);
		ft_putendl_fd("exit", 1);
		if (data->envp->path != NULL)
			free_array(data->envp->path);
		ft_free_all(data);
		exit(0);
	}
	if (check_pipes_n_execute(data))
		return ;
}

int	check_pipes_n_execute(t_data *data)
{
	char		builtin_index;
	// char		*temp;

	if (!quote(data->cmd))
	// 	return (ft_error(1, NULL, data->no_path));
	// temp = remove_quotes(data->cmd);
	// ft_putendl_fd("temp", 2);
	// if (!temp || only_tabs_and_space(temp))
	// 	return (1);
	if (parse_com(data->cmd))
		return (1);
	data->executor = parse_pipeline(data->cmd, data);
	builtin_index = check_builtin(data->cmd);
	if (builtin_index >= 0 && !check_redir_pipe(data->cmd))
		builtin_command(data->cmd, data);
	else
		execution(data->executor, data);
	free_executor(data->executor);
	return (0);
}

t_executor	*parse_pipeline(char *cmd, t_data *data)
{
	t_executor	*head;
	t_executor	*tail;
	t_executor	*executor;
	char		**token;
	int			i;

	i = 0;
	head = NULL;
	tail = NULL;
	if (ft_strchr(cmd, '\'') || ft_strchr(cmd, '\"'))
		return (init_executor(data, cmd));
	token = ft_split(cmd, '|');
	while (token[i])
	{
		executor = init_executor(data, token[i]);
		executor->prev = tail;
		i++;
		if (head == NULL)
			head = executor;
		else
			tail->next = executor;
		tail = executor;
	}
	free_array(token);
	return (head);
}

int	redir(t_executor *executor)
{
	char	*redir;
	int		i;

	redir = get_redir_and_files(executor->cmd);
	i = 0;
	while (executor->cmd[i])
	{
		if (executor->cmd[i] == '>' && executor->cmd[i + 1] == '>')
		{
			if (!open_files(executor->cmd, redir, i, executor))
				return (free(redir), 0);
			i++;
		}
		else if ((executor->cmd[i] == '>' || executor->cmd[i] == '<')
			&& executor->cmd[i] != ' ')
		{
			if (!open_files(executor->cmd, redir, i, executor))
				return (free(redir), 0);
			i++;
		}
		else
			i++;
	}
	return (free(redir), 1);
}

void	init_pipe_n_signal(int *prev_pipe)
{
	prev_pipe[0] = STDIN_FILENO;
	prev_pipe[1] = STDOUT_FILENO;
	// signal(SIGQUIT, ft_sig2);
	signal(SIGINT, ft_sig2);
}
