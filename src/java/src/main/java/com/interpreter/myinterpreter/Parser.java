package com.interpreter.myinterpreter;

import java.util.ArrayList;
import java.util.Arrays;
import java.util.List;

public class Parser {
    private static class ParseError extends RuntimeException {
    }

    private final List<Token> tokens;
    private int current = 0;

    Parser(List<Token> tokens) {
        this.tokens = tokens;
    }

    public List<Stmt> parse() {
        List<Stmt> statements = new ArrayList<>();
        while (!isAtEnd()) {
            statements.add(declaration());
        }

        return statements;
    }

    private Stmt declaration() {
        try {
            if (match(TokenType.FUN))
                return function("function");
            if (match(TokenType.VAR))
                return varDeclaration();

            return statement();
        } catch (ParseError error) {
            synchronize();
            return null;
        }
    }

    private Stmt function(String kind) {
        Token name = consume(TokenType.IDENTIFIER, "Expected " + kind + " name.");

        consume(TokenType.LEFT_PAREN, "Expected '(' after " + kind + " name.");
        List<Token> params = new ArrayList<>();
        if (!check(TokenType.RIGHT_PAREN)) {
            do {
                if (params.size() > 255) {
                    throw new RuntimeError(peek(), "Can't have more than 255 parameters.");
                }

                params.add(consume(TokenType.IDENTIFIER, "Expect parameter name."));
            } while (match(TokenType.COMMA));
        }
        consume(TokenType.RIGHT_PAREN, "Expected ')' after parameters.");

        consume(TokenType.LEFT_BRACE, "Expected '{' before " + kind + " body.");
        List<Stmt> body = block();
        return new Stmt.Function(name, params, body);
    }

    private Stmt varDeclaration() {
        Token name = consume(TokenType.IDENTIFIER, "Expect variable name.");

        Expr initializer = null;
        if (match(TokenType.EQUAL)) {
            initializer = expression();
        }

        consume(TokenType.SEMICOLON, "Expect ';' after variable declaration");
        return new Stmt.Var(name, initializer);
    }

    private Stmt statement() {
        if (match(TokenType.FOR))
            return forStatement();
        if (match(TokenType.IF))
            return ifStatement();
        if (match(TokenType.PRINT))
            return printStatement();
        if (match(TokenType.RETURN))
            return returnStatement();
        if (match(TokenType.WHILE))
            return whileStatement();
        if (match(TokenType.LEFT_BRACE))
            return new Stmt.Block(block());

        return expressionStatement();
    }

    private Stmt forStatement() {
        consume(TokenType.LEFT_PAREN, "Expected '(' after 'for'.");

        Stmt initializer;
        if (match(TokenType.SEMICOLON))
            initializer = null;
        else if (match(TokenType.VAR))
            initializer = varDeclaration();
        else
            initializer = expressionStatement();

        Expr condition = null;
        if (!check(TokenType.SEMICOLON))
            condition = expression();
        consume(TokenType.SEMICOLON, "Expected ';' after loop condition.");

        Expr increment = null;
        if (!check(TokenType.RIGHT_PAREN))
            increment = expression();
        consume(TokenType.RIGHT_PAREN, "Expected ')' after for clauses.");

        Stmt body = statement();
        if (increment != null) {
            body = new Stmt.Block(Arrays.asList(body, new Stmt.Expression(increment)));
        }

        if (condition == null)
            condition = new Expr.Literal(true);
        body = new Stmt.While(condition, body);

        if (initializer != null) {
            body = new Stmt.Block(Arrays.asList(initializer, body));
        }

        return body;
    }

    private Stmt ifStatement() {
        consume(TokenType.LEFT_PAREN, "Expected '(' after 'if'.");
        Expr condition = expression();
        consume(TokenType.RIGHT_PAREN, "Expected ')' after if condition.");

        Stmt thenBranch = statement();
        Stmt elseBranch = null;
        if (match(TokenType.ELSE))
            elseBranch = statement();

        return new Stmt.If(condition, thenBranch, elseBranch);
    }

    private Stmt printStatement() {
        Expr value = expression();
        consume(TokenType.SEMICOLON, "Expect ';' after value.");
        return new Stmt.Print(value);
    }

    private Stmt returnStatement() {
        Token keyword = previous();
        Expr value = null;
        if (!check(TokenType.SEMICOLON))
            value = expression();

        consume(TokenType.SEMICOLON, "Expected ';' after return value.");
        return new Stmt.Return(keyword, value);
    }

    private Stmt whileStatement() {
        consume(TokenType.LEFT_PAREN, "Expected '(' after 'while'.");
        Expr condition = expression();
        consume(TokenType.RIGHT_PAREN, "Expected ')' after condition.");
        Stmt body = statement();

        return new Stmt.While(condition, body);
    }

    private Stmt expressionStatement() {
        Expr value = expression();
        consume(TokenType.SEMICOLON, "Expect ';' after value.");
        return new Stmt.Expression(value);
    }

