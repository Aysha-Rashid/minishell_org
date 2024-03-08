/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zfiros-a <zfiros-a@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/21 15:12:37 by zfiros-a          #+#    #+#             */
/*   Updated: 2024/03/08 14:37:32 by zfiros-a         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	sigint_handler(int signum)
{
	if (signum == SIGINT)
	{
		g_sig_interrupt = 1;
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
		free_env_list(data->envp);
		free(data->pwd);
		free(data->old_pwd);
		ft_putstr_fd("exit\n", 2);
		return (1);
	}
	return (0);
}

void	check_signal(char *input, t_data *data)
{
	if (handle_eof(input, data))
		exit(0);
	if (g_sig_interrupt == 1)
		data->status_code = 1;
}

void	ft_signals(void)
{
	signal(SIGINT, sigint_handler);
	signal(SIGQUIT, SIG_IGN);
}
