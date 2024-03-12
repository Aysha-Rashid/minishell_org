/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipes.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ayal-ras <ayal-ras@student.42abudhabi.a    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/28 17:03:36 by ayal-ras          #+#    #+#             */
/*   Updated: 2024/03/12 14:18:39 by ayal-ras         ###   ########.fr       */
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
	char		builtin_index;
	char		**str;

	temp = data->lexer_list;
	if (parsing_lexar(data, temp))
		return (1);
	init_executor(data);
	count_pipes(temp, data);
	str = ft_split(data->cmd, ' ');
	builtin_index = check_builtin(str);
	free_array(str);
	if (builtin_index)
		builtin_command(data->cmd, data);
	else
		execution(data);
	// data->executor->pid = ft_calloc(sizeof(int), data->executor->pipes + 2);
	// if (!data->executor->pid)
		// return (ft_error(3, NULL, data->no_path), free(data->executor), 1);
	// waitpid(-1, &data->status_code, 0);
	// free(data->executor->pid);
	return (0);
}

void	count_pipes(t_lexer *lexer, t_data *data)
{
	int	num_pipe;

	num_pipe = 0;
	while (lexer)
	{
		if (lexer->token == PIPE)
			num_pipe++;
		lexer = lexer->next;
	}
	data->executor->pipes = num_pipe;
}
