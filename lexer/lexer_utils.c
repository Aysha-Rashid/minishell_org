/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ayal-ras <ayal-ras@student.42abudhabi.a    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/27 09:01:45 by zfiros-a          #+#    #+#             */
/*   Updated: 2024/03/03 19:22:10 by ayal-ras         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

t_lexer	*ft_lexernew(char *str, int token)
{
	t_lexer		*new_element;

	new_element = (t_lexer *)malloc(sizeof(t_lexer));
	if (!new_element)
		return (0);
	new_element->str = str;
	new_element->token = token;
	new_element->next = NULL;
	new_element->prev = NULL;
	return (new_element);
}

int	ft_lexeradd_back(t_lexer **list, t_lexer *new)
{
	t_lexer	*temp;

	temp = *list;
	if (temp == NULL)
	{
		*list = new;
		return (1);
	}
	while (temp != NULL && temp->next != NULL)
		temp = temp->next;
	temp->next = new;
	new->prev = temp;
	new->next = NULL;
	return (0);
}
