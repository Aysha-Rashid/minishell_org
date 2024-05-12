/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expansion_utils.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zfiros-a <zfiros-a@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/07 18:24:41 by zfiros-a          #+#    #+#             */
/*   Updated: 2024/03/15 15:12:03 by zfiros-a         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	joining_str(char *str, char *word)
{
	char *temp_str;

	temp_str = str;
	str = ft_strjoin(temp_str, word);
}

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
				return ( NULL);
			temp[j++] = key[i];
		}
		i++;
	}
	temp[j] = '\0';
	return (temp);
}

char *search_env_variable2(t_env *envp, char *key)
{
	char	*temp;
	char	*str;
	char	*new_value;
	char	*temp_str;
	t_env	*current_envp;
	size_t	i;
	size_t	j;

	i = 0;
	temp = handle_quote(key);
	if (!temp)
		return (NULL);
	str = NULL;
	while (ft_strlen(key) > i)
	{
		if (temp[i] == '$' && (temp[i + 1] == ' ' || temp[i + 1] == '\0'))
		{
			temp_str = str;
			str = ft_strjoin(str, "$");
			free(temp_str);
			i++;
		}
		else if (temp[i] == '$' && temp[i + 1] != ' ' &&  temp[i + 1] != '\0')
			i++;
		current_envp = envp;
		while (current_envp)
		{
			if (ft_strncmp(current_envp->key, temp + i, ft_strlen(current_envp->key)) == 0)
			{
				i += ft_strlen(current_envp->key);
				if (!ft_isalpha(temp[i]) && !ft_isalnum(temp[i]))
				{
					if (str == NULL)
						str = ft_strdup(current_envp->value);
					else
					{
						new_value = ft_strdup(current_envp->value);
						j = ft_strlen(current_envp->key) + 1;
						temp_str = str;
						str = ft_strjoin(temp_str, new_value + j);
						free(new_value);
						free(temp_str);
					}
					break;
				}
				while (temp[i] && temp[i] != '$')
					i++;
				break;
			}
			current_envp = current_envp->next;
		}
		if (temp[i] && temp[i] != '"' && temp[i] != '$')
		{
			if (!str)
				str = ft_strdup("");
			temp_str = str;
			str = ft_strjoin(temp_str, (char[]){temp[i], '\0'});
            free(temp_str);
		}
		i++;
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
		// else if (str[0] == '$' && str[1])
		// 	name_error(str, NULL, "command not found", 0);
		else
			name_error2(str, NULL, "command not found", 0);
	}
	else
		name_error2(str, NULL, "command not found", flag);
	return (0);
}
