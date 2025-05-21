package com.interpreter.myinterpreter;

public class Token {
	public final TokenType type;
	public final String lexeme;
	public final String literal;
	public final int line;

	public Token(TokenType type, String lexeme, String literal, int line) {
		this.type = type;
		this.lexeme = lexeme;
		this.literal = literal;
		this.line = line;
	}

	@Override
	public String toString() {
		return "Token={type=" + type + ", lexeme=" + lexeme + ",literal=" + literal + "}";
	}
}
