/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipes.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ayal-ras <ayal-ras@student.42abudhabi.a    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/28 17:03:36 by ayal-ras          #+#    #+#             */
/*   Updated: 2024/03/01 19:50:29 by ayal-ras         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	check_n_execute(char *str, t_data *data)
{
	if (str && ft_strncmp(str, "exit", 5) == 0)
	{
		ft_putendl_fd("\033[0;32msee you around 😮‍💨!\033[0m", 1);
		ft_putendl_fd("exit", 1);
		free_env_list(data->envp);
		free(data->cmd);
		exit(0);
	}
	if (check_pipes_n_execute(data))
		return ;
}

void	free_lexer_list(t_lexer **list)
{
    t_lexer *current = *list;
    t_lexer *next;

    while (current != NULL) {
        next = current->next;  // Save the reference to the next node
        free(current->str);    // Free the dynamically allocated string
        free(current);         // Free the current node
        current = next;        // Move to the next node
    }
    *list = NULL;  // Set the list pointer to NULL after freeing all nodes
}

int	check_pipes_n_execute(t_data *data)
{
	t_executor	*executor;
	t_lexer		*temp;

	temp = data->lexer_list;
	executor = (t_executor *)malloc(sizeof(t_executor));
	if (!executor)
		return (ft_error(3), 1);
	executor->pipes = 0;
	executor->pid = 0;
	executor->heredoc = 0;
	while (temp)
	{
		if (temp->token == PIPE)
			executor->pipes++;
		temp = temp->next;
	}
	if (!*data->cmd)
		prompt_loop(data->cmd, data);
	else if (data->lexer_list->token == 0)
		buitin_command(data->cmd, data);
	else
	{
		executor->pid = ft_calloc(sizeof(int), executor->pipes);
		if (!executor->pid)
			return (ft_error(3), free(executor), 1);
		execution (data->cmd, data);
	}
	free(executor);
	return (0);
}
