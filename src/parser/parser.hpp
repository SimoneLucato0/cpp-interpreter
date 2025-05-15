#pragma once

#include <memory>
#include <vector>

#include "../ast/expr.hpp"
#include "../token.hpp"

class Parser {
   private:
    struct ParseError : public std::runtime_error {
        ParseError(const std::string& msg) : std::runtime_error(msg) {}
    };

    std::vector<Token> tokens;
    size_t current = 0;

    std::unique_ptr<Expr> expression();
    std::unique_ptr<Expr> equality();
    std::unique_ptr<Expr> comparison();
    std::unique_ptr<Expr> term();
    std::unique_ptr<Expr> factor();
    std::unique_ptr<Expr> unary();
    std::unique_ptr<Expr> primary();

    bool match(std::initializer_list<TokenType> tokens);
    bool check(TokenType type);
    Token advance();
    Token consume(TokenType type, const std::string& message);

    ParseError error(Token token, const std::string& message);
    void synchronize();

    Token previous() const;
    Token peek() const;
    bool isAtEnd() const;

   public:
    bool hadError = false;

    Parser(std::vector<Token> tokens) : tokens(tokens) {}
    std::unique_ptr<Expr> parse();
};