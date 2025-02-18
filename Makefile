NAME		= philo
CC			= cc
CFLAGS		= -Wall -Wextra -Werror -pthread
RM			= rm -f

SRCS		= src/main.c \
			  src/init_and_parse.c \
			  src/threads.c \
			  src/activity.c \
			  src/utils_lib.c \
			  src/utils_philo.c

OBJS		= $(SRCS:.c=.o)

INCLUDE		= -I include/

all: $(NAME)

$(NAME): $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) -o $(NAME)

%.o: %.c
	$(CC) $(CFLAGS) $(INCLUDE) -c $< -o $@

clean:
	$(RM) $(OBJS)

fclean: clean
	$(RM) $(NAME)

re: fclean all

.PHONY: all clean fclean re