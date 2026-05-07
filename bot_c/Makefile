NAME = bot_client

SRC = main.cpp 
HDR = header_client.hpp
CC = c++
FLAGS = -Wall -Wextra -Werror -std=c++98 -ggdb
RM = rm -rf
OBJS_DIR = objs

OBJS = $(addprefix $(OBJS_DIR)/, $(SRC:.cpp=.o))

all : $(NAME)

$(OBJS_DIR)/%.o : %.cpp $(HDR) Makefile
	mkdir -p $(OBJS_DIR)
	$(CC) $(FLAGS) -c $< -o $@

$(NAME): $(OBJS) 
	$(CC) $(FLAGS) $(OBJS) -o $(NAME)

clean: 
	$(RM) $(OBJS_DIR)
		 
fclean: clean
	$(RM) $(NAME)
re: fclean all 

.PHONY: all clean fclean re 
