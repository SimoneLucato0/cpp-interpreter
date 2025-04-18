#pragma once
#include <string>
#include <vector>

enum class TokenType
{
    // Add your token types here
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
    EQUAL,
    EQUAL_EQUAL,
    BANG,
    BANG_EQUAL,
    EOF_TOKEN
};

struct Token
{
    TokenType type;
    std::string lexeme;
    int line;
};

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
    bool isAtEnd() const;
};
