# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: mberne <mberne@student.42lyon.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2021/11/10 15:30:20 by mberne            #+#    #+#              #
#    Updated: 2021/11/10 16:39:24 by mberne           ###   ########lyon.fr    #
#                                                                              #
# **************************************************************************** #

#========================================#
#=============== VARIABLES ==============#
#========================================#

#~~~~ Output ~~~~#

NAME		=	philo

#~~~~ Paths ~~~~#

VPATH		=	srcs/
PATH_OBJ	=	objs/
PATH_INC	=	include/

#~~~~ Sources ~~~~#
 
SRCS		=	main.c	\
				utils.c	\

#~~~~ Objects ~~~~#

OBJS		=	$(addprefix $(PATH_OBJ), $(SRCS:.c=.o))

#~~~~ Includes ~~~~#

INCS		=	$(addprefix $(PATH_INC), philo.h)

#~~~~ Macros ~~~~#

CC			=	gcc
CFLAGS		=	-Wall -Wextra -Werror
RM			=	rm -rf

#========================================#
#=============== TARGETS ================#
#========================================#

#~~~~ Main Rules ~~~~#

all :			$(NAME)

$(NAME) :		$(OBJS)
				$(CC) $(CFLAGS) $(OBJS) -o $(NAME) -I $(PATH_INC)

re :			fclean all

#~~~~ Compilation Rules ~~~~#

$(PATH_OBJ)%.o :	%.c $(INCS)
					@mkdir -p $(dir $@);
					$(CC) $(CFLAGS) -c $< -o $@ -I $(PATH_INC)

#~~~~ Cleaning Rules ~~~~#

clean :
				$(RM) $(PATH_OBJ)

fclean :		
				$(RM) $(PATH_OBJ) $(NAME)

.PHONY:		all clean fclean re