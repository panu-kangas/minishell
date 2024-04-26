# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: pkangas <pkangas@student.hive.fi>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2024/04/11 10:39:11 by tsaari            #+#    #+#              #
#    Updated: 2024/04/23 18:06:22 by pkangas          ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = minishell

CC = clang
RM = rm -rf
CFLAGS =  -g -lreadline
#FSFLAGS =-fsanitize=address
LIBFT = libft/libft.a
HEADERS = -I includes

SRC_DIR = src/
SRCS = main.c parsing.c parse_utils.c node_utils.c ft_pipex_split.c free_and_error.c init.c \
	ft_cd.c ft_echo.c ft_empty_export.c ft_pwd_env.c ft_unset_export.c \
	env_lst_utils.c expand_env_var.c get_env_node.c make_env_node.c save_env_list.c \
	execute_command.c get_paths.c make_env_var_array.c \
	ft_heredoc.c ft_redirect.c \
	write_error.c check_for_built_in.c execute_built_in.c handle_command.c \
	pids_and_pipes.c make_processes.c ft_open_functions.c
OBJS	= $(SRCS:%.c=$(OBJ_DIR)/%.o)

OBJ_DIR = obj

all: $(NAME)

$(OBJ_DIR):
	@mkdir -p $(OBJ_DIR)

# remove the -g flag eventually
$(OBJ_DIR)/%.o: $(SRC_DIR)%.c
	@$(CC) -c $< -o $@ $(HEADERS) -g
	@echo "\033[0;36mObject $@ [\033[0;32mOK\033[0;36m]\033[0m"

$(LIBFT):
	@make -C libft

# libft/*.c might be a norm-problem because of the asterisk. Let's see!
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
