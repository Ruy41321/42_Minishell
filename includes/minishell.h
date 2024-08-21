#ifndef MINISHELL_H
# define MINISHELL_H

# include "libft.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <readline/readline.h>
#include <readline/history.h>
#include <unistd.h>
#include <sys/wait.h>
#include <signal.h>

typedef struct s_env_var {
    char *name;
    char *value;
    struct s_env_var *next;
} t_env_var;


void    signal_init(void);
int     execute_command(t_env_var *head, char **command, char **envp);
char    *get_input(char **envp);
int     len(char **array);
char    *substitute_$(t_env_var *head, char *input);
void    set_env_var(t_env_var *head, char *name, char *value);
void    parse_and_exec(t_env_var *head, char *input, char **envp);
char	*get_env_var(char **envp, const char *var_name);
void	free_command(char **command);
char    *get_local_var(t_env_var *head, char *name, char *temp);
char    *get_exit_status(void);
char	**get_separeted_inputs(char *input);
void	*safe_malloc(size_t size);

#endif
