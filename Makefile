NAME = minishell

CC = cc

LIBFT = ./libft/libft.a

FLAGS = -Wall -Wextra -Werror -Iincludes -I./libft

SRCS = main.c 

OBJS = $(SRCS:.c=.o)

RM = rm -f

all: ${NAME}

norm:
	norminette $(SRCS) includes

valgrind: all
	valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes ./minishell

exe: all
	./minishell

${NAME}: ${OBJS}
	${CC} ${FLAGS} ${OBJS} -lreadline -o ${NAME}

%.o: %.c
	${CC} ${FLAGS} -c $< -o $@

clean:
	${RM} ${OBJS}

fclean: clean
	${RM} ${NAME}

re: fclean all