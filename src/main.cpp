#include <iostream>
#include <fstream>
#include <string>
#include "scanner.h"

int main(int argc, char *argv[])
{
    if (argc != 3)
    {
        std::cerr << "Usage: " << argv[0] << " tokenize <file>\n";
        return 1;
    }

    std::string command = argv[1];
    if (command != "tokenize")
    {
        std::cerr << "Unknown command: " << command << "\n";
        return 1;
    }

    std::ifstream file(argv[2]);
    if (!file)
    {
        std::cerr << "Could not open file: " << argv[2] << "\n";
        return 1;
    }

    std::string source((std::istreambuf_iterator<char>(file)),
                       std::istreambuf_iterator<char>());

    Scanner scanner(source);
    auto tokens = scanner.scanTokens();

    // For now, just print token count (replace with your own printing logic)
    std::cout << "Token count: " << tokens.size() << "\n";
    return 0;
}
