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
	if (signum == SIGQUIT)
	{
		g_signal = 131;
		ft_putendl_fd("Quit: 3", 2);
	}
	else if (signum == SIGINT)
	{
		g_signal = 130;
		ft_putendl_fd(" ", 2);

	}
}

void	sigint_handler(int signum)
{	
	if (signum == SIGQUIT)
	{
		rl_redisplay();
		signal(SIGQUIT, SIG_IGN);
		ft_putstr_fd("\b\b\b\b\b\b\b\b\b", 2);
		g_signal = 0;
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
		free(data->pwd);
		free(data->old_pwd);
		free_array(data->envp->path);
		free_env_list(data->envp);
		ft_putstr_fd("exit\n", 2);
		return (1);
	}
	return (0);
}

void	check_signal(char *input, t_data *data)
{
	if (handle_eof(input, data))
		exit(0);
	// if (g_sig_interrupt == 1)
	// 	g_signal = 1;
	// else
	// 	g_signal = 0;
	// if (ft_str != NULL)
	// 	g_signal = 1;
}

void	ft_signals(void)
{
	signal(SIGINT, sigint_handler);
	// signal(SIGQUIT, SIG_IGN);
	signal(SIGQUIT, sigint_handler);
}
