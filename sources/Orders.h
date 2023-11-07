#pragma once

#include <filesystem>
#include <fstream>
#include <iostream>
#include <mutex>
#include <sstream>
#include <thread>
#include <vector>

#include "tools.h"

struct Order {
    int number;
    double cx;
    double cy;
    std::vector<int> products;
};

std::mutex orderMutex;
Order found_order;
bool is_found;

bool getOrderDetails(std::ifstream& file, const int order_number) {
    std::lock_guard<std::mutex> lock(orderMutex);
    found_order.number = order_number;
    std::string line;
    std::string find_cx = "cx: ";
    std::string find_cy = "cy: ";

    // read cx
    std::getline(file, line);
    std::istringstream iss(
        trimString(line.substr(line.find(find_cx) + find_cx.length())));
    iss >> found_order.cx;

    // read cy
    std::getline(file, line);
    std::istringstream iss1(
        trimString(line.substr(line.find(find_cy) + find_cy.length())));
    iss1 >> found_order.cy;

    // read products
    std::getline(file, line);  // omit line with just product string
    std::getline(file, line);
    bool any_product = false;
    while (file && line[0] != '-') {
        int product_number = std::stoi(trimFrontOfString(line, '-'));
        found_order.products.push_back(product_number);
        any_product = true;
        std::getline(file, line);
    }
    if (!any_product) {
        return false;
    }
    return true;
}

bool findOrderInFile(const std::string& file_path, const int order_id) {
    std::string order_id_string = std::to_string(order_id);
    std::ifstream file(file_path);
    if (!file.is_open()) {
        std::cerr << "Failed to open the file." << std::endl;
        return false;
    }

    std::string line;
    bool result = false;
    std::string find_order = "order: ";
    while (std::getline(file, line)) {
        if (line[0] = '-') {
            if (line.find(find_order) != std::string::npos) {
                std::string id_string =
                    line.substr(line.find(find_order) + find_order.length());
                if (id_string == order_id_string) {
                    result = getOrderDetails(file, order_id);
                }
            }
        }
    }
    return result;
}

void processFile(const std::string& file_path, const int order_number) {
    if (findOrderInFile(file_path, order_number)) {
        std::lock_guard<std::mutex> lock(orderMutex);
        is_found = true;
        std::cout << "Found order in file: " << file_path << std::endl;
    }
}

bool startOrderSearchInDirectory(Order& order, const int order_number,
                                 const std::string& folder_path) {
    // function iterates over all files in orders directory, starts new thread
    // for search in each file and waits till all threads will finish, returns
    // true if the order was found
    std::vector<std::thread> threads;
    is_found = false;
    for (const auto& entry : std::filesystem::directory_iterator(folder_path)) {
        const std::string& file_name = entry.path().filename().string();
        std::thread worker(
            [=]() { processFile(folder_path + file_name, order_number); });
        threads.push_back(std::move(worker));
    }
    for (auto& thread : threads) {
        thread.join();
    }
    order = found_order;
    return is_found;
}
