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
    case '(':
        std::cout << "LEFT_PAREN ( null" << std::endl;
        tokens.push_back({TokenType::LEFT_PAREN, "(", line});
        break;
    case ')':
        std::cout << "RIGHT_PAREN ) null" << std::endl;
        tokens.push_back({TokenType::RIGHT_PAREN, ")", line});
        break;
    case '{':
        std::cout << "LEFT_BRACE { null" << std::endl;
        tokens.push_back({TokenType::LEFT_BRACE, "{", line});
        break;
    case '}':
        std::cout << "RIGHT_BRACE } null" << std::endl;
        tokens.push_back({TokenType::RIGHT_BRACE, "}", line});
        break;
    case ',':
        std::cout << "COMMA , null" << std::endl;
        tokens.push_back({TokenType::COMMA, ",", line});
        break;
    case '.':
        std::cout << "DOT . null" << std::endl;
        tokens.push_back({TokenType::DOT, ".", line});
        break;
    case ';':
        std::cout << "SEMICOLON ; null" << std::endl;
        tokens.push_back({TokenType::SEMICOLON, ";", line});
        break;
    case '+':
        std::cout << "PLUS + null" << std::endl;
        tokens.push_back({TokenType::PLUS, "+", line});
        break;
    case '-':
        std::cout << "MINUS - null" << std::endl;
        tokens.push_back({TokenType::MINUS, "-", line});
        break;
    case '*':
        std::cout << "STAR * null" << std::endl;
        tokens.push_back({TokenType::STAR, "*", line});
        break;
    default:
        break;
    }
}

bool Scanner::isAtEnd() const
{
    return current >= source.size();
}
