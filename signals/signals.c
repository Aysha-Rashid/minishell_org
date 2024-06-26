/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ayal-ras <ayal-ras@student.42abudhabi.a    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/21 15:12:37 by zfiros-a          #+#    #+#             */
/*   Updated: 2024/03/09 19:39:14 by ayal-ras         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	ft_sig2(int signum)
{
	if (signum == SIGINT)
	{
		if (g_signal == IN_HERE)
		{
			g_signal = IN_HERE;
			exit(1);
		}
		else
			g_signal = 130;
		ft_putendl_fd(" ", 2);
	}
}

void	sigint_handler(int signum)
{
	if (signum == SIGQUIT)
	{
		if (g_signal == IN_HERE)
			signal(SIGQUIT, SIG_IGN);
		else
		{
			rl_redisplay();
			signal(SIGQUIT, SIG_IGN);
			ft_putstr_fd("\b\b", 2);
			g_signal = 0;
		}
	}
	if (signum == SIGINT)
	{
		g_signal = 1;
		ft_putstr_fd("\n", 2);
		// rl_replace_line("", 0);
		rl_on_new_line();
		rl_redisplay();
	}
}

int	handle_eof(char *str, t_data *data)
{
	if (str == NULL)
	{
		if (data->pwd)
			free(data->pwd);
		if (data->old_pwd)
			free(data->old_pwd);
		if (data->path)
			free_array(data->path);
		if (data->envp)
		{
			free_env_list(data->envp);
		}
		ft_putstr_fd("exit\n", 2);
		return (1);
	}
	return (0);
}

void	check_signal(char *input, t_data *data)
{
	if (handle_eof(input, data))
		exit(0);
}

void	ft_signals(void)
{
	if (g_signal != IN_HERE)
	{
		signal(SIGINT, sigint_handler);
		signal(SIGQUIT, sigint_handler);
	}
}
