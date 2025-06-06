package com.interpreter.myinterpreter;

import java.util.List;

public class Parser {
    private static class ParseError extends RuntimeException {}

    private final List<Token> tokens;
    private int current = 0;

    Parser(List<Token> tokens){
        this.tokens = tokens;
    }

    public Expr parse() {
        try {
            return expression();
        } catch (ParseError error){
            return null;
        }
    }

    private Expr expression(){
        return equality();
    }

    private Expr equality() {
        Expr expr = comparison();
        while(match(TokenType.BANG_EQUAL, TokenType.EQUAL_EQUAL)) {
            Token op = previous();
            Expr right = comparison();
            expr = new Expr.Binary(expr, op, right);
        }
        
        return expr;
    }

    private Expr comparison() {
        Expr expr = term();
        while (match(TokenType.GREATER, TokenType.GREATER_EQUAL, TokenType.LESS, TokenType.LESS_EQUAL)){
            Token op = previous();
            Expr right = term();
            expr = new Expr.Binary(expr, op, right);
        }

        return expr;
    }

    private Expr term() {
        Expr expr = factor();
        while (match(TokenType.MINUS, TokenType.PLUS)){
            Token op = previous();
            Expr right = factor();
            expr = new Expr.Binary(expr, op, right);
        }
      
        return expr;
    }

    private Expr factor() {
        Expr expr = unary();
        while (match(TokenType.SLASH, TokenType.STAR)){
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

        return primary();
    }

    private Expr primary() {
        if (match(TokenType.TRUE)) return new Expr.Literal(true);
        if (match(TokenType.FALSE)) return new Expr.Literal(false);
        if (match(TokenType.NIL)) return new Expr.Literal(null);
    
        if (match(TokenType.STRING, TokenType.NUMBER)){
            return new Expr.Literal(previous().literal);
        }

        if (match(TokenType.LEFT_PAREN)){
            Expr expr = expression();
            consume(TokenType.RIGHT_PAREN, "Expected ')' after expression.");
            return new Expr.Grouping(expr);
        }

        throw error(peek(), "Expect expression.");
    }

    private Token consume(TokenType type, String message){
        if (check(type)) return advance();

        throw error(peek(), message);
    }

    private ParseError error(Token token, String message){
        Lox.error(token, message);
        
        return new ParseError();
    }

    private void synchronize() {
        advance();

        while(!isAtEnd()){
            if (previous().type == TokenType.SEMICOLON) return;

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
                default: break;
            }

            advance();
        }
    }

    private boolean match(TokenType... tokens){
        for (TokenType token: tokens){
            if (check(token)){
                advance();
                return true;
            }
        }
        return false;
    }

    private Token advance() {
        if (!isAtEnd()) current++;
        return previous();
    }

    private boolean check(TokenType expected){
        if (isAtEnd()) return false;
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
