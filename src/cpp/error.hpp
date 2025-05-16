#pragma once

#include <iostream>
#include <string>

#include "token/tokenType.hpp"
#include "token/token.hpp"

class Error {
   private:
    template <typename T, typename... Rest>
    static void report(const T& first, const Rest&... rest) {
        std::cerr << "[line " << first << "]";
        ((std::cerr << rest), ...);
        std::cerr << "\n";
    }

   public:
    static void output(Token token, const std::string& message) {
        if (token.type == TokenType::EOF_TOKEN) {
            report(token.line, " at end", message);
        } else {
            report(token.line, " at '", token.lexeme, "': ", message);
        }
    }
};
