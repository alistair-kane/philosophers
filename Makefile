NAME	= philo
CC		= gcc
CFLAGS	= -Wall -Werror -Wextra
SRCS	= philo.c ft_atoi.c ft_calloc.c

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
	rm -f $(NAME) checker

re:	fclean all

.PHONY: all bonus clean fclean re