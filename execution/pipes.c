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

void	free_executor(t_executor *executor)
{
	if (!executor)
		return ;
	free(executor->cmd);
	free_executor(executor->next);
	free(executor);
}

void	free_lexer_list(t_lexer *list)
{
	if (!list)
		return ;
	// free(list->str);
    free_lexer_list(list->next);
    free(list);
}

void	check_n_execute(char *str, t_data *data)
{
	if (!data->cmd || data->cmd[0] == '\0')
		return ;
	if (str && (!(ft_strcmp(str, "exit"))))
	{
		ft_putendl_fd("\033[0;32msee you around 😮‍💨!\033[0m", 1);
		ft_putendl_fd("exit", 1);
		// free_lexer_list(data->lexer_list);
		ft_free_all(data);
		exit(0);
	}
	if (!token_reader(data))
		ft_error(3, NULL, data->no_path);
	if (check_pipes_n_execute(data))
		return ;
}

void	count_pipes(t_lexer *lexer, t_data *data)
{
	t_lexer	*tmp;

	tmp = lexer;
	while (tmp)
	{
		if (tmp->token == PIPE)
			data->executor->pipes++;
		tmp = tmp->next;
	}
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
	ft_expansion(data);
	str = ft_split(data->cmd, ' ');
	builtin_index = check_builtin(str);
	free_array(str);
	data->executor = parse_pipeline(data->cmd, data);
	count_pipes(data->lexer_list, data);
	free_lexer_list(data->lexer_list);
	if (builtin_index >= 0 && data->executor->pipes == 0)
		builtin_command(data->cmd, data);
	else
		execution(data->executor, data);;
	free_executor(data->executor);
	return (0);
}
