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

    void scanToken();
    void addToken(TokenType token);
    void addToken(TokenType token, std::string value);
    bool matchCharacter(char c);
    bool isAtEnd() const;
};
