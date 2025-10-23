# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: dlesieur <dlesieur@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2025/10/23 19:03:21 by dlesieur          #+#    #+#              #
#    Updated: 2025/10/23 20:51:58 by dlesieur         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

include common.mk

all:

$(NAME): $(OBJS)
	

clean:
	$(RM) $(OBJS)

fclean: clean
	$(RM) $()

re:
	
.PHONY: all clean 