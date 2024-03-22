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

typedef enum s_tokens
{
	PIPE = 1,
	OUTFILE,
	APPEND,
	INFILE,
	HEREDOC,
}	t_tokens;

typedef struct s_env
{
	char			**path;
	char			*value;
	char			*key;
	struct s_env	*next;
}				t_env;

typedef struct s_lexer
{
	int				i;
	char			*str;
	t_tokens		token;
	struct s_lexer	*next;
	struct s_lexer	*prev;
}				t_lexer;

typedef struct s_executor
{
	// int					*pid;
	// int					pipes;
	// int					heredoc;
	// char				*here_name;
	int					in;
	int					out;
	int					append;
	char				*cmd;
	struct s_executor	*next;
	struct s_executor	*prev;
}				t_executor;

typedef struct s_data
{
	// int				*pid;
	int				no_path;
	int				status_code;
	char			*cmd;
	char			*pwd;
	char			*old_pwd;
	t_env			*envp;
	t_executor		*executor;
	t_lexer			*lexer_list;
}				t_data;

void		ft_putstrn_fd(const char *str, size_t n, int fd);
void		print_after_equal(char *temp);
int			ft_env(t_data *data, char *str);
int			parse_env(t_data *data, char **env);
t_env		*allocate_env(char **env);
char		*find_paths_and_split(char **env);
char		*cmd_file(char *cmd, char **paths);
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

void		count_pipes(t_data *data);

//signals
void		sigint_handler(int signum);
int			handle_eof(char *str, t_data *data);
void		ft_signals(void);
void		check_signal(char *input, t_data *data);

//echo
int			ft_echo(char *argv, t_data *data);
void		check_and_write(char *str, t_data *data);

//parse
int			quote(char *l);
int			ft_error(int i, char *str, int no_path);

//lexer
int			token_reader(t_data *data);
int			read_words(int i, char *str, t_lexer **lexer_list);
int			add_node(char *str, t_tokens token, t_lexer **lexer_list);
int			skip_spaces(char *str, int i);
int			is_whitespace(char c);
t_lexer		*ft_lexernew(char *str, int token);
int			ft_lexeradd_back(t_lexer **list, t_lexer *new);
int			handle_quotes(int i, char *str, char del);
t_tokens	check_token(int n);
int			handle_token(char *str, int i, t_lexer **lexer_list);
char		*return_woqoutes(char *str, char del);
char		*remove_all_qoutes(char *str);
//execution
void		check_n_execute(char *str, t_data *data);
int			check_pipes_n_execute(t_data *data);
int			execution(t_executor *executor, t_data *data);
int			builtin_command(char *str, t_data *data);
void		prompt_loop(char *str, t_data *data);
int			check_builtin(char *str);

t_executor	*init_executor(t_data *data, char *cmd);
t_executor	*parse_pipeline(char *cmd, t_data *data);
//lexer_parsing
int			parsing_lexar(t_data *data, t_lexer *lexar);
int			double_token_error(char *str);
void		ft_lexerclear(t_lexer **lst);
//expansion

int			ft_expansion(t_data *data);
int			ft_expansion3(t_data *data, char *str, int flag);
size_t		dollar_sign(char *str);
char		*search_env_variable2(t_env *envp, char *key);
void		print_after_equal2(char *temp);

void		free_executor(t_executor *executor);
void		close_and_free_all(t_data *data, int *end);
int			name_error2(char *name, char *str, char *message, int flag);
void		exit_and_free(t_data *data, int *end, int status);
int			free_array(char **str);
int			free_env_list(t_env *head);
void		free_lexer_list(t_lexer *list);
int			name_error(char *name, char *str, char *message, int flag);
void		ft_free_all(t_data *data);
void		check_command(char *str, char *cmd, int *end, t_data *data);
int			validate_input(t_data *data, char *token,
				t_env *current, char *name);
void		name_error3(char *exit_status, char *message, int flag);
int		invalid_identifier(t_data *data, char *str, char *name);

//redirection
int			is_type(t_lexer *lexer, char *str);
int			is_redir(char *cmd);
void		redir_and_execute(t_data *data, t_executor *executor);
int			check_redir_pipe(char *cmd);
void		redir(t_executor *executor);
char		*remove_redir_or_files(char *cmd);
int	    	ft_open(t_executor *executor, char *redir, char *file);
void		ft_dup_fd(t_data *data, t_executor *executor, int *end, int next);
void		dup_check(int file, int dupped);
char		*remove_quotes(char *str);
// int			check_line(t_executor *executor, t_data *data);

void		ft_sig2(int signum);

extern		int g_sig_interrupt;
