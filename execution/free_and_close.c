/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free_and_close.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ayal-ras <ayal-ras@student.42abudhabi.a    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/16 11:40:59 by ayal-ras          #+#    #+#             */
/*   Updated: 2024/03/16 13:09:20 by ayal-ras         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	free_executor(t_executor *executor)
{
	if (!executor)
		return ;
	free(executor->cmd);
	free_executor(executor->next);
	// free_executor(executor->prev);
	free(executor);
}

void	close_and_free_all(t_data *data, int *end)
{
	(void)end;
	// close(data->executor->in);
	// close(data->executor->out);
	// free_lexer_list(data->lexer_list);
	free_array(data->envp->path);
	free_executor(data->executor);
	// free(data->executor->next->cmd);
	ft_free_all(data);
}

void	dup_check(int file, int dupped)
{
	if (dup2(file, dupped) == -1)
	{
		ft_putstr_fd("dup error", 2);
		exit(1);
	}
	close(file);
}

void	exit_and_free(t_data *data, int *end, int status)
{
	close_and_free_all(data, end);
	exit(status);
}
