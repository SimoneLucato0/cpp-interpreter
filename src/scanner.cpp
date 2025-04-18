#include "scanner.h"

Scanner::Scanner(const std::string &src) : source(src) {}

std::vector<Token> Scanner::scanTokens()
{
    while (!isAtEnd())
    {
        start = current;
        scanToken();
    }
    tokens.push_back({TokenType::EOF_TOKEN, "", line});
    return tokens;
}

void Scanner::scanToken()
{
    // Minimal example: skip whitespace, recognize numbers
    char c = source[current++];
    switch (c)
    {
    case '+':
        tokens.push_back({TokenType::PLUS, "+", line});
        break;
    case '-':
        tokens.push_back({TokenType::MINUS, "-", line});
        break;
    // ... handle more tokens
    default:
        if (isdigit(c))
        {
            // Scan a number (simplified)
            size_t numStart = current - 1;
            while (current < source.size() && isdigit(source[current]))
                ++current;
            tokens.push_back({TokenType::NUMBER, source.substr(numStart, current - numStart), line});
        }
        // ... handle identifiers, etc.
        break;
    }
}

bool Scanner::isAtEnd() const
{
    return current >= source.size();
}
