#pragma once
#include <string>

#include "token.h"

std::string TokenTypeToString(TokenType t);
std::string TokenTypeToLexeme(TokenType t);
bool isDigit(char c);
bool isCharacter(char c);
std::string toLowercase(const std::string &s);