NAME = minishell
CC = cc
LIBFT = ./libft/libft.a
FLAGS = -Wall -Wextra  -Iincludes -I./libft -g -fsanitize=address
SRCS = main.c srcs/signals.c srcs/command.c srcs/executing.c srcs/utils/utils.c srcs/lexing_handling.c \
		srcs/utils/command_utils.c srcs/utils/command_utils2.c srcs/redirection.c srcs/pipe.c srcs/utils/path_utils.c \
		srcs/utils/command_utils3.c srcs/utils/redirection_utils.c srcs/utils/executing_utils.c srcs/utils/envp_utils.c \
		srcs/builtins/cd.c srcs/builtins/echo.c srcs/builtins/env.c srcs/builtins/exit.c srcs/builtins/export.c \
		srcs/builtins/pwd.c srcs/builtins/unset.c srcs/utils/pipe_utils.c srcs/heredoc.c srcs/utils/utils2.c
OBJS = $(SRCS:.c=.o)
OBJS = $(addprefix $(OBJDIR)/, $(SRCS:.c=.o))
OBJDIR = objs
RM = rm -rf

all: ${NAME}

$(LIBFT):
	$(MAKE) all -C ./libft

norm:
	norminette $(SRCS) includes libft

valgrind: all
	valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes --show-reachable=no --trace-children=yes ./$(NAME)

test: all
	./minishell

${NAME}: ${OBJS} $(LIBFT)
	${CC} ${FLAGS} ${OBJS} -lreadline $(LIBFT) -o ${NAME}

$(OBJDIR)/%.o: %.c
	@mkdir -p $(OBJDIR)
	@mkdir -p $(OBJDIR)/srcs
	@mkdir -p $(OBJDIR)/srcs/utils
	@mkdir -p $(OBJDIR)/srcs/builtins
	${CC} ${FLAGS} -c $< -o $@

clean:
	$(MAKE) clean -C ./libft
	$(RM) $(OBJS)
	${RM} ${OBJDIR}

fclean: clean
	$(MAKE) fclean -C ./libft
	$(RM) $(NAME)

re: fclean all
