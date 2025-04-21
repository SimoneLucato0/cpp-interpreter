#include "scanner.h"

#include <iostream>

#include "helper.h"

Scanner::Scanner(const std::string &src) : source(src) {}

std::vector<Token> Scanner::scanTokens() {
    while (!isAtEnd()) {
        start = current;
        scanToken();
    }
    addToken(TokenType::EOF_TOKEN);

    if (hasLexicalErrors) std::exit(65);

    return tokens;
}

void Scanner::scanToken() {
    char c = advance();
    switch (c) {
        case ' ':
        case '\t':
        case '\r':
            break;
        case '\n':
            if (c == '\n') line++;
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
                while (!isAtEnd() && peek() != '\n') advance();
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
        case '"': {
            while (!isAtEnd() && peek() != '"') {
                if (peek() == '\n') line++;
                advance();
            }

            if (isAtEnd()) {
                std::cerr << "[line " << line << "] Error: Unterminated string." << std::endl;
                hasLexicalErrors = true;
                break;
            }

            advance();

            std::string lexeme = source.substr(start, current - start);
            std::string literal = source.substr(start + 1, current - start - 2);
            addToken(TokenType::STRING, lexeme, literal);

            break;
        }

        default:
            if (isDigit(c)) {
                while (isDigit(peek())) advance();

                if (peek() == '.' && isDigit(peekNext())) {
                    advance();

                    while (isDigit(peek())) advance();
                }

                std::string lexeme = source.substr(start, current - start);
                std::string literal = lexeme;

                if (literal.find('.') == std::string::npos) literal.append(".0");
                while (literal.back() == '0' && literal[literal.size() - 2] == '0')
                    literal.pop_back();

                addToken(TokenType::NUMBER, lexeme, literal);

                break;
            }

            if (isCharacter(c)) {
                while (!isAtEnd() && (isCharacter(peek()) || isDigit(peek()))) advance();

                std::string value = source.substr(start, current - start);

                if (reservedKeywordsMap.find(value) != reservedKeywordsMap.end()) {
                    addToken(reservedKeywordsMap.at(value), toLowercase(value), "null");
                } else {
                    addToken(TokenType::IDENTIFIER, value, "null");
                }

                break;
            }

            std::cerr << "[line " << line << "] Error: Unexpected character: " << c << std::endl;
            hasLexicalErrors = true;
            break;
    }
}

char Scanner::advance() { return source.at(current++); }

char Scanner::peek() const {
    if (isAtEnd()) return '\0';
    return source.at(current);
}

char Scanner::peekNext() const {
    if (current + 1 >= source.size()) return '\0';
    return source.at(current + 1);
}

bool Scanner::isAtEnd() const { return current >= source.size(); }

void Scanner::addToken(TokenType token) { addToken(token, TokenTypeToLexeme(token), "null"); }

void Scanner::addToken(TokenType token, std::string lexeme, std::string literal) {
    std::cout << TokenTypeToString(token) << " " << lexeme << " " << literal << std::endl;
    tokens.push_back({token, lexeme, literal, line});
}

bool Scanner::matchCharacter(char expected) {
    if (isAtEnd()) return false;
    if (source.at(current) != expected) return false;

    current++;
    return true;
}