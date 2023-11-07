#include <iostream>

#include "Configuration.h"
#include "OrderSearcher.h"
#include "Routing.h"

int main() {
    Configuration config;
    bool result = config.loadConfigFromFile(
        "/mnt/d/Michal_share/projectsCPP/knapp/amr_example_fcs_1/"
        "amr_example_fcs_1/configuration/products.yaml");
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
    // std::vector<Product> products;
    // result = getProductsPositions(found_order, products, config);

    // std::cout << result << ", " << found_order.number << " " <<
    // found_order.cx
    //           << " products:" << std::endl;
    // for (int i = 0; i < products.size(); i++) {
    //     std::cout << products[i].name << std::endl;
    // }

    planTheOrder(found_order, "DUUUUUPA", config);
    return 0;
}