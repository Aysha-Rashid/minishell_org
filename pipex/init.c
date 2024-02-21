/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ayal-ras <ayal-ras@student.42abudhabi.a    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/25 14:20:11 by ayal-ras          #+#    #+#             */
/*   Updated: 2024/02/08 14:27:42 by ayal-ras         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

void	init(t_data *data, char **argv)
{
	data->infile = open(argv[1], O_RDONLY);
	if (data->infile == -1)
		in_file_error(data->infile);
	data->cmd1 = ft_cmd(argv[2]);
	data->cmd2 = ft_cmd(argv[3]);
	data->outfile = open(argv[4], O_RDWR | O_TRUNC | O_CREAT, 0644);
	if (data->outfile == -1)
		file_error(data->outfile);
}

char	**ft_cmd(char *s)
{
	char	**str;

	if (!s)
		return (NULL);
	str = ft_split(s, ' ');
	if (!str)
		free_path(str);
	return (str);
}
