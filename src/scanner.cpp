#include <iostream>
#include <stdexcept>

#include "scanner.h"
#include "helper.h"

Scanner::Scanner(const std::string &src) : source(src) {}

std::vector<Token> Scanner::scanTokens()
{
    while (!isAtEnd())
    {
        start = current;
        scanToken();
    }
    addToken(TokenType::EOF_TOKEN);

    if (hasLexicalErrors)
        std::exit(65);

    return tokens;
}

void Scanner::scanToken()
{
    char c = source[current++];
    switch (c)
    {
    case ' ':
    case '\t':
    case '\r':
        break;
    case '\n':
        if (c == '\n')
            line++;
        break;
    case '(':
        addToken(TokenType::LEFT_PAREN);
        break;
    case ')':
        addToken(TokenType::RIGHT_PAREN);
        break;
    case '{':
        addToken(TokenType::LEFT_BRACE);
        break;
    case '}':
        addToken(TokenType::RIGHT_BRACE);
        break;
    case ',':
        addToken(TokenType::COMMA);
        break;
    case '.':
        addToken(TokenType::DOT);
        break;
    case ';':
        addToken(TokenType::SEMICOLON);
        break;
    case '+':
        addToken(TokenType::PLUS);
        break;
    case '-':
        addToken(TokenType::MINUS);
        break;
    case '*':
        addToken(TokenType::STAR);
        break;
    case '/':
        if (matchCharacter('/'))
            while (!isAtEnd() && source[current] != '\n')
                current++;
        else
            addToken(TokenType::SLASH);
        break;
    case '=':
        addToken(matchCharacter('=') ? TokenType::EQUAL_EQUAL : TokenType::EQUAL);
        break;
    case '!':
        addToken(matchCharacter('=') ? TokenType::BANG_EQUAL : TokenType::BANG);
        break;
    case '<':
        addToken(matchCharacter('=') ? TokenType::LESS_EQUAL : TokenType::LESS);
        break;
    case '>':
        addToken(matchCharacter('=') ? TokenType::GREATER_EQUAL : TokenType::GREATER);
        break;
    case '"':
    {
        std::string lexeme = std::string(1, c);
        while (current < source.size() && source[current] != '"')
        {
            lexeme.append(std::string(1, source[current]));
            current++;
        }
        if (current == source.size())
        {
            std::cerr << "[line " << line << "] Error: Unterminated string." << std::endl;
            hasLexicalErrors = true;
        }
        else
        {
            lexeme.append(std::string(1, source[current]));
            current++;
            std::string literal = lexeme.substr(1, lexeme.size() - 2);
            addToken(TokenType::STRING, lexeme, literal);
        }
        break;
    }

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

void Scanner::addToken(TokenType token)
{
    addToken(token, TokenTypeToLexeme(token), "null");
}

void Scanner::addToken(TokenType token, std::string lexeme, std::string literal)
{
    std::cout << TokenTypeToString(token) << " " << lexeme << " " << literal << std::endl;
    tokens.push_back({token, lexeme, literal, line});
}

bool Scanner::matchCharacter(char expected)
{
    if (isAtEnd())
        return false;
    if (source.at(current) != expected)
        return false;

    current++;
    return true;
}