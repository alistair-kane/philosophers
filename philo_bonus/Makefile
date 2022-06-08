NAME	= philo_bonus
CC		= gcc
CFLAGS	= -Wall -Werror -Wextra -pthread -fsanitize=thread
SRCS	= philo.c init.c utils.c checks.c libft/ft_atoi.c libft/ft_calloc.c libft/ft_itoa.c \
libft/ft_strjoin.c libft/ft_strlen.c

OBJS	= ${SRCS:.c=.o}
BONUS_OBJS	= ${BONUS:.c=.o}

all: $(NAME)

$(NAME): $(OBJS)
	$(CC) $(CFLAGS) -o $(NAME) $(OBJS)

# bonus: $(BONUS_OBJS)
# 	rm -f $(BONUS_OBJS)

clean:
	rm -f $(OBJS) 
#$(BONUS_OBJS)

fclean: clean
	rm -f $(NAME)

re:	fclean all

.PHONY: all bonus clean fclean re