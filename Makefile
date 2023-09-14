NAME := wifi

CC := g++
RM := rm -rf

OBJDIR := .objFiles

OPTS := -std=c++17

FILES := main wifi_utils

SRC := $(FILES:=.cpp)
OBJ := $(addprefix $(OBJDIR)/, $(FILES:=.o))
HEADER := wifilib.hpp

.PHONY: all clean fclean re

all: $(NAME)

$(NAME): $(OBJ)
	@$(CC) $^ -o $(NAME)

$(OBJDIR)/%.o: %.cpp $(HEADER)
	@mkdir -p $(dir $@)
	@$(CC) $(OPTS) -c $< -o $@

clean:
	@$(RM) $(OBJDIR)

fclean: clean
	@$(RM) $(NAME)

re: fclean all
