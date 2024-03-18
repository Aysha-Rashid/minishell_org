/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ayal-ras <ayal-ras@student.42abudhabi.a    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/27 08:41:59 by zfiros-a          #+#    #+#             */
/*   Updated: 2024/03/12 11:42:41 by ayal-ras         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

t_tokens	check_token(int n)
{
	static int	token_array[3][2] = {
	{'|', PIPE},
	{'>', OUTFILE},
	{'<', INFILE},
	};
	int			i;

	i = 0;
	while (i < 3)
	{
		if (token_array[i][0] == n)
			return (token_array[i][1]);
		i++;
	}
	return (0);
}

int	handle_token(char *str, int i, t_lexer **lexer_list)
{
	t_tokens	token;

	token = check_token(str[i]);
	if (token == OUTFILE && check_token(str[i + 1] == OUTFILE))
	{
		if (!add_node(NULL, APPEND, lexer_list))
			return (-1);
		return (2);
	}
	else if (token == INFILE && str[i + 1] == INFILE)
	{
		if (!add_node(NULL, HEREDOC, lexer_list))
			return (-1);
		return (2);
	}
	else if (token)
	{
		if (!add_node(NULL, token, lexer_list))
			return (-1);
		return (1);
	}
	return (0);
}
