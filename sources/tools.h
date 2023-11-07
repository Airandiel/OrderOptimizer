#pragma once
#include <string>

inline std::string trimString(const std::string& str) {
    // Trim string from front and back erasing whitespaces and quotation marks
    std::string result = str;

    // Trim from the front
    while (!result.empty() &&
           (std::isspace(result.front()) || result.front() == '"')) {
        result.erase(result.begin());
    }

    // Trim from the back
    while (!result.empty() &&
           (std::isspace(result.back()) || result.back() == '"')) {
        result.pop_back();
    }

    return result;
}

inline std::string trimFrontOfString(const std::string& str,
                                     const char trim_char) {
    std::string result = str;
    while (!result.empty() &&
           (std::isspace(result.front()) || result.front() == trim_char)) {
        result.erase(result.begin());
    }
    return result;
}