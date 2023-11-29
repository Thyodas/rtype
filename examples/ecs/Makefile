##
## EPITECH PROJECT, 2023
## B-CPP-500-STG-5-2-rtype-remi.mergen
## File description:
## Makefile
##


CXXFLAGS = -std=c++17 -I ../../include/
LDFLAGS = -lsfml-graphics -lsfml-window -lsfml-system

NAME = example_ecs

SRC_DIR = ../../src

SRC = main.cpp \
      $(SRC_DIR)/common/ecs/Entity.cpp \
	  $(SRC_DIR)/common/ecs/systems/Controls.cpp \
	  $(SRC_DIR)/common/ecs/systems/Draw.cpp \
	  $(SRC_DIR)/common/ecs/systems/Physics.cpp \

OBJ = $(SRC:.cpp=.o)

all: $(NAME)

$(NAME): $(OBJ)
	g++ $(OBJ) -o $(NAME) $(LDFLAGS)

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -f $(OBJ)

fclean: clean
	rm -f $(NAME)

re: fclean all

.PHONY: all clean fclean re
