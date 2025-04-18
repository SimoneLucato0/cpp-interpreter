#pragma once

#include <string>

enum class TokenType
{
    // Single-character tokens
    LEFT_PAREN,
    RIGHT_PAREN,
    LEFT_BRACE,
    RIGHT_BRACE,
    COMMA,
    DOT,
    SEMICOLON,
    PLUS,
    MINUS,
    STAR,
    SLASH,

    // One or two character tokens
    EQUAL,
    EQUAL_EQUAL,
    BANG,
    BANG_EQUAL,
    LESS,
    LESS_EQUAL,
    GREATER,
    GREATER_EQUAL,

    EOF_TOKEN
};

struct Token
{
    TokenType type;
    std::string lexeme;
    int line;
};