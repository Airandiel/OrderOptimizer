#include "AMRInterface.h"

#include <cctype>
#include <string>

#include "OrderSearcher.h"
#include "Routing.h"

AMRInterface::AMRInterface(Configuration& config, std::string project_directory)
    : config(config), project_directory(project_directory), not_end(true) {
    current_position.x = 0;
    current_position.y = 0;
    current_position.yaw = 0;
}

void AMRInterface::start() {
    MessageContent message;
    while (not_end) {
        std::cout << "Enter command: ";
        std::string user_input;
        std::getline(std::cin, user_input);
        std::string lowered = "";
        for (char c : user_input) {
            lowered += std::tolower(c);
        }
        if (lowered.find("currentposition") != std::string::npos) {
            message.current_position = true;
            message.next_order = false;
            message.quit = false;
        } else if (lowered.find("nextorder") != std::string::npos) {
            message.current_position = false;
            message.next_order = true;
            message.quit = false;
            std::cout << "uint32 order_id: ";
            std::getline(std::cin, user_input);
            std::istringstream iss(user_input);
            iss >> message.order_id;

            std::cout << "string description: ";
            std::getline(std::cin, message.description);
        } else if (lowered.find("quit") != std::string::npos) {
            message.quit = true;
            not_end = false;
        } else {
            message.current_position = false;
            message.next_order = false;
            message.quit = false;
        }
        handleMessage(message);
    }
}

void AMRInterface::handleMessage(const MessageContent message) {
    if (message.quit) {
        std::cout << "quit" << std::endl;
        not_end = false;
    } else if (message.current_position) {
        std::cout << "double x: " << current_position.x << std::endl;
        std::cout << "double y: " << current_position.y << std::endl;
        std::cout << "double yaw: " << current_position.yaw << std::endl;
    } else if (message.next_order) {
        runNextOrder(message);
    } else {
        std::cerr << "Unknown message. You can enter - currentPostion, "
                     "nextOrder or quit"
                  << std::endl;
    }
}

void AMRInterface::runNextOrder(const MessageContent message) {
    try {
        OrderSearcher orderSearcher;

        bool result = orderSearcher.startOrderSearchInDirectory(
            message.order_id, project_directory + "/orders/");
        if (!result) {
            std::cout << "Problem with reading order from configuration, order "
                         "not found, order number: "
                      << message.order_id << std::endl;
            return;
        }

        const Order& found_order = orderSearcher.getFoundOrder();
        result = planOrder(found_order, message.description, config,
                           current_position);

        if (!result) {
            std::cout << "Problem with planning order" << std::endl;
            return;
        }
    } catch (const std::exception& e) {
        std::cerr << "Exception in runNextOrder: " << e.what() << std::endl;
    }
}
