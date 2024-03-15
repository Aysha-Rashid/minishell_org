/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strdup.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ayal-ras <ayal-ras@student.42abudhabi.a    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/13 18:46:36 by ayal-ras          #+#    #+#             */
/*   Updated: 2024/03/15 13:53:47 by ayal-ras         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

#include <stdlib.h>

char *ft_strdup(const char *s1)
{
    size_t len = ft_strlen(s1);
    char *ptr = (char *)malloc(sizeof(*ptr) * (len + 1));
    if (!ptr)
        return NULL;

    // Copy the string
    size_t i = 0;
    while (*s1)
        ptr[i++] = *s1++;
    ptr[i] = '\0';

    return ptr;
}

