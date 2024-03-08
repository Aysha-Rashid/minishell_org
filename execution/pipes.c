/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipes.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ayal-ras <ayal-ras@student.42abudhabi.a    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/28 17:03:36 by ayal-ras          #+#    #+#             */
/*   Updated: 2024/03/07 19:42:57 by ayal-ras         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	check_n_execute(char *str, t_data *data)
{
	if (str && (!ft_strncmp(str, "exit", 5)))
	{
		ft_putendl_fd("\033[0;32msee you around 😮‍💨!\033[0m", 1);
		ft_putendl_fd("exit", 1);
		free(data->old_pwd);
		free(data->pwd);
		free_env_list(data->envp);
		// free_lexer_list(data->lexer_list);
		free(data->cmd);
		exit(0);
	}
	if (!token_reader(data))
		ft_error(3, NULL, data);
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
	// t_executor	*executor;
	t_lexer		*temp;

	temp = data->lexer_list;
	data->executor = (t_executor *)malloc(sizeof(t_executor));
	if (!data->executor)
		return (ft_error(3, NULL, data), 1);
	data->executor->pipes = 0;
	data->executor->heredoc = 0;
	data->executor->in = 0;
	data->executor->out = 0;
	count_pipes(temp, data->executor->pipes);
	if (parsing_lexar(data, temp))
		return (0);
	data->executor->pid = ft_calloc(sizeof(int), data->executor->pipes + 2);
	if (!data->executor->pid)
		return (ft_error(3, NULL, data), free(data->executor), 1);
	if (!*data->cmd)
		return (free(data->executor), 0);
	if (data->lexer_list->token == 0)
		buitin_command(data->cmd, data);
	else if (data->lexer_list->token)
	{
		// write(1, "comeshere", 9);
		execution (data->cmd, data, data->executor);
		// exit(0);
		// write(1, "comes", 5);
		// exit(0);
		// wait(NULL);
	}
	free(data->executor->pid);
	return (free(data->executor), 0);
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
