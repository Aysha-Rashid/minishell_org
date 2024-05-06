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

char	*handle_quote(char *temp, char *key)
{
	int	len;
	int	i;
	int	j;

	i = 0;
	j = 0;
	len = ft_strlen(key);
	temp = malloc(sizeof(char) * (len + 1));
	while (key[i])
	{
		while (i < len - 1)
			temp[j++] = key[i++];
		if (key[i] != '\"')
		{
			if (key[i] == '\'')
				return (free(temp), NULL);
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
	int		i;

	i = 0;
	temp = NULL;
	temp = handle_quote(temp, key);
	str = NULL;
	if (!temp)
		return (NULL);
	while (envp)
	{
		if (ft_strncmp(envp->key, temp, ft_strlen(envp->key)) == 0)
		{
			while (temp[i] && temp[i] == envp->key[i])
				i++;
			if (!ft_isalpha(temp[i]) && !(temp[i] >= '0' && temp[i] <= '9'))
				str = ft_strjoin(envp->value, temp + i);
		}
		envp = envp->next;
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
	else
		print_after_equal2(name);
	return (0);
}

int	ft_specified_error(char *str, int flag)
{
	if (!flag)
	{
		if (str[0] == '$' && str[1] == '\0')
			return (0);
		if (!ft_strncmp(str, "$PATH", 6))
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
