/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memmove.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ayal-ras <ayal-ras@student.42abudhabi.a    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/12 15:11:06 by ayal-ras          #+#    #+#             */
/*   Updated: 2023/12/24 19:44:29 by ayal-ras         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

//  The memmove() function copies len bytes from string src to string dst.
//  The two strings may overlap 
//  the copy is always done in a non-destructive manner.
#include "libft.h"

void	*ft_memmove(void	*dst, const void	*src, size_t	len)
{
	unsigned char		*dst1;
	unsigned char		*src1;

	if ((dst == NULL && src == NULL ))
		return (NULL);
	dst1 = (unsigned char *)dst;
	src1 = (unsigned char *)src;
	if (dst < src)
	{
		while (len--)
		{
			*dst1++ = *src1++;
		}
	}
	else
	{
		dst1 = ((unsigned char *)dst + len - 1);
		src1 = ((unsigned char *)src + len - 1);
		while (len--)
		{
			*dst1-- = *src1--;
		}
	}
	return (dst);
}
