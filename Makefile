# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: elerazo- <marvin@42.fr>                    +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2025/04/07 13:27:33 by elerazo-          #+#    #+#              #
#    Updated: 2026/02/07 20:38:26 by elerazo          ###   ########.fr        #
#                                                                              #
# **************************************************************************** #
NAME		=	ircserv
CC			=	c++
CFLAGS		=	-Wall -Wextra -Werror -std=c++98 -I inc/
OBJDIR		=	build
SRCSDIR		=	src
SRCS		=	main.cpp 

OBJS		=	$(addprefix $(OBJDIR)/, ${SRCS:.cpp=.o})

DEPS		=	$(addprefix $(OBJDIR)/, ${OBJS:.o=.d})

PURPLE		=	\033[0;33m
BLUE		=	\033[0;33m
GREEN		=	\033[0;32m
RED			=	\033[0;31m
RESET		=	\033[m

all: banner $(NAME)

banner:
	@printf "%b" "$(PURPLE)\n"
	@echo "# ========================== #"
	@echo "#            IRC             #"
	@echo "#        by:                 #"
	@echo "# ========================== #"
	@printf "%b" "\n$(RESET)"

$(OBJS): $(OBJDIR)/%.o : $(SRCSDIR)/%.cpp Makefile | $(OBJDIR)
	@printf "%-42b" "$(BLUE)compiling... $(PURPLE)$(@F)$(RESET)\n"
	@$(CC) $(CFLAGS) -MMD -c $< -o $@

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

re:    fclean all

-include $(DEPS)

.PHONY: all banner clean fclean re
