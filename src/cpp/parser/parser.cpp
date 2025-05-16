#include "parser.hpp"

#include "../error.hpp"

// ** Expression is basically just the equality rule
std::unique_ptr<Expr> Parser::expression() { return equality(); }

// ** equality       → comparison ( ( "!=" | "==" ) comparison )* ;
std::unique_ptr<Expr> Parser::equality() {
    std::unique_ptr<Expr> expr = comparison();

    while (match({TokenType::BANG_EQUAL, TokenType::EQUAL_EQUAL})) {
        Token op = previous();
        std::unique_ptr<Expr> right = comparison();
        expr = std::make_unique<Binary>(std::move(expr), op, std::move(right));
    }

    return expr;
}

// ** comparison     → term ( ( ">" | ">=" | "<" | "<=" ) term )* ;
std::unique_ptr<Expr> Parser::comparison() {
    std::unique_ptr<Expr> expr = term();

    while (match(
        {TokenType::GREATER, TokenType::GREATER_EQUAL, TokenType::LESS, TokenType::LESS_EQUAL})) {
        Token op = previous();
        std::unique_ptr<Expr> right = term();
        expr = std::make_unique<Binary>(std::move(expr), op, std::move(right));
    }

    return expr;
}

// ** term           → factor ( ( "-" | "+" ) factor )* ;
std::unique_ptr<Expr> Parser::term() {
    std::unique_ptr<Expr> expr = factor();

    while (match({TokenType::MINUS, TokenType::PLUS})) {
        Token op = previous();
        std::unique_ptr<Expr> right = term();
        expr = std::make_unique<Binary>(std::move(expr), op, std::move(right));
    }

    return expr;
}

// ** factor         → unary ( ( "/" | "*" ) unary )* ;
std::unique_ptr<Expr> Parser::factor() {
    std::unique_ptr<Expr> expr = unary();

    while (match({TokenType::SLASH, TokenType::STAR})) {
        Token op = previous();
        std::unique_ptr<Expr> right = unary();
        expr = std::make_unique<Binary>(std::move(expr), op, std::move(right));
    }

    return expr;
}

// ** unary          → ( "!" | "-" ) unary
// **                | primary ;
std::unique_ptr<Expr> Parser::unary() {
    if (match({TokenType::BANG, TokenType::MINUS})) {
        Token op = previous();
        std::unique_ptr<Expr> right = unary();
        return std::make_unique<Unary>(op, std::move(right));
    }

    return primary();
}

// ** primary        → NUMBER | STRING | "true" | "false" | "nil"
// **               | "(" expression ")" ;
std::unique_ptr<Expr> Parser::primary() {
    if (match({TokenType::TRUE})) return std::make_unique<Literal>("true");
    if (match({TokenType::FALSE})) return std::make_unique<Literal>("false");
    if (match({TokenType::NIL})) return std::make_unique<Literal>("null");

    if (match({TokenType::NUMBER, TokenType::STRING})) {
        return std::make_unique<Literal>(previous().literal);
    }

    if (match({TokenType::LEFT_PAREN})) {
        std::unique_ptr<Expr> expr = expression();
        consume(TokenType::RIGHT_PAREN, "Expected ')' after expression");
        return std::make_unique<Grouping>(std::move(expr));
    }

    throw error(peek(), "Expected expression");
}

bool Parser::match(std::initializer_list<TokenType> tokens) {
    for (TokenType type : tokens) {
        if (check(type)) {
            advance();
            return true;
        }
    }
    return false;
}

bool Parser::check(TokenType type) {
    if (isAtEnd()) return false;
    return peek().type == type;
}

Token Parser::advance() {
    if (!isAtEnd()) current++;
    return previous();
}

Token Parser::previous() const { return tokens.at(current - 1); }

Token Parser::peek() const { return tokens.at(current); }

bool Parser::isAtEnd() const { return peek().type == TokenType::EOF_TOKEN; }

Token Parser::consume(TokenType type, const std::string& message) {
    if (check(type)) return advance();

    hadError = true;
    throw error(peek(), message);
}

Parser::ParseError Parser::error(Token token, const std::string& message) {
    Error::output(token, message);
    return Parser::ParseError(message);
}

void Parser::synchronize() {
    advance();

    while (!isAtEnd()) {
        if (previous().type == TokenType::SEMICOLON) return;

        switch (peek().type) {
            case TokenType::CLASS:
            case TokenType::FOR:
            case TokenType::FUN:
            case TokenType::IF:
            case TokenType::PRINT:
            case TokenType::RETURN:
            case TokenType::VAR:
            case TokenType::WHILE:
                return;

            default:
                continue;
        }
        advance();
    }
}

std::unique_ptr<Expr> Parser::parse() {
    try {
        return expression();
    } catch (const ParseError& error) {
        return nullptr;
    }
}