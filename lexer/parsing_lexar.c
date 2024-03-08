/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing_lexar.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ayal-ras <ayal-ras@student.42abudhabi.a    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/06 21:00:41 by ayal-ras          #+#    #+#             */
/*   Updated: 2024/03/07 20:30:25 by ayal-ras         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int    parsing_lexar(t_data *data, t_lexer *lexar)
{
	(void)data;
	char **str;
	size_t	len;

	data->executor->in = 0;
	str = ft_split(data->cmd, ' ');
	len = size_of_env(str);
	// t_lexer	*parsing_tools;
	if (lexar->token == PIPE)
		return (double_token_error(lexar));
	// while (lexar)
	// {
		// parsing_tools = init_parse(lexar, data);
		if (lexar->token == INFILE)
		{
			// if (double_token_error(l))
			data->executor->in = open(str[1], O_RDONLY);
			if (data->executor->in == -1)
				write(2, "no infile", 9);
		}
		if (lexar->token == OUTFILE)
		{
			// if (double_token_error(l))
			ft_putnbr_fd(len, 1);
			data->executor->out = open(str[len - 1], O_CREAT | O_RDWR | O_TRUNC, 0644);
			if (data->executor->out == -1)
				write(2, "no out", 6);
		}
		if (lexar->token == OUTEOF)
		{
			// if (double_token_error(l))
			data->executor->out = open(str[len - 1 ], O_CREAT | O_RDWR | O_APPEND, 0644);
			if (data->executor->out == -1)
				write(2, "1", 1);
		}
	// }
	free_array(str);
	return (0);
}

int	double_token_error(t_lexer *lexar)
{
	ft_putstr_fd("minishell: syntax error near unexpected token ",
		2);
	if (lexar->token == INFILE)
		ft_putstr_fd("'>'\n", STDERR_FILENO);
	else if (lexar->token == OUTFILE)
		ft_putstr_fd("'<'\n", STDERR_FILENO);
	else if (lexar->token == HEREDOC)
		ft_putstr_fd("'>>'\n", STDERR_FILENO);
	else if (lexar->token == OUTEOF)
		ft_putstr_fd("'<<'\n", STDERR_FILENO);
	else if (lexar->token == PIPE)
		ft_putstr_fd("'|'\n", STDERR_FILENO);
	else
		ft_putstr_fd("\n", STDERR_FILENO);
	ft_lexerclear(&lexar);
	return (1);
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
