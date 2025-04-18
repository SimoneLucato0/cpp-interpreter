CXX = g++
CXXFLAGS = -std=c++11 -Wall -Wextra

SRC = src/main.cpp src/helper.cpp src/scanner.cpp
INC = -Isrc

all: tokenize

tokenize: $(SRC)
	$(CXX) $(CXXFLAGS) $(INC) -o tokenize $(SRC)

clean:
	rm -f tokenize
