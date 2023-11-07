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

void AMRInterface::publisher() {
    MessageContent message;
    while (not_end) {
        std::cout << "Publisher" << std::endl;
        std::string user_input;
        std::getline(std::cin, user_input);
        // std::cout << "Publisher send" << std::endl;
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

            // std::cin >> message.order_id;
            std::cout << "string description: ";
            std::getline(std::cin, message.description);
        } else if (lowered.find("quit") != std::string::npos) {
            // std::lock_guard<std::mutex> lock(mtx);
            message.quit = true;
            not_end = false;
        } else {
            message.current_position = false;
            message.next_order = false;
            message.quit = false;
        }
        std::lock_guard<std::mutex> lock(mtx);
        messages.push(message);
        cv.notify_one();

        std::cout << "Publisher send" << std::endl;
    }
}

void AMRInterface::listener() {
    while (not_end) {
        // std::cout << "Listener" << std::endl;
        std::unique_lock<std::mutex> lock(mtx);
        if (cv.wait_for(lock, std::chrono::seconds(2),
                        [&] { return !messages.empty(); })) {
            std::cout << "Listener received" << std::endl;
            MessageContent message = messages.front();
            messages.pop();
            handleMessage(message);
        }
    }
}

void AMRInterface::handleMessage(const MessageContent message) {
    std::cout << "handle message" << std::endl;
    if (message.quit) {
        std::cout << "quit" << std::endl;
        not_end = false;
    } else if (message.current_position) {
        std::cout << "current_position" << std::endl;
        std::cout << "double x: " << current_position.x << std::endl;
        std::cout << "double y: " << current_position.y << std::endl;
        std::cout << "double yaw: " << current_position.yaw << std::endl;
    } else if (message.next_order) {
        std::cout << "next_order" << std::endl;
        // runNextOrder(message);
        std::thread worker([=]() { runNextOrder(message); });
    } else {
        std::cout << "Unknown message." << std::endl;
    }
}

void AMRInterface::runNextOrder(const MessageContent message) {
    try {
        std::cout << "thread started";
        OrderSearcher orderSearcher;

        bool result = orderSearcher.startOrderSearchInDirectory(
            message.order_id, project_directory + "/orders/");
        if (!result) {
            std::cout << "Problem with reading order from configuration"
                      << std::endl;
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
        // Handle the exception, or log it for debugging
        std::cerr << "Exception in runNextOrder: " << e.what() << std::endl;
    }
}

void AMRInterface::start() {
    std::cout << "Start" << std::endl;
    publisher_thread = std::thread(&AMRInterface::publisher, this);
    listener_thread = std::thread(&AMRInterface::listener, this);
}

void AMRInterface::join() {
    // not_end = false;
    // cv.notify_all();
    if (publisher_thread.joinable()) {
        publisher_thread.join();
    }
    if (listener_thread.joinable()) {
        listener_thread.join();
    }
}
