/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipes.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ayal-ras <ayal-ras@student.42abudhabi.a    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/28 17:03:36 by ayal-ras          #+#    #+#             */
/*   Updated: 2024/02/29 15:45:04 by ayal-ras         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	check_n_execute(char *str, t_data *data)
{
	if (str && ft_strncmp(str, "exit", 5) == 0)
	{
		ft_putendl_fd("\033[0;32msee you around 😮‍💨!\033[0m", 1);
		ft_putendl_fd("exit", 1);
		free(data->cmd);
		free_env_list(data->envp);
		exit(0);
	}
	// check_redirection(data);
	check_pipes_n_execute(data);
}

void	check_pipes_n_execute(t_data *data)
{
	t_executor	*executor;
	t_lexer		*temp;

	temp = data->lexer_list;
	executor = (t_executor *)malloc(sizeof(t_executor));
	if (!executor)
		return ;
	executor->pipes = 0;
	executor->pid = 0;
	executor->heredoc = 0;
	while (temp)
	{
		if (temp->token == PIPE)
			executor->pipes++;
		temp = temp->next;
	}
	if (!executor->pipes)
		buitin_command(data->cmd, data);
	else
		execution (data->cmd, data);
	free(executor);
}
