/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   herdoc_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lpennisi <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/28 15:20:57 by lpennisi          #+#    #+#             */
/*   Updated: 2024/09/28 15:29:35 by lpennisi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	get_heredoc_fd(int count)
{
	char	*filename;
	int		fd;
	char	*str;

	filename = ft_strjoin_free("/tmp/heredoc", ft_itoa(count), 2);
	fd = open(filename, O_WRONLY | O_CREAT | O_TRUNC, 0644);
	if (fd < 0)
	{
		str = ft_strjoin_free("minishell: ", filename, 2);
		perror(str);
		return (free(str), 1);
	}
	free(filename);
	return (fd);
}

void	print_heredoc_warning(char *terminator)
{
	ft_putstr_fd("minishell: warning: ", STDERR_FILENO);
	ft_putstr_fd(HEREDOC_WARN, STDERR_FILENO);
	ft_putstr_fd(terminator, STDERR_FILENO);
	ft_putstr_fd("')\n", STDERR_FILENO);
}

char	*get_unvalid_terminator(char *terminator)
{
	if (terminator[0] == '<')
	{
		if (terminator[1] == '<')
			return (ft_strdup("<<'\n"));
		else
			return (ft_strdup("<'\n"));
	}
	if (terminator[0] == '>')
	{
		if (terminator[1] == '>')
			return (ft_strdup(">>'\n"));
		else
			return (ft_strdup(">'\n"));
	}
	if (terminator[0] == '|')
		return (ft_strdup("|'\n"));
	return (NULL);
}
