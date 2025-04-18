#pragma once
#include <string>

#include "token.h"

std::string TokenTypeToString(TokenType t) throw();
std::string TokenTypeToLexeme(TokenType t) throw();