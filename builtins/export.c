/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ayal-ras <ayal-ras@student.42abudhabi.a    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/11 18:01:34 by ayal-ras          #+#    #+#             */
/*   Updated: 2024/02/21 14:26:23 by ayal-ras         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"


void	sorted_env(char **env, size_t env_len)
{
	int		ordered;
	size_t		i;
	char	*tmp;

	ordered = 0;
	while (env && ordered == 0)
	{
		ordered = 1;
		i = 0;
		while (i < env_len - 1)
		{
			if (ft_strcmp(env[i], env[i + 1]) > 0)
			{
				tmp = env[i];
				env[i] = env[i + 1];
				env[i + 1] = tmp;
				ordered = 0;
			}
			i++;
		}
		env_len--;
	}
}


int	ft_export(char *str, t_data *data)
{
	t_env	*env_duplicate;
	char	**token;

	env_duplicate = duplicate_env(data->envp);
	token = ft_split(str, ' ');
	if (ft_strlen(token[0]) != 6)
		return (1);
	else if (token[1] == NULL)
	{
		declare_sorted(env_duplicate);
		return (0);
	}
	else
	{
		if (!validate_input(token, env_duplicate, "export"))
			return (0);
		ft_putendl_fd("\033[31mnot working or more arguments", 2);
		return (0);
	}
	// Handle exporting specific environment variables (not implemented yet)
	return (1);
}

void	declare_sorted(t_env *head)
{
	char	**temp;
	char	*str;
	int		i;
	size_t	env_len;

	str = env_str(head);
	temp = ft_split(str, '\n');
	free(str);
	env_len = size_of_env(temp);
	sorted_env(temp, env_len);
	i = 0;
	while (temp[i])
	{
		ft_putstr_fd("declare -x ", 1);
		ft_putendl_fd(temp[i], 1);
		i++;
	}
	free_array(temp);
}

char	*env_str(t_env *env)
{
	char	*str;
	int		i;
	int		j;

	i = 0;
	j = 0;
	str = malloc(sizeof(char) * len_of_values(env) + 1);
	if (!str)
		return (NULL);
	while (env && env->next != NULL)
	{
		if (env->value != NULL)
		{
			j = 0;
			while (env->value[j])
				str[i++] = env->value[j++];
		}
		if (env->next->next != NULL)
			str[i++] = '\n';
		env = env->next;
	}
	str[i] = '\0';
	return (str);
}
