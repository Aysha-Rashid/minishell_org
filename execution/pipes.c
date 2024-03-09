/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipes.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ayal-ras <ayal-ras@student.42abudhabi.a    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/28 17:03:36 by ayal-ras          #+#    #+#             */
/*   Updated: 2024/03/09 19:43:23 by ayal-ras         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	check_n_execute(char *str, t_data *data)
{
	if (!data->cmd || data->cmd[0] == '\0')
		return ;
	if (str && (!ft_strncmp(str, "exit", 5)))
	{
		ft_putendl_fd("\033[0;32msee you around 😮‍💨!\033[0m", 1);
		ft_putendl_fd("exit", 1);
		ft_free_all(data);
		exit(0);
	}
	if (!token_reader(data))
		ft_error(3, NULL, data->no_path);
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
	t_lexer		*temp;

	temp = data->lexer_list;
	init_executor(data);
	count_pipes(temp, data->executor->pipes);
	if (parsing_lexar(data, temp))
		return (0);
	// ft_putnbr_fd(temp->token, 1);
	if (data->lexer_list->token == 0 && data->lexer_list->token != 1)
		return (simple_cmd(data->cmd, data), free(data->executor), 1);
	data->executor->pid = ft_calloc(sizeof(int), data->executor->pipes + 2);
	if (!data->executor->pid)
		return (ft_error(3, NULL, data->no_path), free(data->executor), 1);
	// if (data->lexer_list->token)
	// {
	// 	execution (data->cmd, data, data->executor);
	// 	// exit(0);
	// 	// write(1, "comes", 5);
	// 	// exit(0);
	// 	// wait(NULL);
	// }
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


// void	expansion(t_data *data, char *cmd)
// {
	
// }
// found a $ sign then print the environment variable of it
// found a ? sign then print the exit status of the previous command
