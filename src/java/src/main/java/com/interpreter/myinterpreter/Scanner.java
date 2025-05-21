package com.interpreter.myinterpreter;

import static java.util.Map.entry;

import java.util.ArrayList;
import java.util.Map;

public class Scanner {
	private final String source;
	private int start = 0, current = 0, line = 1;
	private ArrayList<Token> tokens = new ArrayList<>();
    private boolean hasSyntaxErrors = false;	

	private Map<String, TokenType> reservedKeywordsMap = Map.ofEntries(
        entry("and", TokenType.AND),     
		entry("class", TokenType.CLASS),   
		entry("else", TokenType.ELSE),
        entry("false", TokenType.FALSE), 
		entry("for", TokenType.FOR),       
		entry("fun", TokenType.FUN),
        entry("if", TokenType.IF),       
		entry("nil", TokenType.NIL),       
		entry("or", TokenType.OR),
        entry("print", TokenType.PRINT), 
		entry("return", TokenType.RETURN), 
		entry("super", TokenType.SUPER),
        entry("this", TokenType.THIS),   
		entry("true", TokenType.TRUE),     
		entry("var", TokenType.VAR),
        entry("while", TokenType.WHILE)
    );

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
					handleNumber();
 					break;
				}

				if (isCharacter(c)){
					while(!isAtEnd() && (isCharacter(peek()) || isDigit(peek()))) advance();

					String value = source.substring(start, current);
					if (reservedKeywordsMap.containsKey(value)){
						addToken(reservedKeywordsMap.get(value), value.toLowerCase(), "null");
					} else {
						addToken(TokenType.IDENTIFIER, value, "null");
					}

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

	private boolean isCharacter(char c) {
		return Character.isLetter(c) || c == '_';
	}

	private void handleNumber() {
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
