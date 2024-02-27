/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ayal-ras <ayal-ras@student.42abudhabi.a    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/27 09:01:45 by zfiros-a          #+#    #+#             */
/*   Updated: 2024/02/27 10:53:14 by ayal-ras         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

t_lexer	*ft_lexernew(char *str, int token)
{
	t_lexer		*new_element;
	static int	i = 0;

	new_element = (t_lexer *)malloc(sizeof(t_lexer));
	if (!new_element)
		return (0);
	new_element->str = str;
	new_element->token = token;
	new_element->i = i++;
	new_element->next = NULL;
	new_element->prev = NULL;
	return (new_element);
}

void	ft_lexeradd_back(t_lexer **list, t_lexer *new)
{
	t_lexer	*temp;

	temp = *list;
	if (*list == NULL)
	{
		*list = new;
		return ;
	}
	while (temp->next != NULL)
		temp = temp->next;
	temp->next = new;
	new->prev = temp;
}
