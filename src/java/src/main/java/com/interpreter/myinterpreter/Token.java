package com.interpreter.myinterpreter;

public class Token {
	public TokenType type;
	public String lexeme;
	public String literal;
	public int line;

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