    private List<Stmt> block() {
        List<Stmt> statements = new ArrayList<>();

        while (!check(TokenType.RIGHT_BRACE) && !isAtEnd()) {
            statements.add(declaration());
        }

        consume(TokenType.RIGHT_BRACE, "Expect '}' after block.");
        return statements;
    }

    private Expr expression() {
        return assignment();
    }

    private Expr assignment() {
        Expr expr = or();

        if (match(TokenType.EQUAL)) {
            Token equal = previous();
            Expr value = assignment();

            if (expr instanceof Expr.Variable) {
                Token name = ((Expr.Variable) expr).name;
                return new Expr.Assign(name, value);
            }

            error(equal, "Invalid assignment target.");
        }

        return expr;
    }

    private Expr or() {
        Expr expr = and();

        while (match(TokenType.OR)) {
            Token op = previous();
            Expr right = and();
            expr = new Expr.Logical(expr, op, right);
        }

        return expr;
    }

    private Expr and() {
        Expr expr = equality();

        while (match(TokenType.AND)) {
            Token op = previous();
            Expr right = equality();
            expr = new Expr.Logical(expr, op, right);
        }

        return expr;
    }

    private Expr equality() {
        Expr expr = comparison();
        while (match(TokenType.BANG_EQUAL, TokenType.EQUAL_EQUAL)) {
            Token op = previous();
            Expr right = comparison();
            expr = new Expr.Binary(expr, op, right);
        }

        return expr;
    }

    private Expr comparison() {
        Expr expr = term();
        while (match(TokenType.GREATER, TokenType.GREATER_EQUAL, TokenType.LESS, TokenType.LESS_EQUAL)) {
            Token op = previous();
            Expr right = term();
            expr = new Expr.Binary(expr, op, right);
        }

        return expr;
    }

    private Expr term() {
        Expr expr = factor();
        while (match(TokenType.MINUS, TokenType.PLUS)) {
            Token op = previous();
            Expr right = factor();
            expr = new Expr.Binary(expr, op, right);
        }

        return expr;
    }

    private Expr factor() {
        Expr expr = unary();
        while (match(TokenType.SLASH, TokenType.STAR)) {
            Token op = previous();
            Expr right = unary();
            expr = new Expr.Binary(expr, op, right);
        }

        return expr;
    }

    private Expr unary() {
        if (match(TokenType.BANG, TokenType.MINUS)) {
            Token op = previous();
            Expr right = unary();
            return new Expr.Unary(op, right);
        }

        return call();
    }

    private Expr call() {
        Expr expr = primary();

        while (true) {
            if (match(TokenType.LEFT_PAREN)) {
                expr = finishCall(expr);
            } else {
                break;
            }
        }
        return expr;
    }

    private Expr finishCall(Expr callee) {
        List<Expr> arguments = new ArrayList<>();
        if (!check(TokenType.RIGHT_PAREN)) {
            do {
                if (arguments.size() > 255) {
                    error(peek(), "Can't have more than 255 arguments.");
                }
                arguments.add(expression());
            } while (match(TokenType.COMMA));
        }

        Token paren = consume(TokenType.RIGHT_PAREN, "Expected ')' after arguments.");

        return new Expr.Call(callee, paren, arguments);
    }

    private Expr primary() {
        if (match(TokenType.TRUE))
            return new Expr.Literal(true);
        if (match(TokenType.FALSE))
            return new Expr.Literal(false);
        if (match(TokenType.NIL))
            return new Expr.Literal(null);

        if (match(TokenType.STRING, TokenType.NUMBER)) {
            return new Expr.Literal(previous().literal);
        }

        if (match(TokenType.IDENTIFIER)) {
            return new Expr.Variable(previous());
        }

        if (match(TokenType.LEFT_PAREN)) {
            Expr expr = expression();
            consume(TokenType.RIGHT_PAREN, "Expected ')' after expression.");
            return new Expr.Grouping(expr);
        }

        throw error(peek(), "Expect expression.");
    }

    private Token consume(TokenType type, String message) {
        if (check(type))
            return advance();

        throw error(peek(), message);
    }

    private ParseError error(Token token, String message) {
        Lox.error(token, message);

        return new ParseError();
    }

    private void synchronize() {
        advance();

        while (!isAtEnd()) {
            if (previous().type == TokenType.SEMICOLON)
                return;

            switch (peek().type) {
                case CLASS:
                case FOR:
                case FUN:
                case IF:
                case PRINT:
                case RETURN:
                case VAR:
                case WHILE:
                    return;
                default:
                    break;
            }

            advance();
        }
    }

    private boolean match(TokenType... tokens) {
        for (TokenType token : tokens) {
            if (check(token)) {
                advance();
                return true;
            }
        }
        return false;
    }

    private Token advance() {
        if (!isAtEnd())
            current++;
        return previous();
    }

    private boolean check(TokenType expected) {
        if (isAtEnd())
            return false;
        return peek().type == expected;
    }

    private boolean isAtEnd() {
        return peek().type == TokenType.EOF_TOKEN;
    }

    private Token peek() {
        return tokens.get(current);
    }

    private Token previous() {
        return tokens.get(current - 1);
    }
}
