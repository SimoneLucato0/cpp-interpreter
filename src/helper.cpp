#include "helper.hpp"

#include <algorithm>
#include <cctype>
#include <stdexcept>
#include <string>

std::string TokenTypeToString(TokenType t) {
    switch (t) {
        case TokenType::LEFT_PAREN:
            return "LEFT_PAREN";
        case TokenType::RIGHT_PAREN:
            return "RIGHT_PAREN";
        case TokenType::LEFT_BRACE:
            return "LEFT_BRACE";
        case TokenType::RIGHT_BRACE:
            return "RIGHT_BRACE";
        case TokenType::COMMA:
            return "COMMA";
        case TokenType::DOT:
            return "DOT";
        case TokenType::SEMICOLON:
            return "SEMICOLON";
        case TokenType::PLUS:
            return "PLUS";
        case TokenType::MINUS:
            return "MINUS";
        case TokenType::STAR:
            return "STAR";
        case TokenType::SLASH:
            return "SLASH";

        case TokenType::EQUAL:
            return "EQUAL";
        case TokenType::EQUAL_EQUAL:
            return "EQUAL_EQUAL";
        case TokenType::BANG:
            return "BANG";
        case TokenType::BANG_EQUAL:
            return "BANG_EQUAL";
        case TokenType::LESS:
            return "LESS";
        case TokenType::LESS_EQUAL:
            return "LESS_EQUAL";
        case TokenType::GREATER:
            return "GREATER";
        case TokenType::GREATER_EQUAL:
            return "GREATER_EQUAL";

        case TokenType::STRING:
            return "STRING";
        case TokenType::NUMBER:
            return "NUMBER";
        case TokenType::IDENTIFIER:
            return "IDENTIFIER";

        case TokenType::AND:
            return "AND";
        case TokenType::CLASS:
            return "CLASS";
        case TokenType::ELSE:
            return "ELSE";
        case TokenType::FALSE:
            return "FALSE";
        case TokenType::FOR:
            return "FOR";
        case TokenType::FUN:
            return "FUN";
        case TokenType::IF:
            return "IF";
        case TokenType::NIL:
            return "NIL";
        case TokenType::OR:
            return "OR";
        case TokenType::PRINT:
            return "PRINT";
        case TokenType::RETURN:
            return "RETURN";
        case TokenType::SUPER:
            return "SUPER";
        case TokenType::THIS:
            return "THIS";
        case TokenType::TRUE:
            return "TRUE";
        case TokenType::VAR:
            return "VAR";
        case TokenType::WHILE:
            return "WHILE";

        case TokenType::EOF_TOKEN:
            return "EOF";

        default:
            throw std::invalid_argument("TokenTypeToString: Unimplemented item");
    }
}

std::string TokenTypeToLexeme(TokenType t) {
    switch (t) {
        case TokenType::LEFT_PAREN:
            return "(";
        case TokenType::RIGHT_PAREN:
            return ")";
        case TokenType::LEFT_BRACE:
            return "{";
        case TokenType::RIGHT_BRACE:
            return "}";
        case TokenType::COMMA:
            return ",";
        case TokenType::DOT:
            return ".";
        case TokenType::SEMICOLON:
            return ";";
        case TokenType::PLUS:
            return "+";
        case TokenType::MINUS:
            return "-";
        case TokenType::STAR:
            return "*";
        case TokenType::SLASH:
            return "/";

        case TokenType::EQUAL:
            return "=";
        case TokenType::EQUAL_EQUAL:
            return "==";
        case TokenType::BANG:
            return "!";
        case TokenType::BANG_EQUAL:
            return "!=";
        case TokenType::LESS:
            return "<";
        case TokenType::LESS_EQUAL:
            return "<=";
        case TokenType::GREATER:
            return ">";
        case TokenType::GREATER_EQUAL:
            return ">=";

        case TokenType::EOF_TOKEN:
            return "";

        default:
            throw std::invalid_argument("TokenTypeToLexeme:Unimplemented item");
    }
}

bool isDigit(char c) { return c >= '0' && c <= '9'; }

bool isCharacter(char c) { return (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || c == '_'; }

std::string toLowercase(const std::string &s) {
    std::string copy;
    copy.reserve(s.size());

    std::transform(s.begin(), s.end(), std::back_inserter(copy),
                   [](unsigned char c) { return std::tolower(c); });

    return copy;
}