#pragma once
#include <string>
#include <vector>

#include "Product.h"

class Configuration {
   public:
    std::vector<Product> products;

    void addProduct(const Product& product);

    const Product& getProduct(const std::string& productName) const;

    bool loadConfigFromFile(const std::string& file_path);
};
