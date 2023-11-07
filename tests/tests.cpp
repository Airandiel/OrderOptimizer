#include "Configuration.h"

#include <cassert>
#include <filesystem>
#include <fstream>
#include <iostream>

#include "OrderSearcher.h"

void testLoadConfigFromFile() {
    std::cout << "Testing load config from file: ";
    Configuration config;

    const std::string yaml_data =
        "- id: 1\n"
        "  product: \"Product 1\"\n"
        "  parts:\n"
        "  - part: \"Part A\"\n"
        "    cx: 791.86304\n"
        "    cy: 732.23236\n"
        "- id: 2\n"
        "  product: \"Product 2\"\n"
        "  parts:\n"
        "  - part: \"Part A\"\n"
        "    cx: 791.86304\n"
        "    cy: 732.23236\n"
        "  - part: \"Part B\"\n"
        "    cx: 550.09924\n"
        "    cy: 655.423\n";

    // create temp file
    const std::string temp_file = "test_config.yaml";
    std::ofstream file(temp_file);
    file << yaml_data;
    file.close();

    // use temporary file, because otherwise the path from executable changes
    bool result = config.loadConfigFromFile(temp_file);

    // Check if file was loaded succesfully
    assert(result && "File load failed!");

    // check number of products
    assert(config.products.size() > 0);
    assert(config.getProduct("Product 1").parts.size() == 1 &&
           "Get Product1 size failed!");
    assert(config.getProduct("Product 1").id == 1 && "Get Product1 id failed!");
    assert(config.getProduct("Product 2").parts.size() == 2 &&
           "Get Product2 size failed!");
    assert(config.getProduct("Product 2").id == 2 && "Get Product2 id failed!");

    // Product1 tests
    assert(config.getProduct("Product 1").parts[0].name == "Part A" &&
           "Get Product1 parts[0].name failed!");
    assert(config.getProduct("Product 1").parts[0].cx == 791.86304);
    assert(config.getProduct("Product 1").parts[0].cy == 732.23236);

    // Product2 tests
    assert(config.getProduct("Product 2").parts[0].name == "Part A");
    assert(config.getProduct("Product 2").parts[0].cx == 791.86304);
    assert(config.getProduct("Product 2").parts[0].cy == 732.23236);
    assert(config.getProduct("Product 2").parts[1].name == "Part B");
    assert(config.getProduct("Product 2").parts[1].cx == 550.09924);
    assert(config.getProduct("Product 2").parts[1].cy == 655.423);

    std::cout << "PASSED" << std::endl;
}

void testLoadOrderFromFile() {
    std::cout << "Testing load Order from file: ";
    Configuration config;

    const std::string yaml_data =
        "- order: 1000001\n"
        "  cx: 748.944\n"
        "  cy: 474.71707\n"
        "  products:\n"
        "  - 902\n"
        "  - 293\n"
        "  - 142\n"
        "  - 56\n"
        "  - 894";

    // create temp file
    std::filesystem::create_directory("temp_dir");
    const std::string temp_file = "temp_dir/test_config.yaml";
    std::ofstream file(temp_file);
    file << yaml_data;
    file.close();

    OrderSearcher orderSearcher;

    bool result =
        orderSearcher.startOrderSearchInDirectory(1000001, "temp_dir/");
    if (!result) {
        std::cout << "Problem with reading order from configuration, order "
                     "not found, order number: "
                  << 1000001 << std::endl;
        return;
    }

    const Order& found_order = orderSearcher.getFoundOrder();
    assert(result && "Order was found");

    // check number of products
    assert(found_order.cx == 748.944 && "Order cx failed");
    assert(found_order.cy == 474.71707 && "Order cy failed");
    assert(found_order.number == 1000001 && "Order number failed");
    assert(found_order.products[0] == 902 && "Order product 0 failed");
    assert(found_order.products[1] == 293 && "Order product 1 failed");
    assert(found_order.products[2] == 142 && "Order product 2 failed");
    assert(found_order.products[3] == 56 && "Order product 3 failed");
    assert(found_order.products[4] == 894 && "Order product 4 failed");

    std::cout << "PASSED" << std::endl;
}
int main() {
    testLoadConfigFromFile();
    testLoadOrderFromFile();

    std::cout << "All tests passed." << std::endl;

    return 0;
}
