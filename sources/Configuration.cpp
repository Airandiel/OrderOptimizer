#include "Configuration.h"

#include <fstream>
#include <iostream>
#include <sstream>
#include <stdexcept>
// #include <typeinfo>

#include "Product.h"
#include "tools.h"

void Configuration::addProduct(const Product& product) {
    products.emplace_back(product);
}

const Product& Configuration::getProduct(const std::string& productName) const {
    for (const Product& product : products) {
        if (product.name == productName) {
            return product;
        }
    }
    throw std::runtime_error("Product not found");
}

bool Configuration::loadConfigFromFile(const std::string& file_path) {
    // Load configuration from yaml file
    std::ifstream file(file_path);

    // is the file is opened successfully
    if (!file.is_open()) {
        return false;
    }

    std::vector<Product> products;
    Product product;
    Part part;

    std::string line;
    std::string find_id = "- id:";
    std::string find_product = "product:";
    std::string find_part = "- part:";
    std::string find_cx = "cx:";
    std::string find_cy = "cy:";

    while (std::getline(file, line)) {
        if (line.find(find_id) != std::string::npos) {
            // Find id line
            if (!product.name.empty()) {
                products.push_back(product);
                product = Product();
            }
            std::istringstream iss(
                trimString(line.substr(line.find(find_id) + find_id.length())));
            iss >> product.id;
        } else if (line.find(find_product) != std::string::npos) {
            // Find product name line
            product.name = trimString(
                line.substr(line.find(find_product) + find_product.length()));
        } else if (line.find(find_part) != std::string::npos) {
            // Read part details - name, cx, cy
            part.name = trimString(
                line.substr(line.find(find_part) + find_part.length()));

            std::getline(file, line);  // Read cx line
            std::istringstream iss2(
                trimString(line.substr(line.find(find_cx) + find_cx.length())));
            iss2 >> part.cx;
            // std::cout << "Part cx: " << part.cx << " type "
            // << typeid(part.cx).name() << std::endl;

            std::getline(file, line);  // Read cy line
            std::istringstream iss3(
                trimString(line.substr(line.find(find_cy) + find_cy.length())));
            iss3 >> part.cy;

            product.parts.push_back(part);  // Push ready product
        }
    }

    products.push_back(product);  // Add the last product
    this->products = products;
    file.close();
    return true;
}
