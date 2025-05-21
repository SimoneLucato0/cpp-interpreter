package com.interpreter.myinterpreter;

import java.util.ArrayList;

public class Scanner {
	private final String source;
	private int start = 0, current = 0, line = 1;
	private ArrayList<Token> tokens = new ArrayList<>();
       	private boolean hasSyntaxErrors = false;	

	Scanner(String source) {
		this.source = source;
	}

	public ArrayList<Token> scanTokens() {
		while(!isAtEnd()){
			start = current;
			scanToken();
		}	
		addToken(TokenType.EOF_TOKEN);

		if(hasSyntaxErrors) System.exit(65);

		return tokens;
	}

	private void scanToken() {
		char c = advance();
		switch(c) {
			case ' ':
			case '\r':
			case '\t':
				break;
			case '\n':
				line++;
				return;
			case '(':
				addToken(TokenType.LEFT_PAREN);
				break;
			case ')':
				addToken(TokenType.RIGHT_PAREN);
				break;
			case '{':
				addToken(TokenType.LEFT_BRACE);
				break;
			case '}':
				addToken(TokenType.RIGHT_BRACE);
				break;
			case ',':
            			addToken(TokenType.COMMA);
            			break;
       			case '.':
            			addToken(TokenType.DOT);
            			break;
        		case ';':
            			addToken(TokenType.SEMICOLON);
            			break;
        		case '+':
           			addToken(TokenType.PLUS);
            			break;
        		case '-':
            			addToken(TokenType.MINUS);
            			break;
        		case '*':
            			addToken(TokenType.STAR);
            			break;
			case '>':
				addToken(matchCharacter('=') ? TokenType.GREATER_EQUAL : TokenType.GREATER);
				break;
			case '<':
				addToken(matchCharacter('=') ? TokenType.LESS_EQUAL : TokenType.LESS);
				break;
			case '!':
				addToken(matchCharacter('=') ? TokenType.BANG_EQUAL : TokenType.BANG);
				break;
			case '=':
				addToken(matchCharacter('=') ? TokenType.EQUAL_EQUAL : TokenType.EQUAL);
				break;
			case '/':
				if (matchCharacter('/')){
					while (!isAtEnd() && source.charAt(current) != '\n') current++;
				} else {
					addToken(TokenType.SLASH);
				}
				break;
			case '"': {
				while (!isAtEnd() && peek() != '"') {
					if (peek() == '\n') line++;
					advance();
				}

				if (isAtEnd()) {
					reportError("Unterminated string.");
					break;
				}
				
				advance();
				
				String lexeme = source.substring(start, current);
				String literal = source.substring(start + 1, current - 1);

				addToken(TokenType.STRING, lexeme, literal);
				
				break;
			}	
			default: { 
				if (isDigit(c)) {
					while (!isAtEnd() && isDigit(peek())) advance();
	
					if (peek() == '.' && isDigit(peekNext())) {
						advance();
		
						while (isDigit(peek())) advance();
					}

					String lexeme = source.substring(start, current);
					String literal = lexeme;	

					if (!literal.contains(".")) literal += ".0";
					while (literal.endsWith("0") && literal.charAt(literal.length() - 2) == '0') {
						literal = literal.substring(0, literal.length() - 1);
					}

					addToken(TokenType.NUMBER, lexeme, literal);
 					break;
				}

				String errorMessage = "Unexpected character: " + c;
				reportError(errorMessage);
			}	
		}
	}

	private char advance() {
		return source.charAt(current++);
	}

	private boolean matchCharacter(char expected) {
		if (isAtEnd()) return false;
		if (source.charAt(current) != expected) return false;

		current++;
		return true;
	}

	private boolean isAtEnd() {
		return current >= source.length();
	}

	private boolean isDigit(char c) {
		return Character.isDigit(c);
	}

	private void handleNumber() {
 	
	}

	private char peek() {
		if (isAtEnd()) return '\0';
		return source.charAt(current);
	}	

	private char peekNext() {
		if (current >= source.length() - 1) return '\0';
		return source.charAt(current + 1);
	}

	private void addToken(TokenType type) {
		addToken(type, type.toString(), "null");
	}

	private void addToken(TokenType type, String lexeme, String literal) {
		System.out.printf("%s %s %s\n", type, lexeme, literal);
		tokens.add(new Token(type, lexeme, literal, line));
	}

	private void reportError(String message) {
		System.err.printf("Error [line %d]: %s\n", line, message);
		hasSyntaxErrors = true;
	}
}
