// // simulated mqtt interface using two processes
// #include <condition_variable>
// #include <iostream>
// #include <mutex>
// #include <queue>
// #include <thread>

// std::mutex mtx;
// std::condition_variable cv;
// std::queue<std::string> messages;
// bool not_end = true;

// struct AMR {
//     double cx;
//     double cy;
//     double yaw;
// };
// // pulisher thread
// void publisher() {
//     std::string message = "";
//     while (not_end) {
//         std::string user_input;
//         std::getline(std::cin, user_input);
//         if (user_input.find("currentPosition") != std::string::npos) {
//             message = "currentPosition";
//         } else if (user_input.find("nextOrder") != std::string::npos) {
//             std::cout << "uint32 order_id: ";
//             std::cin >> user_input;
//             std::string description;
//             std::cout << "string description: ";
//             std::cin >> description;
//             message = "nextOrder," + user_input + ";" + description;
//         } else if (user_input.find("quit") != std::string::npos) {
//             std::lock_guard<std::mutex> lock(mtx);
//             message = "quit";
//             not_end = false;
//         }
//         messages.push(message);
//         cv.notify_one();
//     }
// }

// // subscriber thread
// std::string listener() {
//     std::unique_lock<std::mutex> lock(mtx);
//     while (not_end) {
//         if (cv.wait_for(lock, std::chrono::seconds(2),
//                         [] { return !messages.empty(); })) {
//             std::string message = messages.front();
//             messages.pop();

//             return message;
//         }
//     }
// }

// void handleMessage(const std::string message) {
//     if (message.find("currentPosition") != std::string::npos) {
//         std::cout << "double x: " <<
//     }
// }