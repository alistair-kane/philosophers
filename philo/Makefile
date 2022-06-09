NAME	= philo
CC		= gcc
CFLAGS	= -Wall -Werror -Wextra -pthread #-fsanitize=thread
SRCS	= source/init.c source/main.c source/philo.c source/utils.c \
source/waiters.c source/libft/ft_atoi.c

OBJS	= ${SRCS:.c=.o}
BONUS_OBJS	= ${BONUS:.c=.o}

all: $(NAME)

$(NAME): $(OBJS)
	$(CC) $(CFLAGS) -o $(NAME) $(OBJS)

clean:
	rm -f $(OBJS) 

fclean: clean
	rm -f $(NAME)

re:	fclean all

.PHONY: all bonus clean fclean re