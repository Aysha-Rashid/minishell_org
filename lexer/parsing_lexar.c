/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing_lexar.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ayal-ras <ayal-ras@student.42abudhabi.a    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/06 21:00:41 by ayal-ras          #+#    #+#             */
/*   Updated: 2024/03/09 20:53:04 by ayal-ras         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int    parsing_lexar(t_data *data, t_lexer *lexar)
{
	char **str;
	size_t	len;
	int		heredoc;

	data->executor->in = 0;
	str = ft_split(data->cmd, ' ');
	len = size_of_env(str);
	if (lexar->token == PIPE)
		return (double_token_error(lexar, data->cmd));
	if (lexar->token == INFILE)
	{
		data->executor->in = open(str[1], O_RDONLY);
		if (str[1] != NULL)
			double_token_error(lexar, str[1]);
		if (data->executor->in == -1)
			ft_error(2, str[1], 1);
	}
	if (lexar->token == OUTFILE)
	{
		data->executor->out = open(str[len - 1], O_CREAT | O_RDWR | O_TRUNC, 0644);
		if (str[1] != NULL)
			double_token_error(lexar, str[1]);
		if (data->executor->out == -1)
			ft_error(4, str[1], 0);
	}
	if (lexar->token == OUTEOF)
	{
		data->executor->out = open(str[len - 1 ], O_CREAT | O_RDWR | O_APPEND, 0644);
		if (double_token_error(lexar, str[1]))
			return (1);
		if (data->executor->out == -1)
			ft_error(4, str[1], 0);
	}
	if (lexar->token == HEREDOC)
	{
		heredoc = open(, O_CREAT | O_RDWR | O_APPEND, 0644);
		if (double_token_error(lexar, str[1]))
			return (1);
		if (data->executor->out == -1)
			ft_error(4, str[1], 0);
	}
	free_array(str);
	return (0);
}

int	double_token_error(t_lexer *lexar, char *str)
{
	// ft_putstr_fd("minishell: syntax error near unexpected token ",
	// 	2);
	if (str)
	{
		if (lexar->token == INFILE && str[0] == INFILE)
			ft_putstr_fd("minishell: syntax error near unexpected token '>'\n", STDERR_FILENO);
		else if (lexar->token == OUTFILE && str[0] == OUTFILE)
			ft_putstr_fd("minishell: syntax error near unexpected token '<'\n", STDERR_FILENO);
		else if (lexar->token == HEREDOC)
			ft_putstr_fd("minishell: syntax error near unexpected token '>>'\n", STDERR_FILENO);
		else if (lexar->token == OUTEOF)
			ft_putstr_fd("minishell: syntax error near unexpected token '<<'\n", STDERR_FILENO);
		else if (lexar->token == PIPE)
			ft_putstr_fd("minishell: syntax error near unexpected token '|'\n", STDERR_FILENO);
		// else
		// 	ft_putstr_fd("\n", STDERR_FILENO);
		return (0);
	}
	// ft_lexerclear(&lexar);
	return (0);
}

void	ft_lexerclear(t_lexer **lst)
{
	t_lexer	*tmp;

	if (!*lst)
		return ;
	while (*lst)
	{
		tmp = (*lst)->next;
		if ((*lst)->str)
			free((*lst)->str);
		free(*lst);
		*lst = tmp;
	}
	*lst = NULL;
}
