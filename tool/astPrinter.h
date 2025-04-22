#include <sstream>

#include "expr.h"

class AstPrinter : public ExprVisitor<std::string> {
   private:
    template <typename... Expr>
    std::string parenthesize(const std::string& name, const Expr&... es) {
        std::stringstream ss;
        ss << "(" << name;
        ((ss << " " << es.accept(*this)), ...);
        ss << ")";
        return ss.str();
    }

   public:
    std::string print(const Expr& expr) { return expr.accept(*this); }

    void visitBinary(const Binary& expr) override {
        result = parenthesize(expr.operation.lexeme, *expr.left, *expr.right);
    }

    void visitGrouping(const Grouping& expr) override {
        result = parenthesize("group", *expr.expression);
    }

    void visitLiteral(const Literal& expr) override { result = expr.value; }

    void visitUnary(const Unary& expr) override {
        result = parenthesize(expr.operation.lexeme, *expr.right);
    }
};
