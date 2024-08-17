NAME = minishell

CC = cc

LIBFT = ./libft/libft.a

FLAGS = -Wall -Wextra -Werror -Iincludes -I./libft

SRCS = main.c 

OBJS = $(SRCS:.c=.o)

RM = rm -f

all: ${NAME}

$(LIBFT):
	$(MAKE) -C ./libft

norm:
	norminette $(SRCS) includes

valgrind: all
	valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes ./minishell

exe: all
	./minishell

${NAME}: ${OBJS} $(LIBFT)
	${CC} ${FLAGS} ${OBJS} -lreadline $(LIBFT) -o ${NAME}

%.o: %.c
	${CC} ${FLAGS} -c $< -o $@

clean: 
	$(MAKE) clean -C ./libft
	$(RM) $(OBJS)

fclean: clean
	$(MAKE) fclean -C ./libft
	$(RM) $(NAME)

re: fclean all