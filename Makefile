# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: tsaari <tsaari@student.hive.fi>            +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2024/04/11 10:39:11 by tsaari            #+#    #+#              #
#    Updated: 2024/04/23 15:34:46 by tsaari           ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = minishell

CC = clang
RM = rm -rf
CFLAGS =  -g -lreadline
#FSFLAGS =-fsanitize=address
LIBFT = libft/libft.a

SRC_DIR = src/
SRCS = main.c parsing.c parse_utils.c node_utils.c ft_pipex_split.c free_and_error.c init.c
OBJS	= $(SRCS:%.c=$(OBJ_DIR)/%.o)

OBJ_DIR = obj

all: $(NAME)

$(OBJ_DIR):
	@mkdir -p $(OBJ_DIR)

$(OBJ_DIR)/%.o: $(SRC_DIR)%.c
	@$(CC) -c $< -o $@
	@echo "\033[0;36mObject $@ [\033[0;32mOK\033[0;36m]\033[0m"

$(LIBFT):
	@make -C libft

$(NAME):  $(OBJ_DIR) $(OBJS) $(LIBFT) libft/*.c
	@$(CC) $(OBJS) $(CFLAGS) $(LIBFT) $(FSFLAGS) -o $(NAME)
	@echo "\033[1;32mMinishell compile success!\n\033[0m"

clean:
	@$(RM) $(OBJ_DIR)
	@make clean -C libft
	@echo "\033[0;36mClean Minishell [\033[0;32mDONE\033[0;36m]\033[0m"

fclean: clean
	@$(RM) $(NAME)
	@make fclean -C libft
	@echo "\033[0;36mFClean Minishell [\033[0;32mDONE\033[0;36m]\033[0m"

re: fclean all

.PHONY: all clean fclean re
