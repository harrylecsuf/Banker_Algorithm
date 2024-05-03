/**
 * Use the code below if you'd like to
 * test a safe system workload
 * Uncomment this part when using the code underneath
 * this part for an UNSAFE system state (DEADLOCK)
*/
#include <iostream>
#include "BankerAlgorithm.h"

int main() {
    // Example data setup
    int num_processes = 5;
    int num_resources = 3;

    std::vector<std::vector<int>> alloc = {{0, 1, 0}, {2, 0, 0}, {3, 0, 2}, {2, 1, 1}, {0, 0, 2}};
    std::vector<std::vector<int>> max = {{7, 5, 3}, {3, 2, 2}, {9, 0, 2}, {2, 2, 2}, {4, 3, 3}};
    std::vector<int> avail = {3, 3, 2};

    BankerAlgorithm ba(num_processes, num_resources);
    ba.setAllocation(alloc);
    ba.setMaximum(max);
    ba.setAvailable(avail);
    ba.calculateNeed();

    if (ba.checkSafe()) {
        std::cout << "The system is in a safe state." << std::endl;
    } else {
        std::cout << "The system is in an unsafe state (deadlock)." << std::endl;
    }

    return 0;
}
/**
 * Uncomment the following code if
 * you'd like to test a scenario
 * where the system workload
 * is unsafe to be executed and thus creating a
 * DEADLOCK
*/
// #include <iostream>
// #include "BankerAlgorithm.h"

// int main() {
//     // Example data setup
//     int num_processes = 3;
//     int num_resources = 3;

//     // Allocation matrix
//     std::vector<std::vector<int>> alloc = {{0, 1, 0}, {2, 0, 0}, {3, 0, 2}};

//     // Maximum matrix
//     std::vector<std::vector<int>> max = {{7, 5, 3}, {3, 2, 2}, {9, 0, 2}};

//     // Available resources vector
//     std::vector<int> avail = {3, 3, 2};

//     // Create a BankerAlgorithm instance
//     BankerAlgorithm ba(num_processes, num_resources);

//     // Set allocation, maximum, and available resources
//     ba.setAllocation(alloc);
//     ba.setMaximum(max);
//     ba.setAvailable(avail);

//     // Calculate need matrix
//     ba.calculateNeed();

//     // Check if the system is in a safe state
//     if (ba.checkSafe()) {
//         std::cout << "The system is in a safe state." << std::endl;
//     } else {
//         std::cout << "The system is in an unsafe state (deadlock)." << std::endl;
//     }

//     return 0;
// }
