# Evaluating expressions

While the end user of a language sees only the types defined in the language (in this case Lox), it is the implementation of the interpreter (for an interpreted language) what truly matters. Therefore, as of now, there must be a mapping between Lox types and Java types. Considering that Lox is dynamically typed, we need to use `java.lang.Object` as a form of representing any object and then dialing it down using the `instanceof` operator.

As for the other defined types, we can use the following mapping:
| Lox Type | Java Type |
| ----------------- | --------- |
| Any Lox value | Object |
| `nil` | `null` |
| Boolean | Boolean |
| number | Double |
| string | String |

The interpreter is doing **post-order traversal** as it evaluates its operand subexpression first, before evaluating its operator itself.

The truthiness check is quite simple: if a value is _false_ or _nil_ it's false; otherwise it's true. Equality check is quite straightforward as well: either they are both _null_ or they are equal based on the overloading of their type (e.g. for `Double` they must have the same value or for `String` they must have the same string content), falling back on equality of memory reference if the operator is not overloaded.

There are runtime checks being performed in order to ensure that the performed operation is allowed. Examples of such are:

- For the `MINUS` unary expression (e.g. `-(25 + 50)`), we must ensure that the right operand is a number;
- For the `PLUS` binary expression, we must ensure that both the left and right operands are either both `Double` (e.g. `25 + 50.0`) or both `String` for string concatenation (e.g. `"Hello" + "world"`);
- For the other arithmetic and comparison operators, we must ensure that both operands are numbers.

If such errors occurs a `RuntimeError` is thrown.
