#include "scanner.h"
#include <iostream>

Scanner::Scanner(const std::string &src) : source(src) {}

std::vector<Token> Scanner::scanTokens()
{
    while (!isAtEnd())
    {
        start = current;
        scanToken();
        current++;
    }
    tokens.push_back({TokenType::EOF_TOKEN, "", line});
    std::cout << "EOF  null" << std::endl;

    if (hasLexicalErrors)
        std::exit(65);

    return tokens;
}

void Scanner::scanToken()
{
    char c = source[current];
    switch (c)
    {
    case ' ':
        break;
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
    case '=':
        if (current + 1 < source.size() && source[current + 1] == '=')
        {
            std::cout << "EQUAL_EQUAL == null" << std::endl;
            tokens.push_back({TokenType::EQUAL_EQUAL, "==", line});
            current++;
        }
        else
        {
            std::cout << "EQUAL = null" << std::endl;
            tokens.push_back({TokenType::EQUAL, "=", line});
        }
        break;
    case '!':
        if (current + 1 < source.size() && source[current + 1] == '=')
        {
            std::cout << "BANG_EQUAL != null" << std::endl;
            tokens.push_back({TokenType::BANG_EQUAL, "!=", line});
            current++;
        }
        else
        {
            std::cout << "BANG ! null" << std::endl;
            tokens.push_back({TokenType::BANG, "!", line});
        }
        break;
    case '<':
        if (current + 1 < source.size() && source[current + 1] == '=')
        {
            std::cout << "LESS_EQUAL <= null" << std::endl;
            tokens.push_back({TokenType::LESS_EQUAL, "<=", line});
            current++;
        }
        else
        {
            std::cout << "LESS < null" << std::endl;
            tokens.push_back({TokenType::LESS, "<", line});
        }
        break;
    case '>':
        if (current + 1 < source.size() && source[current + 1] == '=')
        {
            std::cout << "GREATER_EQUAL >= null" << std::endl;
            tokens.push_back({TokenType::GREATER_EQUAL, ">=", line});
            current++;
        }
        else
        {
            std::cout << "GREATER > null" << std::endl;
            tokens.push_back({TokenType::GREATER, ">", line});
        }
        break;

    default:
        std::cerr << "[line " << line << "] Error: Unexpected character: " << c << std::endl;
        hasLexicalErrors = true;
        break;
    }
}

bool Scanner::isAtEnd() const
{
    return current >= source.size();
}
