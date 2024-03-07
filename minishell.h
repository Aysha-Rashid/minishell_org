/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ayal-ras <ayal-ras@student.42abudhabi.a    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/28 12:45:58 by ayal-ras          #+#    #+#             */
/*   Updated: 2024/03/07 18:08:31 by ayal-ras         ###   ########.fr       */
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

typedef enum s_tokens
{
	PIPE = 1,
	OUTFILE,
	OUTEOF,
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
	int		*pid; // use g_sig to access this member.
	int		pipes;
	int		heredoc;
	int		in;
	int		out;
	// char	**cmd;
	// char	*cmd_path;
}				t_executor;

typedef struct s_data
{
	int				no_path;
	int				status_code;
	char			*cmd;
	char			*pwd;
	char			*old_pwd;
	t_env			*envp;
	// t_dupenv		*dup_env;
	t_executor		*executor;
	t_lexer			*lexer_list;
}				t_data;

void		ft_putstrn_fd(const char *str, size_t n, int fd);
void		print_after_equal(char *temp);
int			ft_env(t_data *data, char *str);
int			parse_env(t_data *data, char **env);
t_env		*allocate_env(char **env);
char		*find_paths_and_split(char **env);
char		*cmd_file(char	*cmd, char **paths);
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
int			unset_loop(t_data *data, t_env *current, int len, char **token);
t_env		*search_env_variable(t_env *envp, char *key);

int			check_unset_arg(char *str, char *token, t_data *data,
				int size_of_env);

int			free_array(char **str);
int			free_env_list(t_env *head);
void		free_lexer_list(t_lexer *list);
int			name_error(char *name, char *str, char *message);
void		free_data(t_data *data);
int			validate_input(char **token, t_env *current, char *name);

size_t		len_of_values(t_env *lst);
size_t		size_of_env(char **head);
char		*ft_strcpy(char	*dest, char *src);

void		count_pipes(t_lexer *lexer, int pipes);

// int		count_arg(char **str);
// int		*builtin_arr(char *str);
// void	init_signal(void);
// void	sigint_handler(int sig);

//signals
void		sigint_handler(int signum);
int			handle_eof(char *str, t_data *data);
void		ft_signals(void);
void		check_signal(char *input, t_data *data);

//echo
int			ft_echo(char *argv);
void		check_and_write(char *str);

//parse
int			quote(char *l);
int			ft_error(int i, char *str, t_data *data);

//lexer
int			token_reader(t_data *data);
int			read_words(int i, char *str, t_lexer **lexer_list);
int			add_node(char *str, t_tokens token, t_lexer **lexer_list);
int			skip_spaces(char *str, int i);
int			is_whitespace(char c);
t_lexer		*ft_lexernew(char *str, int token);
void		ft_lexeradd_back(t_lexer **list, t_lexer *new);
int			handle_quotes(int i, char *str, char del);
t_tokens	check_token(int n);
int			handle_token(char *str, int i, t_lexer **lexer_list);
char		*return_woqoutes(char *str, char del);
char 		*remove_all_qoutes(char *str);

//execution
void		check_n_execute(char *str, t_data *data);
int			check_pipes_n_execute(t_data *data);
int			execution(char *str, t_data	*data, t_executor *executor);
int			buitin_command(char *str, t_data *data);
void		prompt_loop(char *str, t_data *data);

int			parsing_lexar(t_data *data, t_lexer *lexar);
int			double_token_error(t_lexer *lexar);
void		ft_lexerclear(t_lexer **lst);
extern		int g_sig_interrupt;

// t_env	*duplicate_env(t_env *env)
// {
// 	t_env	*head;
// 	t_env	*temp;

// 	temp = NULL;
// 	head = NULL;
// 	while (env != NULL)
// 	{
// 		if (head == NULL)
// 		{
// 			head = malloc(sizeof(t_env));
// 			temp = head;
// 		}
// 		else
// 		{
// 			temp->next = malloc(sizeof(t_env));
// 			temp = temp->next;
// 		}
// 		if (temp == NULL)
// 			return (NULL);
// 		temp->value = ft_strdup(env->value);
// 		temp->next = NULL;
// 		env = env->next;
// 	}
// 	return (head);
// }