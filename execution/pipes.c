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
	if (!data->cmd || data->cmd[0] == '\0')
		return ;
	if (str && (!(ft_strcmp(str, "exit"))))
	{
		ft_putendl_fd("\033[0;32msee you around 😮‍💨!\033[0m", 1);
		ft_putendl_fd("exit", 1);
		free_array(data->envp->path);
		// free_executor(data->executor);
		ft_free_all(data);
		// free(data->executor->cmd);
		exit(0);
	}
	if (check_pipes_n_execute(data))
		return ;
}

int	check_pipes_n_execute(t_data *data)
{
	char		builtin_index;
	char		**str;

	if (!quote(data->cmd))
		return (ft_error(1, NULL, data->no_path));
	// if (parsing_lexar(data, temp))
	// 	return (1);
	// data->cmd = remove_all_qoutes(data->cmd);
	str = ft_split(data->cmd, ' ');
	builtin_index = check_builtin(data->cmd);
	free_array(str);
	// if (ft_strchr(data->cmd, '|'))
	data->executor = parse_pipeline(data->cmd, data);
	if (builtin_index >= 0 && !check_redir_pipe(data->cmd))
		builtin_command(data->cmd, data);
	else
	{
		execution(data->executor, data);
	}
	// free_lexer_list(data->lexer_list);
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
	token = ft_split(cmd, '|');
	while (token[i])
	{
		executor = init_executor(data, token[i++]);
		if (head == NULL)
			head = executor;
		else
		{
			tail->next = executor;
			executor->prev = tail;
		}
		tail = executor;
	}
	free_array(token);
	return (head);
}

int	check_redir_pipe(char *cmd)
{
	int	i;

	i = 0;
	while (cmd[i])
	{
		if (cmd[i] == '<' || cmd[i] == '>' || cmd[i] == '|')
			return (1);
		i++;
	}
	return (0);
}
