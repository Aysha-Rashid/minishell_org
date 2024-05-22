/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipes.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zfiros-a <zfiros-a@student.42abudhabi.a    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/22 17:26:32 by zfiros-a          #+#    #+#             */
/*   Updated: 2024/05/22 17:26:32 by zfiros-a         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	check_n_execute(char *str, t_data *data)
{
	char	**cmd;

	if (str[0] == '\0' || only_tabs_and_space(str, 0))
	{
		free(data->cmd);
		prompt_loop(str, data);
	}
	if (str && (!(ft_strncmp(str, "exit", 4))) && !check_redir_pipe(str))
	{
		cmd = ft_split(str, ' ');
		if (ft_strlen(cmd[0]) == 4)
		{
			exit_conditions(cmd, data);
			ft_putendl_fd("\033[0;32msee you around 😮‍💨!\033[0m", 1);
			if (cmd[1])
				exit_properly(data, cmd, ft_atoi(cmd[1]));
			ft_putendl_fd("exit", 2);
			exit_properly(data, cmd, 0);
		}
		free_array(cmd);
	}
	if (check_pipes_n_execute(data))
		return ;
}

int	check_pipes_n_execute(t_data *data)
{
	char	builtin_index;
	char	*str;

	builtin_index = -1;
	if (!quote(data->cmd))
		return (ft_error(2, NULL, 0));
	if (parse_com(data->cmd))
		return (g_signal = 258, 1);
	str = remove_quotes(data->cmd);
	builtin_index = check_builtin(str);
	if (builtin_index >= 0 && !check_redir_pipe(str))
	{
		if (!builtin_command(str, data, NULL, 0))
		{
			if (g_signal == 1)
				g_signal = 1;
			else
				g_signal = 0;
			return (free(str), 0);
		}
	}
	data->executor = parse_pipeline(data->cmd, data);
	free(str);
	execution(data->executor, data);
	return (free_executor(data->executor), 0);
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
	token = ft_split(cmd, '|');
	while (token[i])
	{
		executor = init_executor(data, token[i]);
		if (!executor)
			return (free_executor(executor), free_array(token), NULL);
		executor->prev = tail;
		if (head == NULL)
			head = executor;
		else
			tail->next = executor;
		tail = executor;
		i++;
	}
	free_array(token);
	return (head);
}

int	redir(t_executor *executor)
{
	char	*redir;
	int		i;

	i = 0;
	redir = NULL;
	if (quote_redirection_parse(executor->cmd, i))
		return (1);
	i = 0;
	while (executor->cmd[i])
	{
		if (executor->cmd[i] == '>' || executor->cmd[i] == '<')
		{
			if (redir)
				free(redir);
			redir = get_redir_and_files(executor->cmd + i);
			if ((executor->cmd[i] == '<' && executor->cmd[i + 1] == '<')
				|| (executor->cmd[i] == '>' && executor->cmd[i + 1] == '>'))
				i++;
			if (!open_files(executor->cmd, redir, i, executor))
				return (free(redir), 0);
		}
		i++;
	}
	return (free(redir), 1);
}

void	init_pipe_n_signal(int *prev_pipe)
{
	prev_pipe[0] = STDIN_FILENO;
	prev_pipe[1] = STDOUT_FILENO;
	signal(SIGINT, ft_sig2);
}
