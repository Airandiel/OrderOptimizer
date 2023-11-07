#include <iostream>

#include "Configuration.h"
#include "Order.h"

int main() {
    Configuration config;
    bool result = config.loadConfigFromFile("configuration/products.yaml");
    // Order found_order;

    // result = startOrderSearchInDirectory(
    //     found_order, 12324,
    //     "/mnt/d/Michal_share/projectsCPP/knapp/"
    //     "amr_example_fcs_1/amr_example_fcs_1/orders/");

    OrderSearcher orderSearcher;

    result = orderSearcher.startOrderSearchInDirectory(
        1400080,
        "/mnt/d/Michal_share/projectsCPP/knapp/"
        "amr_example_fcs_1/amr_example_fcs_1/orders/");

    const Order& found_order = orderSearcher.getFoundOrder();
    // result = findOrderInFile(
    //     found_order,
    //     "/mnt/d/Michal_share/projectsCPP/knapp/amr_example_fcs_1/"
    //     "amr_example_fcs_1/orders/orders_20201201.yaml",
    //     1000003);
    std::cout << result << ", " << found_order.number << " " << found_order.cx
              << " products:" << std::endl;
    for (int i = 0; i < found_order.products.size(); i++) {
        std::cout << found_order.products[i] << std::endl;
    }
    return 0;
}