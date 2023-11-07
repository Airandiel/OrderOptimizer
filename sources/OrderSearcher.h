#pragma once

#include <filesystem>
#include <fstream>
#include <iostream>
#include <mutex>
#include <sstream>
#include <thread>
#include <vector>

struct Order {
    int number;
    double cx;
    double cy;
    std::vector<int> products;
};

class OrderSearcher {
   public:
    OrderSearcher();
    virtual ~OrderSearcher();

    bool startOrderSearchInDirectory(int order_number,
                                     const std::string& folder_path);

    const Order& getFoundOrder() const;

   private:
    bool getOrderDetails(std::ifstream& file, int order_number);

    bool findOrderInFile(const std::string& file_path, int order_id);

    void processFile(const std::string& file_path, int order_number);

    static std::mutex orderMutex;
    static bool is_found;
    static Order found_order;
};
