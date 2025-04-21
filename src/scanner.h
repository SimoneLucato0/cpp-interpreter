#pragma once
#include <string>
#include <unordered_map>
#include <vector>

#include "token.h"

class Scanner {
   public:
    Scanner(const std::string &source);
    std::vector<Token> scanTokens();

   private:
    const std::string &source;
    size_t start = 0, current = 0;
    int line = 1;
    std::vector<Token> tokens;
    bool hasLexicalErrors = false;
    const std::unordered_map<std::string, TokenType> reservedKeywordsMap = {
        {"and", TokenType::AND},     {"class", TokenType::CLASS},   {"else", TokenType::ELSE},
        {"false", TokenType::FALSE}, {"for", TokenType::FOR},       {"fun", TokenType::FUN},
        {"if", TokenType::IF},       {"nil", TokenType::NIL},       {"or", TokenType::OR},
        {"print", TokenType::PRINT}, {"return", TokenType::RETURN}, {"super", TokenType::SUPER},
        {"this", TokenType::THIS},   {"true", TokenType::TRUE},     {"var", TokenType::VAR},
        {"while", TokenType::WHILE},
    };

    char advance();
    char peek() const;
    char peekNext() const;
    bool isAtEnd() const;

    void scanToken();
    void addToken(TokenType token);
    void addToken(TokenType token, std::string lexeme, std::string literal);
    bool matchCharacter(char c);
};
