##
## EPITECH PROJECT, 2023
## B-CPP-500-STG-5-2-rtype-remi.mergen
## File description:
## Makefile
##

CXXFLAGS = -std=c++17 -I ../../include/ -I ../../include/common -I ../../include/game_engine
LDFLAGS = -lraylib

NAME = example_game_engine

SRC_DIR = .

SRC = main.cpp \
      $(SRC_DIR)/core/GameEngine.cpp \
	  $(SRC_DIR)/core/window/Window.cpp \
	  $(SRC_DIR)/core/event/Event.cpp \
	  $(SRC_DIR)/ecs/components/Shapes.cpp \
	  $(SRC_DIR)/ecs/components/Behaviour.cpp \
	  $(SRC_DIR)/ecs/systems/Render.cpp \
	  $(SRC_DIR)/ecs/systems/Physics.cpp \
	  $(SRC_DIR)/ecs/systems/Behaviour.cpp \
	  $(SRC_DIR)/ecs/systems/Animations.cpp \
	  $(SRC_DIR)/ecs/systems/Input.cpp \
	  $(SRC_DIR)/ecs/Entity.cpp \

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
