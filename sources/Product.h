#pragma once
#include <string>
#include <vector>

struct Part {
    std::string name;
    double cx;
    double cy;
};

struct Product {
    int id;
    std::string name;
    std::vector<Part> parts;
};
