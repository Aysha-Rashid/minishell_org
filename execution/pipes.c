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
	str = ft_split(data->cmd, ' ');
	builtin_index = check_builtin(data->cmd);
	free_array(str);
	data->executor = parse_pipeline(data->cmd, data);
	count_pipes(data->lexer_list, data);
	if (builtin_index >= 0 && data->executor->pipes == 0)
		builtin_command(data->cmd, data);
	else if (ft_expansion(data))
		ft_error(2, data->cmd, 0);
	else
	{
		execution(data->executor, data);
	}
	free_lexer_list(data->lexer_list);
	free_executor(data->executor);
	return (0);
}

int	check_line(t_executor *executor, t_data *data)
{
	(void)executor;
	while (data->lexer_list)
	{
		if (is_type(data->lexer_list, "IOH") && (!data->lexer_list->next
				|| is_type(data->lexer_list, "IOHAP")))
		{
			ft_putstr_fd("error", 1);
			return (0);
		}
		if (is_type(data->lexer_list, "PA") && (!data->lexer_list->prev
				|| is_type(data->lexer_list, "IOHAP")))
		{
			ft_putstr_fd("error", 1);
			return (0);
		}
		data->lexer_list = data->lexer_list->next;
	}
	return (1);
}

int	is_type(t_lexer *lexer, char *str)
{
	if (ft_strchr(str, 'P') && lexer->token == PIPE)
		return (1);
	if (ft_strchr(str, 'I') && lexer->token == INFILE)
		return (1);
	if (ft_strchr(str, 'O') && lexer->token == OUTFILE)
		return (1);
	if (ft_strchr(str, 'H') && lexer->token == HEREDOC)
		return (1);
	if (ft_strchr(str, 'A') && lexer->token == OUTEOF)
		return (1);
	return (0);
}
