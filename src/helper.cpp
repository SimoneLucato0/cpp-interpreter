#include "helper.hpp"

#include <algorithm>
#include <cctype>
#include <string>

bool isDigit(char c) { return c >= '0' && c <= '9'; }

bool isCharacter(char c) { return (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || c == '_'; }

std::string toLowercase(const std::string &s) {
    std::string copy;
    copy.reserve(s.size());

    std::transform(s.begin(), s.end(), std::back_inserter(copy),
                   [](unsigned char c) { return std::tolower(c); });

    return copy;
}