#include <iostream>

#include "astPrinter.h"
#include "expr.h"

int main() {
    Binary* expression = new Binary(
        std::unique_ptr<Unary>(new Unary(Token(TokenType::MINUS, "-", "null", 1),
                                         std::unique_ptr<Literal>(new Literal("123")))),
        Token(TokenType::STAR, "*", "null", 1),
        std::unique_ptr<Grouping>(new Grouping(std::unique_ptr<Literal>(new Literal("45.67")))));

    AstPrinter* p = new AstPrinter();
    std::cout << p->print(*expression);

    return 0;
}