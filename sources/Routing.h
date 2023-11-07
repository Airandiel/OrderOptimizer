#pragma once
#include <cmath>
#include <iostream>
#include <limits>
#include <vector>

#include "Configuration.h"
#include "OrderSearcher.h"
#include "Product.h"

inline bool getPartsPositions(Order order, std::vector<Part>& parts,
                              Configuration& config) {
    // get all parts from config for order
    for (int product_id : order.products) {
        Product new_product;
        bool result = config.getProductWithID(new_product, product_id);
        if (result) {
            // append parts
            parts.insert(parts.end(), new_product.parts.begin(),
                         new_product.parts.end());

        } else {
            std::cout << product_id << std::endl;
            return false;
        }
    }
    return true;
}

inline double calculateDistance(const Part& part1, const Part& part2) {
    double dx = part1.cx - part2.cx;
    double dy = part1.cy - part2.cy;
    return std::sqrt(dx * dx + dy * dy);
}

inline int findNearestNeighbor(const std::vector<Part>& parts,
                               std::vector<bool>& visited, int current_part) {
    int nearest_neighbor = -1;
    double min_distance = std::numeric_limits<double>::max();

    for (int i = 0; i < (int)parts.size(); i++) {
        if (!visited[i] && i != current_part) {
            double distance = calculateDistance(parts[current_part], parts[i]);
            if (distance < min_distance) {
                min_distance = distance;
                nearest_neighbor = i;
            }
        }
    }

    return nearest_neighbor;
}

inline std::vector<int> solveTSP(const std::vector<Part>& parts) {
    // solve TSP using the nearest neighbor algorithm, doesn't find the shortest
    // possible path, just the path to the nearest neighbour and next one, etc
    std::vector<bool> visited(parts.size(), false);
    std::vector<int> path;
    int current_part = 0;
    visited[current_part] = true;
    path.push_back(current_part);

    while (path.size() < parts.size()) {
        int nearest_neighbor =
            findNearestNeighbor(parts, visited, current_part);
        if (nearest_neighbor != -1) {
            visited[nearest_neighbor] = true;
            path.push_back(nearest_neighbor);
            current_part = nearest_neighbor;
        }
    }

    return path;
}

inline void printShortestPath(std::vector<Part> parts,
                              AMRPosition& AMR_position,
                              std::vector<int> shortest_path) {
    std::cout << "1.\tAMR starts from x: " << AMR_position.x
              << ", y: " << AMR_position.y << ", yaw: " << AMR_position.yaw
              << std::endl;
    for (unsigned i = 1; i < shortest_path.size(); i++) {
        Part current_part = parts[shortest_path[i]];
        std::cout << i + 1 << ".\t Fetching part \'" << current_part.name
                  << "\' for product \'" << current_part.product_id
                  << "\' at x: " << current_part.cx
                  << ", y: " << current_part.cy << std::endl;
    }
    Part current_part = parts[parts.size() - 1];
    std::cout << parts.size() + 1
              << ".\t Delivering to destination x: " << current_part.cx
              << ", y: " << current_part.cy << std::endl;
}

inline bool planOrder(const Order& order, const std::string description,
                      Configuration& config, AMRPosition& AMR_position) {
    // find the shortest route for all parts for order and print the route
    std::vector<Part> parts;
    Part destination;
    destination.name = "Order destination";
    destination.cx = order.cx;
    destination.cy = order.cy;
    Part start;
    start.name = "AMR position";
    start.cx = AMR_position.x;
    start.cy = AMR_position.y;
    AMRPosition start_position = AMR_position;
    parts.push_back(start);
    bool result = getPartsPositions(order, parts, config);
    if (result) {
        std::cout << "Order acquired" << std::endl;
        std::cout << "Working on order " << order.number << " " << description
                  << std::endl;
        std::vector<int> shortest_path;
        shortest_path = solveTSP(parts);

        AMR_position.x = order.cx;
        AMR_position.y = order.cy;
        // calculate yaw in degrees
        AMR_position.yaw =
            std::atan2(AMR_position.y - parts[parts.size() - 1].cy,
                       AMR_position.x - parts[parts.size() - 1].cx) *
            180 / M_PI;

        parts.push_back(destination);
        printShortestPath(parts, start_position, shortest_path);

    } else {
        std::cout << "Order products not found" << std::endl;
        return false;
    }

    return true;
}
