# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: eleotard <eleotard@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2022/05/15 14:37:48 by eleotard          #+#    #+#              #
#    Updated: 2023/01/21 21:59:34 by eleotard         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME	= libft.a

BONUS	= libft.a

SRCS	= ft_isalpha.c ft_isdigit.c ft_isalnum.c 			\
		ft_isascii.c ft_isprint.c ft_strlen.c ft_memset.c 	\
		ft_bzero.c ft_memcpy.c ft_strlcpy.c					\
		ft_toupper.c ft_tolower.c ft_strchr.c ft_strrchr.c 	\
		ft_strncmp.c ft_memchr.c ft_memcmp.c ft_strnstr.c 	\
		ft_atoi.c ft_strlcat.c ft_memmove.c ft_calloc.c		\
		ft_strdup.c ft_substr.c	ft_strjoin.c ft_strtrim.c	\
		ft_split.c ft_itoa.c 								\
		ft_strmapi.c ft_striteri.c ft_putchar_fd.c			\
		ft_putstr_fd.c ft_putendl_fd.c ft_putnbr_fd.c		\

SRC_BONUS 	= ft_lstnew.c ft_lstadd_front.c ft_lstsize.c ft_lstlast.c	\

OBJS	= ${SRCS:.c=.o}

BOBJS	= ${SRC_BONUS:.c=.o}



CC		= gcc

RM		= rm -f

CFLAGS	= -Wall -Wextra -Werror -g3

.c.o:
			${CC} ${CFLAGS} -c $< -o ${<:.c=.o}

all:		${NAME}

${NAME}:	${OBJS}
			ar rc ${NAME} ${OBJS}
			ranlib ${NAME}
			
bonus: ${BONUS}

${BONUS}:	${BOBJS} ${OBJS}
			ar rc ${BONUS} ${OBJS} ${BOBJS} 
			ranlib ${BONUS}

clean:
			${RM} ${OBJS} ${BOBJS}

fclean:		clean
			${RM} ${NAME} ${BONUS}

re:			fclean all

.PHONY: all clean fclean re bonus
