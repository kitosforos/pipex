# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: maralons <maralons@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2022/08/23 17:15:53 by maralons          #+#    #+#              #
#    Updated: 2022/08/23 18:35:53 by maralons         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

CC = gcc
CFLAGS = -Wall -Wextra -Werror

NAME = pipex

SRCS = srcs/pipex.c
OBJS = ${SRCS:.c=.o}
INCLUDES = -Iincludes

DEF_COLOR = \033[0;39m
GRAY = \033[0;90m
RED = \033[0;91m
GREEN = \033[0;92m
YELLOW = \033[0;93m
BLUE = \033[0;94m
MAGENTA = \033[0;95m
CYAN = \033[0;96m
WHITE = \033[0;97m

all: $(NAME)

$(NAME): $(OBJS)
	@make -C libft
	@$(CC) $(OBJS) -Llibft -lft -o $(NAME)
	@echo "$(MAGENTA)---------> Pipex created$(DEF_COLOR)"

srcs/%.o: srcs/%.c
	@$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

clean:
	@make clean -C libft
	@rm -rf $(OBJS)
	@echo "$(RED)---------> Objects from pipex cleaned$(DEF_COLOR)"

fclean:
	@make fclean -C libft
	@rm -rf $(OBJS) $(NAME)
	@echo "$(RED)---------> Everything from pipex cleaned$(DEF_COLOR)"

re: fclean all