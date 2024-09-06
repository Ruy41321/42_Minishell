/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: flo-dolc <flo-dolc@student.42roma.it>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/17 11:16:45 by lpennisi          #+#    #+#             */
/*   Updated: 2024/09/06 03:06:50 by flo-dolc         ###   ########.fr       */
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

void	signal_init(void)
{
	signal(SIGINT, signal_handler1);
	signal(SIGQUIT, SIG_IGN);
}
