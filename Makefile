CXX = g++
CXXFLAGS = -std=c++17 -Wall -Wextra

SRC = src/main.cpp src/helper.cpp src/scanner/scanner.cpp src/parser/parser.cpp src/token/tokenType.cpp
INC = -Isrc

all: tokenize

tokenize: $(SRC)
	$(CXX) $(CXXFLAGS) $(INC) -o tokenize $(SRC)

generateAst: tool/generateAst.cpp
	$(CXX) $(CXXFLAGS) $(INC) -o generateAst tool/generateAst.cpp

printAst: tool/main.cpp
	$(CXX) $(CXXFLAGS) $(INC) -o print tool/main.cpp

recreateAsts:
	javac ./tool/GenerateAst.java
	java -cp ./tool GenerateAst ./src/java/src/main/java/com/interpreter/myinterpreter

clean:
	rm -f tokenize
