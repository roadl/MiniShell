# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: jeongbel <jeongbel@student.42seoul.kr>     +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2024/02/12 17:18:23 by yojin             #+#    #+#              #
#    Updated: 2024/10/29 16:10:44 by jeongbel         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #


CC = cc
CFLAGS = -Wall -Wextra -Werror
LDIR = ./
LIBFT = libft.a
LIBFT_DIR = ./libft
HEADER = ./minishell.h
SRCS = minishell.c built_in/built_in.c built_in/built_in_env.c built_in/built_in_util.c  built_in/built_in_util2.c\
	execute/execute.c execute/redi.c execute/execute_util.c env/env.c env/env_util.c \
	error/error.c init/init.c debug/debug.c util.c util2.c \
	parse/parse.c parse/tokenize.c parse/quote.c parse/quote_util.c parse/cmd.c \
	parse/parse_util.c parse/parse_error.c signal/signal.c signal/signal2.c
# LINKING_FLAGS = -lreadline -L${HOME}/.brew/opt/readline/lib
# COMFILE_FLAGS = -I${HOME}/.brew/opt/readline/include
LINKING_FLAGS = -lreadline -L/opt/homebrew/opt/readline/lib
COMFILE_FLAGS = -I/opt/homebrew/opt/readline/include
OBJS = $(SRCS:.c=.o)
NAME = minishell

all: $(NAME)

$(NAME): $(OBJS)
	make -j 8 -C $(LIBFT_DIR)
	$(CC) $(CFLAGS) $^ -o $@ -I$(LDIR) $(LIBFT_DIR)/$(LIBFT) $(LINKING_FLAGS) $(COMFILE_FLAGS)

%.o: %.c $(HEADER)
	$(CC) $(CFLAGS) -c $< -o $@ -I$(LDIR) $(COMFILE_FLAGS)

re: fclean all

clean:
	make -C $(LIBFT_DIR) clean
	rm -f $(OBJS)

fclean: clean
	make -C $(LIBFT_DIR) fclean
	rm -f $(NAME)

.PHONY: all re clean fclean
