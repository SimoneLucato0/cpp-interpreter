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

build:
	cd src/java && mvn clean package && cd .. && cd ..

test:
	java -jar ./src/java/target/myinterpreter-1.0-SNAPSHOT.jar tests/t1.txt

clean:
	rm -f tokenize
