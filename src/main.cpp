// main.cpp
#include <iostream>
#include <fstream>
#include "BankerAlgorithm.h"

int main() {
    std::ifstream infile("src/input.txt");
    if (!infile) {
        std::cerr << "Failed to open input.txt." << std::endl;
        return 1;
    }

    int num_processes, num_resources;
    infile >> num_processes >> num_resources;
    std::vector<std::vector<int>> alloc(num_processes, std::vector<int>(num_resources));
    std::vector<std::vector<int>> max(num_processes, std::vector<int>(num_resources));
    std::vector<int> avail(num_resources);

    for (int i = 0; i < num_processes; ++i)
        for (int j = 0; j < num_resources; ++j)
            infile >> alloc[i][j];

    for (int i = 0; i < num_processes; ++i)
        for (int j = 0; j < num_resources; ++j)
            infile >> max[i][j];

    for (int j = 0; j < num_resources; ++j)
        infile >> avail[j];

    BankerAlgorithm ba(num_processes, num_resources);
    ba.setAllocation(alloc);
    ba.setMaximum(max);
    ba.setAvailable(avail);

    ba.findSafeSequence();
    ba.findAllSafeSequences();

    return 0;
}
