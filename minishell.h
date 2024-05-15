/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zfiros-a <zfiros-a@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/28 12:45:58 by ayal-ras          #+#    #+#             */
/*   Updated: 2024/03/15 14:55:07 by zfiros-a         ###   ########.fr       */
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

#define BUFFER_SIZE 42
#define IN_HERE 30
#define OUT_HERE 20

typedef struct s_env
{
	// char			**path;
	char			*value;
	char			*key;
	struct s_env	*next;
}				t_env;

typedef struct s_executor
{
	int					in;
	int					out;
	int					heredoc;
	char				*cmd;
	struct s_executor	*next;
	struct s_executor	*prev;
}				t_executor;

typedef struct s_data
{
	int				no_path;
	char			**path;
	char			*cmd;
	char			*pwd;
	char			*old_pwd;
	t_env			*envp;
	t_executor		*executor;
}				t_data;

void		ft_putstrn_fd(const char *str, size_t n, int fd);
void		print_after_equal(char *temp);
int			ft_env(t_data *data, char *str);
int			parse_env(t_data *data, char **env);
t_env		*allocate_env(char **env);
char		*find_paths_and_split(char **env);
char		*cmd_file(char *cmd, char **paths, t_data *data);
char		*given_path(char *cmd);

int			find_pwd(t_data *data);
int			ft_pwd(t_data *data);

int			ft_export(char *str, t_data *data);
char		*env_str(t_env *env);
int			env_add(char *variable, t_data *env);
int			already_there(char *variable, t_data *data);

int			ft_cd(char *str, t_data *data);
void		change_pwd(t_data *tools);

int			ft_unset(char *str, t_data *data);
int			unset_loop(t_data *data, t_env *current, char **token);
t_env		*search_env_variable(t_env *envp, char *key);

int			check_unset_arg(char *token, t_data *data);
size_t		len_of_values(t_env *lst);
size_t		size_of_env(char **head);
char		*ft_strcpy(char	*dest, char *src);

//signals
void		sigint_handler(int signum);
int			handle_eof(char *str, t_data *data);
char		*handle_quote(char *key);
void		ft_signals(void);
void		check_signal(char *input, t_data *data);

//echo
int			ft_echo(char *argv, t_data *data);
int			check_and_write(char *str, t_data *data);

//parse
int			only_tabs_and_space(char *str, int quote);
void		failed_execve(char *cmd_file, char **str);
int			ft_error(int i, char *str, int no_path);
void		free_executor(t_executor *executor);
void		close_and_free_all(t_data *data);
int			name_error2(char *name, char *str, char *message, int flag);
void		exit_and_free(t_data *data, int status, char *str);
void		exit_properly(t_data *data, char **cmd, int exit_status);
void		exit_conditions(char **cmd, char *str, t_data *data);
int			free_array(char **str);
int			free_env_list(t_env *head);
int			name_error(char *name, char *str, char *message, int flag);
void		ft_free_all(t_data *data);
int			ft_specified_error(char *str, int flag);
void		check_command(char *str, char *cmd,
				t_executor *executor, t_data *data);
int			validate_input(t_data *data, char *token,
				t_env *current, char *name);
void		name_error3(char *exit_status, char *message, int flag);
int			invalid_identifier(t_data *data, char *str, char *name);

//execution
void		check_n_execute(char *str, t_data *data);
int			check_pipes_n_execute(t_data *data);
int			execution(t_executor *executor, t_data *data);
int			builtin_command(char *str, t_data *data, char *cmd, int in_exec);
void		prompt_loop(char *str, t_data *data);
int			check_builtin(char *str);
void		execute_command(char *cmd, t_data *data, t_executor *executor);
void		execute_binary_files(char **paths, char **str, char *cmd_file);
t_executor	*init_executor(t_data *data, char *cmd);
t_executor	*parse_pipeline(char *cmd, t_data *data);

//expansion

int			ft_expansion(t_data *data);
int			ft_expansion3(t_data *data, char *str, int flag);
size_t		dollar_sign(char *str);
char		*search_env_variable2(t_env *envp, char *key);
void		print_after_equal2(char *temp);

//redirection
int			check_redir_pipe(char *cmd);
int			redir(t_executor *executor);
char		check_quote(char s, char quote);
char		*remove_redir_or_files(char *cmd);
int			quote_redirection_parse(char *cmd, int i);
void		exec_quote_redir(t_executor	*executor, t_data	*data);
// int			open_and_check(int struct_file, char *file_name, char *redir);
int			open_and_check(int struct_file, char *file_name, int flag);
int			ft_open(t_executor *executor, char *redir, char *file);
void		dup_check(int file, int dupped);
char		*remove_quotes(char *str);
int			invalid_unset_loop(char *token, char *name, t_data *data);
int			invalid_export_loop(char *token, char *name, t_data *data);
void		not_valid_message(char *token, char *name, t_data *data);
int			parse_com(char *cmd);
int			quote(char *l);
void		ft_sig2(int signum);
int			open_files(char *cmd, char *redir, int i, t_executor *executor);
char		*get_redir_and_files(char *cmd);
void		init_pipe_n_signal(int *prev_pipe);
//get_next_line
char		*remaining(char *buffer);
char		*new_line(char *str);
char		*get_next_line(int fd, char *delimiter);
char		*read_lines(int fd, char *store);
char		*ft_get_strdup(char	*s1);
char		*ft_get_strjoin(char *s1, char *s2);
char		*ft_str_init(void);
char		*ft_get_strchr(const char *s, char c);

int			heredoc(t_executor *executor, t_data *data);
void		sig_handlers(int signum);
int			parse_com(char *cmd);
void		modified_based_quote(char **str);

void		close_exec_files(int fdin, int fdout);
void		check_exit(t_data *data, char *cmd);
void		new_variable(t_env *new, t_env *temp, char *key, char *value);
char		*ft_strjoin_free(char *str1, const char *str2);
void		envp_loop(t_env *envp, char **str, char *temp, size_t *i);
extern int	g_signal;
