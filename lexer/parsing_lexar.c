/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing_lexar.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ayal-ras <ayal-ras@student.42abudhabi.a    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/06 21:00:41 by ayal-ras          #+#    #+#             */
/*   Updated: 2024/03/10 21:31:06 by ayal-ras         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int parsing_lexar(t_data *data, t_lexer *lexar)
{
	char	**str;
	size_t	len;

	data->executor->in = 0;
	str = ft_split(data->cmd, ' ');
	len = size_of_env(str);
	if (ft_strcmp(str[0], "|") == 0 || ft_strcmp(str[1], "|") == 0 || ft_strcmp(str[0], "<<") == 0)
	{
		// write(1, "here", 4);
		// exit(0);
		return (double_token_error(lexar, data->cmd), 1);
	}
	if ((ft_strcmp(str[0], "<") == 0) && len >= 1)
	{
		if (str[1] == NULL)
		{
			if (double_token_error(lexar, "<"))
				return (1);
		}
		else
		{
			if (double_token_error(lexar, str[1]))
				return (1);
			else
			{
				data->executor->in = open(str[1], O_RDONLY);
				if (data->executor->in == -1)
					return (ft_error(2, str[1], 1), 1);
			}
		}
	}
	else if ((ft_strcmp(str[0], ">") == 0 && len >= 1) || (ft_strcmp(str[0], ">>") == 0 && len >= 2))
	{
		if (str[len - 1] != NULL)
		{
			if (double_token_error(lexar, str[len - 1]))
				return (1);
			else
			{
				int flags = O_CREAT | O_RDWR | (lexar->token == OUTEOF ? O_APPEND : O_TRUNC);
				data->executor->out = open(str[len - 1], flags, 0644);
				if (data->executor->out == -1)
					return (ft_error(4, str[len - 1], 0), 1);
			}
		}
	}
	if (ft_strcmp(str[0], "|") == 0 || ft_strcmp(str[0], "<<") == 0)
		return (double_token_error(lexar, data->cmd));
	free_array(str);
	return (0);
}
#include <fcntl.h> // For file operations
#include <unistd.h> // For file descriptors
#include <stdio.h> // For standard I/O functions

// int parsing_lexar(t_data *data, t_lexer *lexar)
// {
// 	(void)lexar;
//     char **tokens;
//     size_t num_tokens;

//     // Split input command into tokens
//     tokens = ft_split(data->cmd, ' ');
//     num_tokens = size_of_env(tokens);

//     // Loop through tokens to handle redirections and pipes
//     for (size_t i = 0; i < num_tokens; i++) {
//         if (strcmp(tokens[i], "<") == 0 && i < num_tokens - 1) {
//             // Handle input redirection
//             int fd = open(tokens[i + 1], O_RDONLY);
//             if (fd == -1) {
//                 perror("open");
//                 return 1;
//             }
//             dup2(fd, STDIN_FILENO);
//             close(fd);
//             i++; // Skip next token (file name)
//         } else if ((strcmp(tokens[i], ">") == 0 || strcmp(tokens[i], ">>") == 0) && i < num_tokens - 1) {
//             // Handle output redirection
//             int flags = O_CREAT | O_WRONLY | (strcmp(tokens[i], ">>") == 0 ? O_APPEND : O_TRUNC);
//             int fd = open(tokens[i + 1], flags, 0644);
//             if (fd == -1) {
//                 perror("open");
//                 return 1;
//             }
//             dup2(fd, STDOUT_FILENO);
//             close(fd);
//             i++; // Skip next token (file name)
//         } else if (strcmp(tokens[i], "|") == 0)
// 		{
			
//             // Handle pipe
//             // (You will need to implement this part)
//         }
//     }

//     // Execute command with modified file descriptors
//     // exec_cmd(data->cmd, data);

//     // Free memory allocated for tokens
//     free_array(tokens);

//     return 0;
// }


int	double_token_error(t_lexer *lexar, char *str)
{
	char	*error;
	(void)lexar;
	error = "";
	if (ft_strcmp(str, "<") == 0 || ft_strcmp(str, ">") == 0
		|| ft_strcmp(str, "<<") == 0 || ft_strcmp(str, "|") == 0)
	{
		if (ft_strcmp(str, "<") == 0)
			error = "`<'";
		else if (ft_strcmp(str, ">") == 0)
			error = "`>'";
		else if (ft_strcmp(str, "<<") == 0)
			error = "`<<'";
		else if (ft_strcmp(str, ">>") == 0)
			error = "`>>'";
		else if (ft_strcmp(str, "|") == 0)
			error = "`|'";
		else
			error = "`newline'";
		name_error("syntax error near unexpected token ", NULL, error);
		return (1);
	}
	return (0);
}



void	ft_lexerclear(t_lexer **lst)
{
	t_lexer	*tmp;

	if (!*lst)
		return ;
	while (*lst)
	{
		tmp = (*lst)->next;
		if ((*lst)->str)
			free((*lst)->str);
		free(*lst);
		*lst = tmp;
	}
	*lst = NULL;
}
