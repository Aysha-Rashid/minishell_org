/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memchr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ayal-ras <ayal-ras@student.42abudhabi.a    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/07 17:37:05 by ayal-ras          #+#    #+#             */
/*   Updated: 2024/01/17 15:57:51 by ayal-ras         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

// s: This is a pointer to the object to be searched for.
// c: This is a character to search for.
// n: Max number of the byte which is examined to search c.
//  memchr library function finds the  occurrence of c in the initial
// n characters of the object pointed to by s.
#include "libft.h"

void	*ft_memchr(const void *s, int c, size_t n)
{
	unsigned char	*p;

	p = (unsigned char *)s;
	while (n > 0)
	{
		if (*p == (unsigned char)c)
			return (p);
		n--;
		p++;
	}
	return (NULL);
}
