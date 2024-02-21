/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zfiros-a <zfiros-a@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/21 15:12:37 by zfiros-a          #+#    #+#             */
/*   Updated: 2024/02/21 15:14:35 by zfiros-a         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	sigint_handler(int signum)
{
	if (signum == SIGINT)
	{
		ft_putstr_fd("\n", 2);
		rl_on_new_line();
		// rl_replace_line("", 0);
		rl_redisplay();
	}
}

int	handle_eof(char *str)
{
	if (str == NULL)
	{
		ft_putstr_fd("exit\n", 2);
		return (1);
	}
	return (0);
}

void	check_signal(char *input, t_data *data)
{
	if (handle_eof(input))
		exit(0);
	if (g_sig_interrupt == 1)
		data->status_code = 1;
}

void	ft_signals(void)
{
	signal(SIGINT, sigint_handler);
	signal(SIGQUIT, SIG_IGN);
}
