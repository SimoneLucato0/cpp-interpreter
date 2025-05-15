# Parser

The grammatic used by Lox is the following: (source is [Chapter 6 of Crafting Interpreters](https://craftinginterpreters.com/parsing-expressions.html))

```
expression     → equality ;
equality       → comparison ( ( "!=" | "==" ) comparison )* ;
comparison     → term ( ( ">" | ">=" | "<" | "<=" ) term )* ;
term           → factor ( ( "-" | "+" ) factor )* ;
factor         → unary ( ( "/" | "*" ) unary )* ;
unary          → ( "!" | "-" ) unary
               | primary ;
primary        → NUMBER | STRING | "true" | "false" | "nil"
               | "(" expression ")" ;
```

The parsing algorithm that will be used is the **recursive descent**: basically it's a top-down parser (starting from "expression" downwards) that more or less translates each grammar rule into some code.

## Error handling

Basically everytime our parser notices an error, it uses **synchronization** to try to get back to a correct state: the idea is we select some grammar rules as synchronization points and the parser jumps out of any rule until it reaches these checkpoints. Any token met before reaching the ones of a checkpoint rule are discarded.

When we want to synchronize we throw a `ParseError` object, that is caught in the method for the grammar rule we are synchronizing to. After catching the error, the parser is in a right state, but the tokens are not. Therefore, we discard all the tokens until the beginning of the next statement. Usually a statement end with a semicolon and begins with a keyword, so if we-ve matched with a semicolon and the next token is a keyword, we _should_ be good (theoretically one could match semicolons inside a for statement, but for now it's all good).
