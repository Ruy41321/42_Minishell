/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lpennisi <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/17 11:16:45 by lpennisi          #+#    #+#             */
/*   Updated: 2024/09/28 14:57:14 by lpennisi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	signal_handler_child(int sig)
{
	if (sig == SIGINT)
	{
		ft_printf("\n");
		exit(1);
	}
}

void	signal_handler1(int sig)
{
	if (sig == SIGINT)
	{
		ft_printf("\n");
		rl_on_new_line();
		rl_replace_line("", 0);
		rl_redisplay();
		g_exit_status = 130;
	}
}

void	sigint_handler_heredoc(int sig)
{
	if (sig == SIGINT)
	{
		g_exit_status = 300;
	}
}

void	signal_init(void)
{
	signal(SIGINT, signal_handler1);
	signal(SIGQUIT, SIG_IGN);
}
