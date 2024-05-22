/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expansion_utils.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zfiros-a <zfiros-a@student.42abudhabi.a    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/22 17:28:09 by zfiros-a          #+#    #+#             */
/*   Updated: 2024/05/22 17:28:09 by zfiros-a         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

char	*handle_quote(char *key)
{
	size_t	len;
	size_t	i;
	size_t	j;
	char	*temp;

	i = 0;
	j = 0;
	len = ft_strlen(key);
	temp = malloc(sizeof(char) * (len + 1));
	if (!temp)
		return (NULL);
	while (key[i])
	{
		while (i < len - 1)
			temp[j++] = key[i++];
		if (key[i] != '\"')
		{
			if (key[i] == '\'' && key[i + 1] == '\0')
				return (NULL);
			temp[j++] = key[i];
		}
		i++;
	}
	temp[j] = '\0';
	return (temp);
}

char	*search_env_variable2(t_env *envp, char *key)
{
	char	*temp;
	char	*str;
	t_env	*current_envp;
	size_t	i;

	i = -1;
	temp = handle_quote(key);
	if (!temp)
		return (NULL);
	str = NULL;
	while (ft_strlen(key) > ++i)
	{
		if (temp[i] == '$' && (temp[i + 1] == ' ' || temp[i + 1] == '\0'))
		{
			str = ft_strjoin_free(str, "$");
			i++;
		}
		if (temp[i] == '$' && temp[i + 1] != ' ' && temp[i + 1] != '\0')
			i++;
		current_envp = envp;
		envp_loop(current_envp, &str, temp, &i);
		if (temp[i] && temp[i] != '"' && temp[i] != '$')
			str = ft_strjoin_free(str, (char []){temp[i], '\0'});
	}
	return (free(temp), str);
}

void	print_after_equal2(char *temp)
{
	char	*equal_pos;
	char	*str;
	char	*new;
	int		i;
	int		j;

	i = 0;
	j = 0;
	while (temp[i] != '$')
		i++;
	str = malloc(i + 1);
	i = 0;
	if (temp[i] == '\"')
		i++;
	while (temp[i] && temp[i] != '$')
		str[j++] = temp[i++];
	str[j] = '\0';
	equal_pos = ft_strchr(temp, '=');
	if (equal_pos != NULL)
		new = ft_strjoin(str, equal_pos + 1);
	else
		new = ft_strdup(str);
	ft_putstr_fd(new, 2);
	free(new);
	free(str);
}

int	name_error2(char *name, char *str, char *message, int flag)
{
	if (!flag)
	{
		ft_putstr_fd("minishell: ", STDERR_FILENO);
		print_after_equal2(name);
		ft_putstr_fd(": ", STDERR_FILENO);
		ft_putstr_fd(str, STDERR_FILENO);
		ft_putendl_fd(message, STDERR_FILENO);
	}
	if (flag == 1)
	{
		print_after_equal2(name);
	}
	if (flag == 2)
	{
		ft_putstr_fd("minishell: ", STDERR_FILENO);
		print_after_equal2(name);
		ft_putstr_fd(": ", STDERR_FILENO);
		ft_putstr_fd(str, STDERR_FILENO);
		ft_putendl_fd("No such file or Directory", STDERR_FILENO);
	}
	return (0);
}

int	ft_specified_error(char *str, int flag)
{
	if (!flag)
	{
		if (str[0] == '$' && str[1] == '\0')
			return (0);
		if (!ft_strncmp(str, "$PATH", 5))
			name_error2(str, NULL, "No such file or directory", 0);
		else if (!ft_strncmp(str, "$PWD", 4)
			|| !ft_strncmp(str, "$OLDPWD", 7) || !ft_strncmp(str, "$HOME", 5))
			name_error2(str, NULL, "is a directory", 0);
		else
			name_error2(str, NULL, "command not found", 0);
	}
	else
		name_error2(str, NULL, "command not found", flag);
	return (0);
}
