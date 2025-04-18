#include "scanner.h"
#include <iostream>

Scanner::Scanner(const std::string &src) : source(src) {}

std::vector<Token> Scanner::scanTokens()
{
    while (!isAtEnd())
    {
        start = current;
        scanToken();
    }
    tokens.push_back({TokenType::EOF_TOKEN, "", line});
    std::cout << "EOF  null" << std::endl;
    return tokens;
}

void Scanner::scanToken()
{
    // Minimal example: skip whitespace, recognize numbers
    char c = source[current++];
    switch (c)
    {
    case '+':
        std::cout << "PLUS +" << std::endl;
        // tokens.push_back({TokenType::PLUS, "+", line});
        break;
    case '-':
        std::cout << "MINUS -" << std::endl;
        // tokens.push_back({TokenType::MINUS, "-", line});
        break;
    case ' ':
        break;
    // ... handle more tokens
    default:
        if (isdigit(c))
        {
            // Scan a number (simplified)
            size_t numStart = current - 1;
            while (current < source.size() && isdigit(source[current]))
                ++current;
            std::cout << "NUMBER " << source.substr(numStart, current - numStart) << std::endl;
            // tokens.push_back({TokenType::NUMBER, source.substr(numStart, current - numStart), line});
        }
        else
        {
            std::cerr << "Unrecognized token: " << c << std::endl;
        }
        // ... handle identifiers, etc.
        break;
    }
}

bool Scanner::isAtEnd() const
{
    return current >= source.size();
}
