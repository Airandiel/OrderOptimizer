#include "Configuration.h"

#include <algorithm>
#include <fstream>
#include <iostream>
#include <sstream>
#include <stdexcept>
// #include <typeinfo>

#include "Product.h"
#include "tools.h"

bool compareProductsById(const Product& a, const Product& b) {
    return a.id < b.id;
}

void Configuration::addProduct(const Product& product) {
    products.emplace_back(product);
}

const Product& Configuration::getProduct(
    const std::string& product_name) const {
    for (const Product& product : products) {
        if (product.name == product_name) {
            return product;
        }
    }
    throw std::runtime_error("Product not found");
}

bool Configuration::getProductOnIndex(Product& found_product,
                                      const unsigned index) {
    if (index < products.size()) {
        found_product = products[index];
    } else {
        return false;
    }
    return true;
}

bool Configuration::getProductWithID(Product& found_product, const int id) {
    int index = id - 1;
    bool result = getProductOnIndex(found_product, index);

    if (result && found_product.id == id) {
        return true;
    } else {
        // if the orders are not in sequence search using binary search
        auto it = std::lower_bound(products.begin(), products.end(), id,
                                   [](const Product& product, int target_id) {
                                       return product.id < target_id;
                                   });
        if (it != products.end() && it->id == id) {
            found_product.id = it->id;
            found_product.name = it->name;
            found_product.parts = it->parts;
            return true;
        }
    }
    return false;
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
            part.product_id = product.id;

            product.parts.push_back(part);  // Push ready product
        }
    }

    products.push_back(product);  // Add the last product
    this->products = products;
    std::sort(products.begin(), products.end(), compareProductsById);
    file.close();
    return true;
}
