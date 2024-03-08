/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_quotes.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ayal-ras <ayal-ras@student.42abudhabi.a    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/27 08:57:05 by zfiros-a          #+#    #+#             */
/*   Updated: 2024/03/08 13:22:47 by ayal-ras         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

char	*return_woqoutes(char *str, char del)
{
	char	*ptr;
	int		j;
	int		i;

	i = 0;
	j = 0;
	ptr = (char *)malloc(ft_strlen(str) + 1);
	if (ptr == NULL)
		return (NULL);
	while (str[j])
	{
		if (str[j] == del)
		{
			j++;
			i = 0;
			while (str[j] != del && str[j] != '\0')
				ptr[i++] = str[j++];
			ptr[i] = '\0';
			break ;
		}
		j++;
	}
	if (str[j] == '\0')
		ft_strcpy(ptr, str);
	return (ptr);
}

int	handle_quotes(int i, char *str, char del)
{
	int	j;

	j = 0;
	if (str[i + j] == del)
	{
		j++;
		while (str[i + j] != del && str[i + j])
			j++;
		j++;
	}
	return (j);
}

char	*remove_all_qoutes(char *str)
{
	char	*temp1;
	char	*temp2;

	temp1 = return_woqoutes(str, 34);
	temp2 = return_woqoutes(temp1, 39);
	free(temp1);
	if (temp2 == NULL)
		return (NULL); 
	str = temp2;
	return (str);
}
