#include <cmath>
#include <iostream>
#include <limits>
#include <vector>

#include "Configuration.h"
#include "OrderSearcher.h"
#include "Product.h"

bool getPartsPositions(Order order, std::vector<Part>& parts,
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

double calculateDistance(const Part& part1, const Part& part2) {
    double dx = part1.cx - part2.cx;
    double dy = part1.cy - part2.cy;
    return std::sqrt(dx * dx + dy * dy);
}

int findNearestNeighbor(const std::vector<Part>& parts,
                        std::vector<bool>& visited, int currentPart) {
    int nearestNeighbor = -1;
    double minDistance = std::numeric_limits<double>::max();

    for (int i = 0; i < parts.size(); ++i) {
        if (!visited[i] && i != currentPart) {
            double distance = calculateDistance(parts[currentPart], parts[i]);
            if (distance < minDistance) {
                minDistance = distance;
                nearestNeighbor = i;
            }
        }
    }

    return nearestNeighbor;
}

std::vector<int> solveTSP(const std::vector<Part>& parts) {
    // solve TSP using the nearest neighbor algorithm
    std::vector<bool> visited(parts.size(), false);
    std::vector<int> path;
    int currentPart = 0;
    visited[currentPart] = true;
    path.push_back(currentPart);

    while (path.size() < parts.size()) {
        int nearestNeighbor = findNearestNeighbor(parts, visited, currentPart);
        if (nearestNeighbor != -1) {
            visited[nearestNeighbor] = true;
            path.push_back(nearestNeighbor);
            currentPart = nearestNeighbor;
        }
    }

    return path;
}

bool planTheOrder(const Order& order, const std::string description,
                  Configuration& config) {
    // find the shortest route for all parts for order
    std::vector<Part> parts;
    Part destination;
    destination.name = "Order destination";
    destination.cx = order.cx;
    destination.cy = order.cy;
    parts.push_back(destination);
    bool result = getPartsPositions(order, parts, config);
    if (result) {
        std::cout << "Order acquired" << std::endl;
    } else {
        std::cout << "Order products not found" << std::endl;
        return false;
    }

    std::cout << "Working on order " << order.number << " " << description
              << std::endl;
    std::vector<int> shortest_path;
    shortest_path = solveTSP(parts);
    int index = 1;
    for (int i = shortest_path.size() - 1; i > 0; i--) {
        Part current_part = parts[shortest_path[i]];
        std::cout << index << ".\t Fetching part \'" << current_part.name
                  << "\' for product \'" << current_part.product_id
                  << "\' at x: " << current_part.cx
                  << ", y: " << current_part.cy << std::endl;
        index++;
    }
    Part current_part = parts[shortest_path[0]];
    std::cout << index << ".\t Delivering to destination x: " << current_part.cx
              << ", y: " << current_part.cy << std::endl;
    return true;
}