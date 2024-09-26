# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: jeongbel <jeongbel@student.42seoul.kr>     +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2024/02/12 17:18:23 by yojin             #+#    #+#              #
#    Updated: 2024/09/26 11:54:39 by jeongbel         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #


CC = cc
CFLAGS = -Wall -Wextra -Werror
LDIR = ./
LIBFT = libft.a
LIBFT_DIR = ./libft
HEADER = ./minishell.h
SRCS = minishell.c built_in/built_in.c built_in/built_in_env.c \
	execute/execute.c execute/redi.c env/env.c env/env_util.c \
	error/error.c init/init.c debug/debug.c util.c util2.c \
	parse/parse.c parse/tokenize.c
LINKING_FLAGS = -lreadline -L${HOME}/.brew/opt/readline/lib
COMFILE_FLAGS = -I${HOME}/.brew/opt/readline/include
OBJS = $(SRCS:.c=.o)
NAME = minishell

all: $(NAME)

$(NAME): $(OBJS)
	make -j 8 -C $(LIBFT_DIR)
	$(CC) $(CFLAGS) $^ -o $@ -I$(LDIR) $(LIBFT_DIR)/$(LIBFT) -lreadline

%.o: %.c $(HEADER)
	$(CC) $(CFLAGS) -c $< -o $@ -I$(LDIR)

re: fclean all

clean:
	make -C $(LIBFT_DIR) clean
	rm -f $(OBJS)

fclean: clean
	make -C $(LIBFT_DIR) fclean
	rm -f $(NAME)

.PHONY: all re clean fclean
