/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ayal-ras <ayal-ras@student.42abudhabi.a    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/08 17:27:11 by ayal-ras          #+#    #+#             */
/*   Updated: 2024/03/13 14:35:12 by ayal-ras         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int 	g_sig_interrupt = 0;

t_executor *init_executor(t_data *data, char *cmd)
{
	(void)data;
	t_executor *executor;
    executor = (t_executor *)malloc(sizeof(t_executor));
	if (!executor)
		return (0);
	// executor = executor;
    executor->cmd = cmd;
    executor->pipes = 0;
    executor->heredoc = 0;
    executor->in = 0;
    executor->out = 0;
    // executor->pid = 0;
    executor->next = NULL;
    executor->prev = NULL;
    ft_simple_cmdsadd_back(&executor, executor);
	return executor;
}

void	child_signals(int signum)
{
	if (signum == SIGINT)
	{
		ft_putchar_fd('\n', STDOUT_FILENO);
		// g_exit_status = 130;
		exit(130);
	}
}

void	ft_lexerdel_first(t_lexer **lst)
{
	t_lexer	*node;

	node = *lst;
	*lst = node->next;
	ft_lexerclear_one(&node);
	if (*lst)
		(*lst)->prev = NULL;
}

t_lexer	*ft_lexerclear_one(t_lexer **lst)
{
	if ((*lst)->str)
	{
		free((*lst)->str);
		(*lst)->str = NULL;
	}
	free(*lst);
	*lst = NULL;
	return (NULL);
}

// void	ft_lexerdel_first(t_lexer **lst)
// {
// 	t_lexer	*node;

// 	node = *lst;
// 	*lst = node->next;
// 	ft_lexerclear_one(&node);
// 	if (*lst)
// 		(*lst)->prev = NULL;
// }
void	ft_lexerdelone(t_lexer **lst, int key)
{
	t_lexer	*node;
	t_lexer	*prev;
	t_lexer	*start;

	start = *lst;
	node = start;
	if ((*lst)->i == key)
	{
		ft_lexerdel_first(lst);
		return ;
	}
	while (node && node->i != key)
	{
		prev = node;
		node = node->next;
	}
	if (node)
		prev->next = node->next;
	else
		prev->next = NULL;
	if (prev->next)
		prev->next->prev = prev;
	ft_lexerclear_one(&node);
	*lst = start;
}

void execution(t_data *data)
{
	int	end[2];
	int	fd_in;
	t_executor *executor_node;

	// init_executor(data, data->cmd);
	fd_in = STDIN_FILENO;
	while (data->lexer_list)
	{
		if (data->lexer_list && data->lexer_list->token == PIPE)
			ft_lexerdelone(&data->lexer_list, data->lexer_list->i);
		executor_node =  initialize_cmd(data);
		// write(1, "hell", 4);
		count_pipes(data->lexer_list, data);
		pipe(end);
		ft_fork(data, end, fd_in, data->cmd);
		close(end[1]);
		fd_in = in_or_heredoc(data, end, data->executor);
		if (data->executor->prev)
			close(fd_in);
		if (data->executor->next)
			data->executor = data->executor->next;
		else
			break ;
		waitpid(-1, &data->status_code, 0);
	}
		// fd_in = check_fd_heredoc(tools, end, tools->simple_cmds);
	// exit(0);
}
t_executor	*initialize_cmd(t_data *parser_tools)
{
	char	**str;
	int		i;
	int		arg_size;
	t_lexer	*tmp;

	i = 0;
	// rm_redirections(parser_tools);
		write(1, "work", 4);
	arg_size = count_args(parser_tools->lexer_list);
	str = ft_calloc(arg_size + 1, sizeof(char *));
	// if (!str)
	// 	parser_error(1, parser_tools->tools, parser_tools->lexer_list);
	tmp = parser_tools->lexer_list;
	// ft_putstr_fd(tmp->str, 1);
	while (arg_size > 0)
	{
		if (tmp->str)
		{
			str[i++] = ft_strdup(tmp->str);
			ft_lexerdelone(&parser_tools->lexer_list, tmp->i);
			tmp = parser_tools->lexer_list;
		}
		arg_size--;
	}
	return (init_executor(parser_tools, parser_tools->cmd));
}

int	count_args(t_lexer *lexer_list)
{
	t_lexer	*tmp;
	int		i;

	i = 0;
	tmp = lexer_list;
	while (tmp && tmp->token != PIPE)
	{
		if (tmp->i >= 0)
			i++;
		tmp = tmp->next;
	}
	return (i);
}

int in_or_heredoc(t_data *data, int *end, t_executor *executor)
{
	(void)data;
	(void)executor;
	int fd_in;

	fd_in = end[0];
	return (fd_in);
}

int	ft_fork(t_data *data, int end[2], int fd_in, char *cmd)
{
	static int	i = 0;

	// if (data->reset == true)
	// {
	// 	i = 0;
	// 	data->reset = false;
	// }
	data->pid = 0;
	data->pid = calloc(sizeof(int), data->executor->pipes + 2);
	(void)cmd;
	data->pid[i] = fork();
	if (data->pid[i] < 0)
		ft_error(5, "pid error", data->no_path);
	if (data->pid[i] == 0)
		dup_cmd(data->executor, data, end, fd_in);
	i++;
	return (EXIT_SUCCESS);
}

int dup_cmd(t_executor *executor, t_data *data, int *end, int fd_in)
{
	if (executor->prev && dup2(fd_in, STDIN_FILENO) < 0)
		return (name_error(NULL, NULL, "dup error"));
	close(end[0]);
	if (executor->next && dup2(end[1], STDOUT_FILENO) < 0)
		return (name_error(NULL, NULL, "dup error"));
	close(end[1]);
	if (executor->prev)
		close(fd_in);
	if (cmd_file(data, data->envp->path) == NULL)
		exit(0);
	return (0);
}

void	ft_simple_cmdsadd_back(t_executor **lst, t_executor *new)
{
	t_executor	*tmp;

	tmp = *lst;
	if (*lst == NULL)
	{
		*lst = new;
		return ;
	}
	while (tmp->next != NULL)
		tmp = tmp->next;
	tmp->next = new;
	new->prev = tmp;
}
// void execution(t_data *data)
// {
//     static int i;

//     i = 0;
//     // ft_putnbr_fd(data->executor->pipes, 1);
//     if (data->executor->pipes > 0)
//     {
//         data->executor->pid = malloc(sizeof(int) * data->executor->pipes);
//         if (!data->executor->pid) {
//             // Handle allocation failure here
//             return;
//         }
//         while (i < data->executor->pipes)
//         {
//             data->executor->pid[i] = fork();
//             if (data->executor->pid[i] == -1) {
//                 ft_error(3, NULL, 0);
//                 // Handle fork failure here
//             }

//             if (data->executor->pid[i] == 0) {
//                 // Child process
//                 free(data->executor);
//                 cmd_file(data, data->envp->path);
//                 ft_free_all(data);
//                 exit(data->status_code);
//             }
//             i++;
//         }
//     }
//     else 
//     {
//         int pid;
//         pid = fork();
//         if (pid == 0) {
//             // Child process
//             free(data->executor);
//             cmd_file(data, data->envp->path);
//             ft_free_all(data);
//             exit(data->status_code);
//         }

//         // Parent process
//     }
//     waitpid(-1, &data->status_code, 0);
//     // exit(0);
// }

// void wait_pid(int *pid, int amount, t_data *data)
// {
//     int i;

//     i = 0;

//     while (i < amount)
//     {
//         waitpid(pid[i], &data->status_code, 0);
//         i++;
//     }
//     waitpid(pid[i], &data->status_code, 0);
//     exit(0);
// }

int	check_builtin(char **str)
{
	int			i;
	static char	*builtins[] = {
		"echo",
		"cd",
		"pwd",
		"export",
		"unset",
		"env",
		"exit",
		"ENV",
		"PWD",
		"ECHO",
	};

	i = 0;
	while (i < 10)
	{
		if (ft_strcmp(builtins[i], str[0]) == 0)
			return (i);
		i++;
	}
	return (-1);
}
