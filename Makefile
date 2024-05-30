# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: pkangas <pkangas@student.hive.fi>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2024/04/11 10:39:11 by tsaari            #+#    #+#              #
#    Updated: 2024/05/30 10:35:09 by pkangas          ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = minishell

CC = cc
RM = rm -rf
CFLAGS =  -g -Wall -Wextra -Werror # remove the -g flag eventually ??
#FSFLAGS =-fsanitize=address
LIBFT = libft/libft.a
# Koulussa tää --> -I /Users/$(USER)/.brew/opt/readline/include ||| Panun kotona --> -I /usr/local/Cellar/readline/8.2.10/include
HEADERS = -I includes -I /Users/$(USER)/.brew/opt/readline/include

SRC_DIR = src/
SRCS = main.c parsing.c parse_utils.c parse_node_utils.c ft_pipex_split.c free_and_error.c parse_init.c \
	parse_expansion.c parse_split_quotes.c parse_expand.c parse_exp_node_utils.c \
	ft_cd.c ft_echo.c ft_empty_export.c ft_pwd_env.c ft_unset_export.c \
	env_lst_utils.c expand_env_var.c get_env_node.c make_env_node.c save_env_list.c \
	execute_command.c get_paths.c make_env_var_array.c \
	ft_heredoc.c ft_redirect.c ft_exit.c \
	write_error.c check_for_built_in.c execute_built_in.c handle_command.c \
	pids_and_pipes.c make_processes.c ft_open_functions.c signal_handling.c extras.c \
	expand_heredoc_env_var.c check_pipe_and_quot_syntax.c ft_cd_analyze_path.c

OBJS	= $(SRCS:%.c=$(OBJ_DIR)/%.o)

OBJ_DIR = obj

all: $(NAME)

$(OBJ_DIR):
	@mkdir -p $(OBJ_DIR)

$(OBJ_DIR)/%.o: $(SRC_DIR)%.c
	@$(CC) -c $< -o $@ $(HEADERS) $(CFLAGS)
	@echo "\033[0;36mObject $@ [\033[0;32mOK\033[0;36m]\033[0m"

$(LIBFT):
	@make -C libft

# Koulussa tää --> -L /Users/$(USER)/.brew/opt/readline/lib  |||  Panun kotona --> -L /usr/local/Cellar/readline/8.2.10/lib
$(NAME):  $(OBJ_DIR) $(OBJS) $(LIBFT)
	@$(CC) $(OBJS) $(CFLAGS) -lreadline -L /Users/$(USER)/.brew/opt/readline/lib $(LIBFT) $(FSFLAGS) -o $(NAME)
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
