#pragma once
#include <string>
#include <vector>

#include "token.h"

class Scanner
{
public:
    Scanner(const std::string &source);
    std::vector<Token> scanTokens();

private:
    const std::string &source;
    size_t start = 0, current = 0;
    int line = 1;
    std::vector<Token> tokens;
    bool hasLexicalErrors = false;

    char advance();
    char peek() const;
    char peekNext() const;
    bool isAtEnd() const;

    void scanToken();
    void addToken(TokenType token);
    void addToken(TokenType token, std::string lexeme, std::string literal);
    bool matchCharacter(char c);
};
