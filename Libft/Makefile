# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: yer-raki <yer-raki@student.1337.ma>        +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2019/10/16 21:39:19 by yer-raki          #+#    #+#              #
#    Updated: 2021/07/08 12:57:20 by mhaddi           ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = libft.a
CC = gcc
INC = ./
SRC = ./*.c
FLAGS = -Wall -Wextra -Werror
ARG = -c
OUT = *.o

all: $(NAME)

$(NAME):
	@$(CC) $(ARG) $(FLAGS) $(SRC) -I $(INC)
	@ar rc $(NAME) $(OUT)
clean:
	@rm -f $(OUT)
fclean: clean
	@rm -f $(NAME)
re : fclean $(NAME)
