/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ayal-ras <ayal-ras@student.42abudhabi.a    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/28 12:45:58 by ayal-ras          #+#    #+#             */
/*   Updated: 2024/02/20 22:24:08 by ayal-ras         ###   ########.fr       */
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
	char	**cmd;
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
void	free_array(char **str);
int		find_pwd(t_data *data);
int		ft_env(t_data *data);
int		ft_pwd(t_data *data);
int		ft_export(char *str, t_data *data);
void	declare_sorted(t_env *head);
int		ft_echo(char *argv);
int		ft_cd(char *str, t_data *data);
char	**find_paths_and_split(char **envp);
int		find_current_path(t_data *data, char *str);
void	add_pwd_to_env(t_data *data, char *str);
int		ft_unset(char *str, t_data *data);
t_env	*search_env_variable(t_env *envp, char *key);

void	change_pwd(t_data *tools);
t_env	*duplicate_node(char *str);
t_env	*duplicate_env(t_env *env);
void	free_env_list(t_env *head);
size_t	len_of_values(t_env *lst);
char	*env_str(t_env *env);
int		valid_command(char *str, t_data *data);
size_t	size_of_env(char **head);
void	name_error(char *name);
int		validate_input(char **token, t_env *current, char *name);
// int		invalid_identifier(char **str_arr);

// int		count_arg(char **str);
// int		*builtin_arr(char *str);
// void	init_signal(void);
// void	sigint_handler(int sig);