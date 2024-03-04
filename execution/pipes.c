/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipes.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ayal-ras <ayal-ras@student.42abudhabi.a    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/28 17:03:36 by ayal-ras          #+#    #+#             */
/*   Updated: 2024/03/04 14:34:22 by ayal-ras         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	check_n_execute(char *str, t_data *data)
{
	if (!token_reader(data))
		ft_error(3, NULL, data);
	if (str && ft_strncmp(str, "exit", 5) == 0)
	{
		ft_putendl_fd("\033[0;32msee you around 😮‍💨!\033[0m", 1);
		ft_putendl_fd("exit", 1);
		free(data->pwd);
		free(data->old_pwd);
		free_env_list(data->envp);
		free(data->cmd);
		exit(0);
	}
	if (check_pipes_n_execute(data))
		return ;
}

void	free_lexer_list(t_lexer *list)
{
	t_lexer	*current;
	t_lexer	*next;

	current = list;
	while (current != NULL)
	{
		next = current->next;
		free(current->str);
		free(current);
		current = next;
	}
	list = NULL;
}

int	check_pipes_n_execute(t_data *data)
{
	t_executor	*executor;
	t_lexer		*temp;

	temp = data->lexer_list;
	executor = (t_executor *)malloc(sizeof(t_executor));
	if (!executor)
		return (ft_error(3, NULL, data), 1);
	executor->pipes = 0;
	executor->heredoc = 0;
	executor->in = 0;
	executor->out = 0;
	count_pipes(temp, executor->pipes);
	if (!*data->cmd)
		return (free(executor), 0);
	else if (data->lexer_list->token == 0)
		buitin_command(data->cmd, data);
	else
	{
		executor->pid = ft_calloc(sizeof(int), executor->pipes + 2);
		if (!executor->pid)
			return (ft_error(3, NULL, data), free(executor), 1);
		execution (data->cmd, data);
		free(executor->pid);
	}
	return (free(executor), 0);
}

void	count_pipes(t_lexer *lexer, int pipes)
{
	while (lexer)
	{
		if (lexer->token == PIPE)
			pipes++;
		lexer = lexer->next;
	}
}
