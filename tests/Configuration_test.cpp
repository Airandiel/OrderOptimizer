#include "Configuration.h"

#include <cassert>
#include <fstream>
#include <iostream>

void testLoadConfigFromFile() {
    std::cout << "Testing load config from file: ";
    Configuration config;

    const std::string yamlData =
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
    const std::string tempFile = "test_config.yaml";
    std::ofstream file(tempFile);
    file << yamlData;
    file.close();

    // use temporary file, because otherwise the path from executable changes
    bool result = config.loadConfigFromFile(tempFile);

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

int main() {
    testLoadConfigFromFile();

    std::cout << "All tests passed." << std::endl;

    return 0;
}
