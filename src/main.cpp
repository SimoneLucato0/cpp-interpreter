#include <fstream>
#include <iostream>
#include <string>

#include "scanner.h"

int main(int argc, char const *argv[]) {
    if (argc != 3) {
        std::cerr << "Usage: " << argv[0] << " tokenize <file>\n";
        return 1;
    }

    std::string command = argv[1];
    if (command != "tokenize") {
        std::cerr << "Unknown command: " << command << "\n";
        return 1;
    }

    std::ifstream file(argv[2]);
    if (!file) {
        std::cerr << "Could not open file: " << argv[2] << "\n";
        return 1;
    }

    std::string source((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
    if (!file.eof() && file.fail()) {
        std::cerr << "File read failed or was incomplete." << std::endl;
        return 1;
    }

    Scanner scanner(source);
    auto tokens = scanner.scanTokens();

    return 0;
}
