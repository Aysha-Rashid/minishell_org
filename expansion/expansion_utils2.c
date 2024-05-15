
#include "../minishell.h"

char	*ft_strjoin_free(char *str1, const char *str2)
{
	size_t	len1;
	size_t	len2;
	char	*result;

	if (str1 == NULL)
		len1 = 1;
	else
		len1 = ft_strlen(str1);
	len2 = ft_strlen(str2);
	result = malloc(len1 + len2 + 1);
	if (str1 == NULL)
		str1 = ft_strdup("");
	if (result == NULL)
		return (NULL);
	if (str1 != NULL)
	{
		ft_strcpy(result, str1);
		free(str1);
	}
	else
		result[0] = '\0';
	ft_strcat(result, str2);
	return (result);
}

void	envp_loop(t_env *envp, char **str, char *temp, size_t *i)
{
	while (envp)
	{
		if (ft_strncmp(envp->key, temp + *i, ft_strlen(envp->key)) == 0)
		{
			(*i) += ft_strlen(envp->key);
			if (!ft_isalpha(temp[*i]) && !ft_isalnum(temp[*i]))
			{
				if (*str == NULL)
					*str = ft_strdup(envp->value);
				else
					*str = ft_strjoin_free(*str,
							envp->value + ft_strlen(envp->key) + 1);
				return ;
			}
			while (temp[*i] && temp[*i] != '$')
				(*i)++;
			return ;
		}
		envp = envp->next;
	}
}
