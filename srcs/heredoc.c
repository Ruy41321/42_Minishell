/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lpennisi <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/20 11:47:49 by lpennisi          #+#    #+#             */
/*   Updated: 2024/09/28 15:28:52 by lpennisi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	redirect_util(int fd, char *terminator)
{
	char	*line;

	while (1)
	{
		signal(SIGINT, sigint_handler_heredoc);
		line = readline("> ");
		signal(SIGINT, signal_handler1);
		if (!line)
		{
			print_heredoc_warning(terminator);
			break ;
		}
		if (g_exit_status == 300)
			return (free(line), set_exit_status(130), 1);
		if (ft_strncmp(line, terminator, ft_strlen(line)) == 0)
		{
			free(line);
			break ;
		}
		ft_putstr_fd(line, fd);
		ft_putstr_fd("\n", fd);
		free(line);
	}
	return (0);
}

int	redirect_input_heredoc(char *terminator, int count)
{
	int		fd;
	char	*token;
	int		ret;

	token = get_unvalid_terminator(terminator);
	if (token)
		return (free(token), 1);
	fd = get_heredoc_fd(count);
	ret = redirect_util(fd, terminator);
	return (close(fd), ret);
}

char	**substitute_heredoc(char **command)
{
	char	**new_command;
	char	*filename;
	int		i;
	int		count;
	int		len;

	len = ft_arrlen(command);
	count = 0;
	i = -1;
	new_command = (char **)safe_malloc(sizeof(char *) * (len + 1));
	new_command[len] = NULL;
	while (command[++i] != NULL)
	{
		if (ft_strcmp(command[i], "<<") == 0)
		{
			filename = ft_strjoin_free("/tmp/heredoc", ft_itoa(count++), 2);
			new_command[i] = ft_strdup("<");
			new_command[++i] = filename;
		}
		else
			new_command[i] = ft_strdup(command[i]);
	}
	return (free_command(command, len), new_command);
}

void	check_heredoc_err(char **command)
{
	int		i;
	char	*token;

	i = -1;
	while (command[++i] != NULL)
	{
		if (ft_strcmp(command[i], "<<") == 0)
		{
			token = get_unvalid_terminator(command[++i]);
			if (token)
			{
				ft_putstr_fd("minishell: syntax error near unexpected token `", \
				STDERR_FILENO);
				ft_putstr_fd(token, STDERR_FILENO);
				free(token);
				set_exit_status(2);
				break ;
			}
		}
	}
}

char	**handle_heredoc(char **command)
{
	char	**new_command;
	int		i;
	int		count;
	int		flag;

	i = -1;
	count = 0;
	flag = 0;
	check_heredoc_err(command);
	while (command[++i] != NULL)
	{
		if (ft_strcmp(command[i], "<<") == 0)
			if (redirect_input_heredoc(command[++i], count++))
				flag = 1;
	}
	if (flag)
		return (free_command(command, -1), NULL);
	new_command = substitute_heredoc(command);
	return (new_command);
}
