/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ayal-ras <ayal-ras@student.42abudhabi.a    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/28 12:45:58 by ayal-ras          #+#    #+#             */
/*   Updated: 2024/02/22 19:50:11 by ayal-ras         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h>
#include <fcntl.h>
#include <stdio.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <signal.h>
#include <errno.h>
#include <string.h>
#include <readline/readline.h>
#include <readline/history.h>
#include "libft/libft.h"
// #include "pipex/pipex.h"

// #define	WRONG_MESSAGE "\033[31mWrong input"
#define PIPE 6
#define INFILE 5
#define STDIN 0
#define STDOUT 1

typedef struct s_commands
{
	char	**path;

}				t_commands;

typedef struct s_env
{
	char			*value;
	char			*key;
	struct s_env	*next;
}				t_env;

typedef struct s_data
{
	// int				exit_status;
	char			*cmd;
	char			*pwd;
	char			*old_pwd;
	t_env			*envp;
	t_commands		*commands;
	struct s_data	*next;
	int				status_code;
}				t_data;

// typedef struct s_pipex
// {
// 	char	**cmd1;
// 	char	**cmd2;
// 	char	*cmd_path1;
// 	char	*cmd_path2;
// 	int		infile;
// 	int		outfile;
// }				t_pipex;

int		parse_env(t_data *data, char **env);
void	prompt_loop(char *str, t_data *data);
t_env	*allocate_env(char **env);
int		free_array(char **str);
int		find_pwd(t_data *data);
int		ft_env(t_data *data);
int		ft_pwd(t_data *data);
int		ft_export(char *str, t_data *data);
int		declare_sorted(t_env *head, int flag);
int		ft_echo(char *argv);
int		ft_cd(char *str, t_data *data);
char	**find_paths_and_split(char **envp);
int		find_current_path(t_data *data, char *str);
void	add_pwd_to_env(t_data *data, char *str);
int		ft_unset(char *str, t_data *data);
t_env	*search_env_variable(t_env *envp, char *key);
char	*ft_strndup(const char *src, size_t n);
int		check_unset_arg(char *str, char *token, t_data *data, int size_of_env);

void	change_pwd(t_data *tools);
t_env	*duplicate_node(char *str);
t_env	*duplicate_env(t_env *env);
int		free_env_list(t_env *head);
size_t	len_of_values(t_env *lst);
char	*env_str(t_env *env);
int		valid_command(char *str, t_data *data);
size_t	size_of_env(char **head);
void	name_error(char *name);
int		validate_input(char **token, t_env *current, char *name);
int		already_there(char *variable, t_env	*env);
// int		invalid_identifier(char **str_arr);

// int		count_arg(char **str);
// int		*builtin_arr(char *str);
// void	init_signal(void);
// void	sigint_handler(int sig);

//signals
void	sigint_handler(int signum);
int		handle_eof(char *str);
void	ft_signals(void);
void	check_signal(char *input, t_data *data);

extern int g_sig_interrupt;