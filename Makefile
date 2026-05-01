# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: nalesso <nalesso@student.42.fr>            +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2026/04/20 15:12:31 by elerazo-          #+#    #+#              #
#    Updated: 2026/04/21 19:29:55 by nalesso          ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME		=	ircserv
CC			=	c++
CFLAGS		=	-Wall -Wextra -Werror -std=c++98 -g3 -fsanitize=address -I inc/
OBJDIR		=	build
SRCSDIR		=	src
SRCS		=	main.cpp Server.cpp Channel.cpp Client.cpp \
				Command.cpp Parser.cpp funtions_aux.cpp

OBJS		=	$(addprefix $(OBJDIR)/, ${SRCS:.cpp=.o})
DEPS		=   $(addprefix $(OBJDIR)/, ${SRCS:.cpp=.d})

PURPLE		=	\033[0;33m
BLUE		=	\033[0;33m
GREEN		=	\033[0;32m
RED			=	\033[0;31m
RESET		=	\033[m

all: banner $(NAME)

banner:
	@printf "%b" "$(PURPLE)\n"
	@echo "# ========================== #"
	@echo "#            IRC             #"
	@echo "#        by:                 #"
	@echo "# ========================== #"
	@printf "%b" "\n$(RESET)"

$(OBJS): $(OBJDIR)/%.o : $(SRCSDIR)/%.cpp Makefile | $(OBJDIR)
	@printf "%-42b" "$(BLUE)compiling... $(PURPLE)$(@F)$(RESET)\n"
	@$(CC) $(CFLAGS) -MMD -MF $(OBJDIR)/$*.d -c $< -o $@

$(OBJDIR):
	@-mkdir $(OBJDIR)

$(NAME): $(OBJS)
	@printf "%-42b" "$(BLUE)linking... $(PURPLE)$(@F)$(RESET)\n"
	@$(CC) $(CFLAGS) $^ -o $@

fclean: banner clean
	@printf "%b" "$(BLUE)$(@)ing...$(RESET)\n"
	@rm -rf $(NAME)

clean: banner
	@printf "%b" "$(BLUE)$(@)ing...$(RESET)\n"
	@rm -rf $(OBJDIR)

re:	fclean all

-include $(DEPS)

.PHONY: all banner clean fclean re
