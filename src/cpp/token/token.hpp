#pragma once

#include <iostream>
#include <string>

#include "tokenType.hpp"

class Token {
   public:
    TokenType type;
    std::string lexeme;
    std::string literal;
    int line;

    Token(TokenType type, std::string lexeme, std::string literal, int line)
        : type(type), lexeme(lexeme), literal(literal), line(line) {}

    friend std::ostream& operator<<(std::ostream& os, const Token& t) {
        os << "Token(type=" << TokenTypeToString(t.type) << ", lexeme=" << t.lexeme << ", literal=" << t.literal << ")";
        return os; 
    }
};