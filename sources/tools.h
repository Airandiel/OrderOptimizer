#pragma once
#include <string>

std::string trimString(const std::string& str) {
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