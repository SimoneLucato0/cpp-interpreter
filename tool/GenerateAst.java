import java.io.IOException;
import java.io.PrintWriter;
import java.nio.file.Path;
import java.nio.file.Paths;
import java.util.Arrays;
import java.util.List;

public class GenerateAst {
    public static void main(String[] args) throws IOException {
        if (args.length < 1) {
            System.err.println("Usage: generate_ast <output directory>");
            System.exit(64);
        }

        String outputDir = args[0];
        defineAst(outputDir, "Expr", Arrays.asList(
                "Assign   : Token name, Expr value",
                "Binary   : Expr left, Token operator, Expr right",
                "Call     : Expr callee, Token paren, List<Expr> arguments",
                "Grouping : Expr expression",
                "Literal  : Object value",
                "Logical  : Expr left, Token operator, Expr right",
                "Unary    : Token operator, Expr right",
                "Variable : Token name"));

        defineAst(outputDir, "Stmt", Arrays.asList(
                "Block      : List<Stmt> statements",
                "Expression : Expr expression",
                "Function   : Token name, List<Token> params, List<Stmt> body",
                "If         : Expr condition, Stmt thenBranch, Stmt elseBranch",
                "Print      : Expr expression",
                "Return     : Token keyword, Expr value",
                "Var        : Token name, Expr initializer",
                "While      : Expr condition, Stmt body"));
    }

    private static void defineAst(String outputDir, String baseName, List<String> types) throws IOException {
        Path path = Paths.get(outputDir, baseName + ".java");
        PrintWriter writer = new PrintWriter(path.toString(), "UTF-8");

        writer.println("package com.interpreter.myinterpreter;");
        writer.println();
        writer.println("// file generated running:");
        writer.println("// `javac ./tool/GenerateAst.java`");
        writer.println("// `java -cp tool GenerateAst ./src/java/src/main/java/com/interpreter/myinterpreter`");
        writer.println();
        writer.println("import java.util.List;");
        writer.println();
        writer.println(String.format("abstract class %s {", baseName));

        defineVisitor(writer, baseName, types);
        writer.println();

        for (String type : types) {
            String className = type.split(":")[0].trim();
            String fields = type.split(":")[1].trim();
            defineType(writer, baseName, className, fields);
            writer.println();
        }

        writer.println();
        writer.println("    abstract <R> R accept(Visitor<R> visitor);");

        writer.println("}");
        writer.close();
    }

    private static void defineType(PrintWriter writer, String baseName, String className, String fieldsList) {
        writer.println(String.format("    static class %s extends %s {", className, baseName));

        // ** fields
        String[] fields = fieldsList.split(", ");
        for (String field : fields) {
            writer.println(String.format("        final %s;", field));
        }
        writer.println();

        // ** constructor
        writer.println(String.format("        %s (%s) {", className, fieldsList));
        for (String field : fields) {
            String name = field.split(" ")[1];
            writer.println(String.format("            this.%s = %s;", name, name));
        }
        writer.println("        }");

        // ** visitor
        writer.println();
        writer.println("        @Override");
        writer.println("        <R> R accept(Visitor<R> visitor){");
        writer.println(String.format("            return visitor.visit%s%s(this);", className, baseName));
        writer.println("        }");

        writer.println("    }");
    }

    private static void defineVisitor(PrintWriter writer, String baseName, List<String> types) {
        writer.println("    interface Visitor<R> {");

        for (String type : types) {
            String typeName = type.split(":")[0].trim();
            writer.println(
                    String.format("        R visit%s%s(%s %s);", typeName, baseName, typeName, baseName.toLowerCase()));
        }
        writer.println("    }");
    }
}
