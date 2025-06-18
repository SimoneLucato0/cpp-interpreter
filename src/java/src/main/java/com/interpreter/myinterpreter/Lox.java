package com.interpreter.myinterpreter;

import java.io.IOException;
import java.nio.file.Files;
import java.nio.file.Paths;
import java.util.List;

public class Lox {
	private static Interpreter intepreter = new Interpreter();

	static boolean hadError = false;
	static boolean hadRuntimeError = false;

	public static void main(String[] args) {
		if (args.length < 1) {
			System.err.println("Usage: java Main <file-path>");
			return;
		}

		String filePath = args[0];
		String source = "";

		try {
			source = Files.readString(Paths.get(filePath));
		} catch (IOException e) {
			System.err.println("Error reading the file: " + e.getMessage());
			return;
		}

		if (source == "") {
			System.err.println("Source is empty.");
			return;
		}

		Scanner scanner = new Scanner(source);
		List<Token> tokens = scanner.scanTokens();

		Parser parser = new Parser(tokens);
		List<Stmt> statements = parser.parse();

		if (hadError)
			return;
		if (hadRuntimeError)
			return;

		Resolver resolver = new Resolver(intepreter);
		resolver.resolve(statements);
		if (hadError)
			return;

		intepreter.interpret(statements);
	}

	static void error(Token token, String message) {
		if (token.type == TokenType.EOF_TOKEN) {
			report(token.line, " at end", message);
		} else {
			report(token.line, " at '" + token.lexeme + "'", message);
		}
	}

	static void error(int line, String message) {
		report(line, message);
	}

	static void runtimeError(RuntimeError error) {
		System.err.println(error.getMessage() + "\n[line " + error.token.line + "]");
		hadRuntimeError = true;
	}

	private static void report(int line, String... messages) {
		hadError = true;
		System.err.println(String.format("[line %d] %s", line, String.join(" ", messages)));
	}
}
