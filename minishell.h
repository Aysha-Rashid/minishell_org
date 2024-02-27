/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zfiros-a <zfiros-a@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/28 12:45:58 by ayal-ras          #+#    #+#             */
/*   Updated: 2024/02/27 10:26:17 by zfiros-a         ###   ########.fr       */
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
// #define PIPE 6
// #define INFILE 5
#define STDIN 0
#define STDOUT 1

typedef enum s_tokens
{
	PIPE = 1,
	OUTFILE,
	OUTEOF,
	INFILE,
	HEREDOC,
}	t_tokens;

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

typedef struct s_lexer
{
	char			*str;
	t_tokens		token;
	int				i;
	struct s_lexer	*next;
	struct s_lexer	*prev;
}				t_lexer;

typedef struct s_data
{
	char			*cmd;
	char			*pwd;
	char			*old_pwd;
	int				no_path;
	t_env			*envp;
	t_commands		*commands;
	struct s_data	*next;
	int				status_code;
	t_lexer			*lexer_list;
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
// void	prompt_loop(char *str, t_data *data);
t_env		*allocate_env(char **env);
int			free_array(char **str);
int			find_pwd(t_data *data);
int			ft_env(t_data *data);
int			ft_pwd(t_data *data);
int			ft_export(char *str, t_data *data);
int			ft_echo(char *argv);
int			ft_cd(char *str, t_data *data);
char		**find_paths_and_split(char **envp);
// int		find_current_path(t_data *data, char *str);
// void	add_pwd_to_env(t_data *data, char *str);
int			ft_unset(char *str, t_data *data);
t_env		*search_env_variable(t_env *envp, char *key);
char		*ft_strndup(const char *src, size_t n);
int			check_unset_arg(char *str, char *token, t_data *data, int size_of_env);

void	change_pwd(t_data *tools);
// t_env	*duplicate_node(char *str);
t_env		*duplicate_env(t_env *env);
int			free_env_list(t_env *head);
size_t		len_of_values(t_env *lst);
char		*env_str(t_env *env);
// int		valid_command(char *str, t_data *data);
size_t		size_of_env(char **head);
int			name_error(char *name, char *str, char *message);
int			validate_input(char **token, t_env *current, char *name);
void		free_data(t_data *data);
int			env_add(char *variable, t_data *env);
// int		already_there(char *variable, t_data	*data);
// int		invalid_identifier(char **str_arr);

// int		count_arg(char **str);
// int		*builtin_arr(char *str);
// void	init_signal(void);
// void	sigint_handler(int sig);

//signals
void		sigint_handler(int signum);
int			handle_eof(char *str);
void		ft_signals(void);
void		check_signal(char *input, t_data *data);

//echo
int			ft_echo(char *argv);
void		check_and_write(char *str);

//parse
int			quote(char *l);
int 		ft_error(int i);

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

extern int g_sig_interrupt;

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