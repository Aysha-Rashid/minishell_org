/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution_utils.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ayal-ras <ayal-ras@student.42abudhabi.a    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/21 14:40:08 by ayal-ras          #+#    #+#             */
/*   Updated: 2024/03/22 15:02:57 by ayal-ras         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	check_command(char *str, char *cmd, int *end, t_data *data)
{
	if (ft_strchr(cmd, '$') || ft_strchr(cmd, '?'))
	{
		ft_expansion3(data, str, 0);
		free(str);
		exit_and_free(data, end, 1);
	}
	if (data->no_path)
	{
		free(str);
		ft_error(2, cmd, data->no_path);
		exit_and_free(data, end, 0);
	}
	if (check_builtin(str) >= 0)
	{
		if (builtin_command(str, data))
		{
			free(str);
			exit_and_free(data, end, 0);
		}
		free(str);
		exit_and_free(data, end, 1);
	}
}

// char	*remove_all_qoutes(char *str)
// {
// 	char	*ptr;
// 	int		i;
// 	int		j;

// 	if (!str || str[0] == '\0')
// 		return (NULL);
// 	ptr = malloc(ft_strlen(str) + 1);
// 	if (ptr == NULL)
// 		return (NULL);
// 	i = 0;
// 	j = 0;
// 	while (str[j] != ' ' && str[j])
// 	{
// 		if (str[j] != '\'' && str[j] != '"')
// 			ptr[i++] = str[j];
// 		j++;
// 	}
// 	while (str[j])
// 		ptr[i++] = str[j++];
// 	ptr[i] = '\0';
// 	return (ptr);
// }
char *remove_all_qoutes(char *str)
{
    char *ptr;
    int i = 0;
    int j = 0;
    int len = ft_strlen(str);

    if (!str || str[0] == '\0')
        return NULL;
    ptr = malloc(len + 1);
    if (ptr == NULL)
        return NULL;
    if (str[0] == '\'' || str[0] == '"')
        j++;
    while (str[j] != '\0' && j < len - 1)
        ptr[i++] = str[j++];
    if (str[len - 1] == '\'' || str[len - 1] == '"')
        j++;
    ptr[i] = '\0';

    return ptr;
}

char	*remove_quotes(char *str)
{
	int		i;
	char	quote;
	int		len;
	char	*ptr;
	int		j;

	i = 0;
	j = 0;
	if (!str)
		return (0);
	len = ft_strlen(str);
	ptr = malloc(len + 1);
	while (i < len)
	{
		if (str[i] == '\'' || str[i] == '"')
		{
			quote = str[i++];
			while (ft_isalpha(str[i]) && i < len && str[i] != quote)
				ptr[j++] = str[i++];
			if (i < len)
				i++;
		}
		else
			ptr[j++] = str[i++];
	}
	ptr[j] = '\0';
	return (ptr);
}
