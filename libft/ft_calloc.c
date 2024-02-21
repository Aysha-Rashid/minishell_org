/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_calloc.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ayal-ras <ayal-ras@student.42abudhabi.a    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/13 18:30:22 by ayal-ras          #+#    #+#             */
/*   Updated: 2023/12/30 15:29:36 by ayal-ras         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_calloc(size_t	count, size_t	size)
{
	char	*ptr;

	if (size != 0 && count > __UINT32_MAX__ / size)
		return (NULL);
	ptr = malloc (count * size);
	if (ptr == 0)
	{
		return (ptr);
	}
	ft_bzero (ptr, count * size);
	return (ptr);
}
