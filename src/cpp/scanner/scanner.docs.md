# Scanner

The `Scanner` class takes a `const std::string&` source (read from a file) and allows to scan the tokens defined inside with the `std::vector<Token> scanTokens` function.

Tokens are defined as follows:

```
struct Token {
    TokenType type;
    std::string lexeme;
    std::string literal;
    int line;
};
```

The tokens that can be parsed are the following:

- single character, e.g. parenthesis, comma, dot, semicolons and arithmetic operations;
- relationship operations, e.g. less/less or equal, greater/greater or equal, not/not equal, equality and deep equality (`==`);
- single line comments (defined with `//`);
- strings (defined between double quote `""`);
- numbers (defaulted to a double with the minimum number of significant digits)
- reserved keywords (`and`, `class`, `else`, `false`, `for`, `fun`, `if`, `nil`, `or`, `print`, `return`, `super`, `this`, `true`, `var`, `while`);
- identifiers (defined starting with an underscore or a letter).

Currently the tokens are just printed to standard output and the errors printed to standard error.
