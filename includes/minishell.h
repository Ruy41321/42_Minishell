/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: flo-dolc <flo-dolc@student.42roma.it>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/28 16:46:40 by lpennisi          #+#    #+#             */
/*   Updated: 2024/09/04 15:16:40 by flo-dolc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include "libft.h"
# include <stdlib.h>
# include <stdio.h>
# include <string.h>
# include <readline/readline.h>
# include <readline/history.h>
# include <unistd.h>
# include <sys/wait.h>
# include <signal.h>
# include <fcntl.h>
# include <linux/limits.h>

typedef struct s_env_var
{
	char				*name;
	char				*value;
	struct s_env_var	*next;
}	t_env_var;

void	signal_init(void);
int		execute_command(t_env_var *head, char **command, char **envp);
char	*get_input(t_env_var *env);
char	*substitute_dollars(t_env_var *head, char *input);
void	set_env_var(t_env_var *head, char *name, char *value);
void	parse_and_exec(t_env_var *head, char *input, char **envp);
char	*get_env_var(t_env_var *env, const char *var_name);
char	*get_full_path(char *command, char **envp);
void	free_command(char **command, int len);
void	free_command_3d(char ***command);
char	*get_local_var(t_env_var *head, char *name, char *temp);
char	*get_exit_status(void);
char	**get_separeted_inputs(char *input);
void	*safe_malloc(size_t size);
char	*get_delim(char *input);
void	init_separeted(char ***separated, int *quotes, int *c, char *input);
char	*clean_input(char *input, char ch);
int		only_spaces(char *input);
char	**handle_redirection(char **command);
char	*redirect_syntax(char *input);
char	***get_piped_command(char **command);
int		is_prepipe_command(char **command);
char	*deep_clean_input(char *input);
char	*syntax_error(char *input);
int		redirect_input(char *file);
int		redirect_output(char *file, int flags);
void	handle_stdfd(int *fd);
void	handle_pipe(char **piped_command, int *p_fd, int *is_prep, int stdOut);
int		handle_env_var_assignment(t_env_var *head, char **piped_command);
char	*remove_quotes(char *input);
void	remove_quotes_2d(char **command);
void	quotes_check(char *input, int *quotes, int *dquotes, int *i);
void	signal_handler_child(int sig);

# define SYNTAX_ERROR "minishell:syntax error near unexpected token `"
#endif
