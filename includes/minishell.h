/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lpennisi <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/28 16:46:40 by lpennisi          #+#    #+#             */
/*   Updated: 2024/09/13 13:06:44 by lpennisi         ###   ########.fr       */
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

typedef struct s_my_envp
{
	t_env_var	*locals;
	t_env_var	*exported;
}	t_my_envp;

extern long long	g_exit_status;

void	signal_init(void);
int		execute_handler(t_my_envp *my_envp, char **command);
char	**list_to_matrix(t_env_var *head);
char	*get_input(t_my_envp *my_envp);
char	*substitute_dollars(t_my_envp *my_envp, char *input);
int		set_env_var(t_env_var *head, char *name, char *value, int create);
int		export_builtin(t_my_envp *my_envp, char **command);
int		pwd_builtin(void);
void	env_builtin(t_env_var *env);
int		cd_builtin(t_my_envp *my_envp, char **command);
int		echo_builtin(char **command);
int		unset_builtin(t_my_envp *my_envp, char **command);
int		exit_builtin(t_my_envp *my_envp, char **command);
void	parse_and_exec(t_my_envp *my_envp, char *input);
char	*get_env_var(t_env_var *env, char *var_name);
char	*get_full_path(char *command, t_env_var *env);
void	free_command(char **command, int len);
void	free_command_3d(char ***command);
void	free_my_envp(t_my_envp *my_envp);
char	*get_local_var(t_my_envp *my_envp, char *name, char *temp);
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
void	handle_stdfd(int **fd);
int		handle_pipe(char **piped_command, int *p_fd, int *is_prep);
int		handle_env_var_assignment(t_my_envp *my_envp, char **piped_command);
char	*remove_quotes(char *input);
void	remove_quotes_2d(char **command);
void	quotes_check(char *input, int *quotes, int *dquotes, int *i);
void	signal_handler_child(int sig);
void	free_my_envp(t_my_envp *my_envp);
void	increase_shlvl(t_env_var *head);
char	*get_env_name(char *envp);
int		exe_bultin(t_my_envp *my_envp, char **command);
int		exe_pipe(t_my_envp *envp, char ***commands, int *old_pipe, pid_t *ch);
void	child_process(char **piped_command, t_my_envp *envp);

# define SYNTAX_ERROR "minishell: syntax error near unexpected token `"
#endif
