#pragma once

#include <condition_variable>
#include <iostream>
#include <mutex>
#include <queue>
#include <string>
#include <thread>

#include "Configuration.h"
#include "Product.h"

struct MessageContent {
    bool current_position;
    bool next_order;
    uint32_t order_id;
    std::string description;
    bool quit;
};

class AMRInterface {
   public:
    AMRInterface(Configuration& config, std::string project_directory);

    void start();

    void handleMessage(const MessageContent message);

   private:
    AMRPosition current_position;
    Configuration config;
    std::string project_directory;
    bool not_end;

    void runNextOrder(const MessageContent message);
};
