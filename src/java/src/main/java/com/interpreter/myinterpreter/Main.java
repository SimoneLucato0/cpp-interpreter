package com.interpreter.myinterpreter;

import java.nio.file.Files;
import java.nio.file.Paths;
import java.io.IOException;

public class Main {
    public static void main(String[] args) {
    	if (args.length < 1){
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
	scanner.scanTokens();
    }
}
