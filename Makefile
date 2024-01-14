##
## EPITECH PROJECT, 2023
## B-CPP-500-STG-5-2-rtype-remi.mergen
## File description:
## Makefile
##

CXXFLAGS = -std=c++17
LDFLAGS = -lraylib -lJolt

NAME = sampleJolt

SRC_DIR = .

SRC = joltSample.cpp \

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
