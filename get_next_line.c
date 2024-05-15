/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ayal-ras <ayal-ras@student.42abudhabi.a    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/23 18:23:58 by ayal-ras          #+#    #+#             */
/*   Updated: 2024/03/23 18:30:54 by ayal-ras         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*get_next_line(int fd, char *delimiter)
{
	static char	*buffer;
	char		*line;

	if (fd < 0 || BUFFER_SIZE <= 0 || BUFFER_SIZE >= 2147483647)
		return (NULL);
	buffer = read_lines(fd, buffer);
	if (!buffer)
		return (NULL);
	line = new_line(buffer);
	buffer = remaining(buffer);
	if (line && line[0] != '\0')
	{
		if (!ft_strcmp(line, delimiter))
			return (free(buffer), line);
	}
	return (line);
}

char	*read_lines(int fd, char *store)
{
	char	*buffer;
	int		bytes;

	bytes = 1;
	buffer = malloc(BUFFER_SIZE);
	if (store == NULL)
		store = ft_get_strdup("");
	while (bytes > 0)
	{
		bytes = read(fd, buffer, BUFFER_SIZE);
		if (bytes == -1)
		{
			free(store);
			free(buffer);
			return (NULL);
		}
		buffer[bytes] = '\0';
		store = ft_get_strjoin(store, buffer);
		if (ft_get_strchr(store, '\n') != NULL)
			break ;
	}
	return (free(buffer), store);
}

char	*new_line(char *str)
{
	int		i;
	char	*new_line;

	i = 0;
	if (!str[i])
		return (NULL);
	while (str[i] && str[i] != '\n')
		i++;
	new_line = (char *)malloc(sizeof(char) * (i + 2));
	if (!new_line)
		return (NULL);
	i = 0;
	while (str[i] && str[i] != '\n')
	{
		new_line[i] = str[i];
		i++;
	}
	if (str[i] == '\n')
	{
		new_line[i] = str[i];
		i++;
	}
	new_line[i] = '\0';
	return (new_line);
}

char	*remaining(char *buffer)
{
	char		*line;
	int			i;
	int			j;

	i = 0;
	j = 0;
	line = NULL;
	while (buffer[i] != '\0' && buffer[i] != '\n')
		i++;
	if (buffer[i] == '\0')
	{
		free(buffer);
		return (NULL);
	}
	line = malloc(ft_strlen(buffer) - i);
	i++;
	if (line == NULL)
		return (NULL);
	while (buffer[i] != '\0')
		line[j++] = buffer[i++];
	line[j] = '\0';
	free(buffer);
	return (line);
}
