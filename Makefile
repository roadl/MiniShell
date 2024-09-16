# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: yojin <yojin@student.42.fr>                +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2024/02/12 17:18:23 by yojin             #+#    #+#              #
#    Updated: 2024/07/17 16:57:59 by yojin            ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

CC = cc
CFLAGS = 
LDIR = ./
LIBFT = libft.a
LIBFT_DIR = ./libft
HEADER = ./minishell.h
SRCS = minishell.c built_in/built_in.c execute/execute.c env/env.c error/error.c init/init.c
LINKING_FLAGS = -lreadline -L${HOME}/.brew/opt/readline/lib
COMFILE_FLAGS = -I${HOME}/.brew/opt/readline/include
OBJS = $(SRCS:.c=.o)
NAME = minishell

all: $(NAME)

$(NAME): $(OBJS)
	make -j 8 -C $(LIBFT_DIR)
	$(CC) $(CFLAGS) $^ -o $@ -I$(LDIR) $(LIBFT_DIR)/$(LIBFT) -lreadline

%.o: %.c $(HEADER)
	$(CC) $(CFLAGS) -c $< -o $@ -I$(LDIR) -lreadline

re: fclean all

clean:
	make -C $(LIBFT_DIR) clean
	rm -f $(OBJS)

fclean: clean
	make -C $(LIBFT_DIR) fclean
	rm -f $(NAME)

.PHONY: all re clean fclean
