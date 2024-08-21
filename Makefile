NAME = minishell

CC = cc

LIBFT = ./libft/libft.a

FLAGS = -Wall -Wextra  -Iincludes -I./libft -g -fsanitize=address

SRCS = main.c srcs/signals.c srcs/command.c srcs/executing.c srcs/utils.c srcs/lexing_handling.c

OBJS = $(SRCS:.c=.o)

RM = rm -f

all: ${NAME}

$(LIBFT):
	$(MAKE) all -C ./libft

norm:
	norminette $(SRCS) includes

valgrind: all
	valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes --show-reachable=yes ./$(NAME)

test: all
	./minishell

${NAME}: ${OBJS} $(LIBFT)
	${CC} ${FLAGS} ${OBJS} -lreadline $(LIBFT) -o ${NAME}
	make clean

%.o: %.c
	${CC} ${FLAGS} -c $< -o $@

clean:
	$(MAKE) clean -C ./libft
	$(RM) $(OBJS)

fclean: clean
	$(MAKE) fclean -C ./libft
	$(RM) $(NAME)

re: fclean all