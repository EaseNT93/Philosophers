NAME = philo

NAME_B = philo_bonus

SRC = src/

SRC_B = src/

OBJ = $(SRC:.c=.o)

OBJ_B = $(SRC_B:.c=.o)

INCLUDE = includes/philo.h

CFLAGS = -Wall -Wextra -Werror

CC = gcc

RM = rm -rf

all: $(NAME)

$(NAME): $(OBJ)
		$(CC) $(CFLAGS) $(OBJ) -I $(INCLUDE) -o $(NAME)
		
bonus: $(OBJ_B)
		$(CC) $(CFLAGS) $(OBJ_B) -I $(INCLUDE) -o $(NAME_B)
		
clean:
		$(RM) $(OBJ) $(OBJ_B)
		
fclean: clean
		$(RM) $(NAME) $(NAME_B)
		
re: fclean all

.PHONY:	all fclean clean re bonus