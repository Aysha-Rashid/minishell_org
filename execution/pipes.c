/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipes.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zfiros-a <zfiros-a@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/28 17:03:36 by ayal-ras          #+#    #+#             */
/*   Updated: 2024/03/15 15:31:04 by zfiros-a         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	check_n_execute(char *str, t_data *data)
{
	if (!str || str[0] == '\0')
		prompt_loop(str, data);
	if (str && (!(ft_strcmp(str, "exit"))))
	{
		ft_putendl_fd("\033[0;32msee you around 😮‍💨!\033[0m", 1);
		ft_putendl_fd("exit", 1);
		free_array(data->envp->path);
		ft_free_all(data);
		exit(0);
	}
	if (check_pipes_n_execute(data))
		return ;
}

int	check_pipes_n_execute(t_data *data)
{
	char		builtin_index;
	char		*temp;

	if (!quote(data->cmd))
		return (ft_error(1, NULL, data->no_path));
	temp = remove_quotes(data->cmd);
	data->executor = parse_pipeline(temp, data);
	if (parse_com(temp))
		return (free(temp), 1);
	builtin_index = check_builtin(temp);
	if (builtin_index >= 0 && !check_redir_pipe(temp))
		builtin_command(temp, data);
	else
	{
		free(temp);
		execution(data->executor, data);
	}
	free_executor(data->executor);
	return (0);
}

t_executor	*parse_pipeline(char *cmd, t_data *data)
{
	t_executor	*head;
	t_executor	*tail;
	t_executor	*executor;
	char		**token;
	int			i;

	i = 0;
	head = NULL;
	tail = NULL;
	if (ft_strchr(cmd, '\'') || ft_strchr(cmd, '\"'))
		return (init_executor(data, cmd));
	token = ft_split(cmd, '|');
	while (token[i])
	{
		executor = init_executor(data, token[i]);
		executor->prev = tail;
		i++;
		if (head == NULL)
			head = executor;
		else
			tail->next = executor;
		tail = executor;
	}
	free_array(token);
	return (head);
}

int	check_redir_pipe(char *cmd)
{
	int	i;

	i = 0;
	while (cmd[i])
	{
		if (cmd[i] == '<' || cmd[i] == '>' || cmd[i] == '|')
			return (1);
		i++;
	}
	return (0);
}

char	*remove_redir_or_files(char *cmd)
{
	char	*dest;
	int		i;
	int		j;

	i = 0;
	j = 0;
	dest = (char *)malloc(sizeof(char) * (ft_strlen(cmd) + 1));
	if (!dest)
		return (NULL);
	while (cmd[i])
	{
		if ((cmd[i] == '<' || cmd[i] == '>') && cmd[i] != ' ')
		{
			i++;
			if (cmd[i] == '>' || cmd[i] == '<')
				i++;
			while (cmd[i] && (cmd[i] == ' ' || cmd[i] == '\t'))
				i++;
			while (cmd[i] && cmd[i] != ' ' && cmd[i] != '\t')
				i++;
		}
		dest[j++] = cmd[i++];
	}
	dest[j] = '\0';
	return (dest);
}
