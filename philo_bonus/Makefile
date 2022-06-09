NAME	= philo_bonus
CC		= gcc
CFLAGS	= -Wall -Werror -Wextra -pthread # -fsanitize=thread -g
SRCS	= source/main.c source/philo.c source/init.c source/utils.c source/waiters.c \
source/libft/ft_atoi.c source/libft/ft_calloc.c source/libft/ft_itoa.c \
source/libft/ft_strjoin.c source/libft/ft_strlen.c

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