#pragma once

#include <string>
#include <memory>

#include "../src/token.h"

class Binary;
class Grouping;
class Literal;
class Unary;

class ExprVisitorBase {
public:
    virtual void visitBinary(const Binary& binary) = 0;
    virtual void visitGrouping(const Grouping& grouping) = 0;
    virtual void visitLiteral(const Literal& literal) = 0;
    virtual void visitUnary(const Unary& unary) = 0;
    virtual ~ExprVisitorBase() = default;
};


template <typename R>
class ExprVisitor : public ExprVisitorBase {
public:
    R result;
};

class Expr {
public:
    virtual void acceptHelper(ExprVisitorBase& visitor) const = 0;
    virtual ~Expr() = default;

    template <typename R>
    R accept(ExprVisitor<R>& visitor) const {
        acceptHelper(visitor);
        return visitor.result;
    }
};

class Binary: public Expr {
public:
    std::unique_ptr<Expr> left;
    Token operation;
    std::unique_ptr<Expr> right;

    Binary(std::unique_ptr<Expr> left, Token operation, std::unique_ptr<Expr> right) : left(std::move(left)), operation(operation), right(std::move(right)){}
    void acceptHelper(ExprVisitorBase& visitor) const override {
        visitor.visitBinary(*this);
    }
};

class Grouping: public Expr {
public:
    std::unique_ptr<Expr> expression;

    Grouping(std::unique_ptr<Expr> expression) : expression(std::move(expression)){}
    void acceptHelper(ExprVisitorBase& visitor) const override {
        visitor.visitGrouping(*this);
    }
};

class Literal: public Expr {
public:
    std::string value;

    Literal(std::string value) : value(value){}
    void acceptHelper(ExprVisitorBase& visitor) const override {
        visitor.visitLiteral(*this);
    }
};

class Unary: public Expr {
public:
    Token operation;
    std::unique_ptr<Expr> right;

    Unary(Token operation, std::unique_ptr<Expr> right) : operation(operation), right(std::move(right)){}
    void acceptHelper(ExprVisitorBase& visitor) const override {
        visitor.visitUnary(*this);
    }
};

