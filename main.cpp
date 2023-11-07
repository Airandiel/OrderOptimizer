#include <iostream>

#include "AMRInterface.h"
#include "Configuration.h"

int main(int argc, char* argv[]) {
    if (argc < 2) {
        std::cerr << "Usage: " << argv[0] << " <project_directory>"
                  << std::endl;
        return 1;
    }
    std::string project_directory = argv[1];
    Configuration config;
    bool result = config.loadConfigFromFile(project_directory +
                                            "/configuration/products.yaml");

    if (result) {
        AMRInterface interface(config, project_directory);
        interface.start();
    } else {
        std::cerr << "Error loading configuration." << std::endl;
    }

    return 0;
}