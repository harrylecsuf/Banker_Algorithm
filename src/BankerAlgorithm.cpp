#include "BankerAlgorithm.h"

BankerAlgorithm::BankerAlgorithm(int p, int r) : num_processes(p), num_resources(r),
                                    allocate(p, std::vector<int>(r, 0)),
                                    maximum(p, std::vector<int>(r, 0)),
                                    need(p, std::vector<int>(r, 0)),
                                    available(r, 0) {}

void BankerAlgorithm::setAllocation(const std::vector<std::vector<int>>& alloc) {
    allocate = alloc;
}

void BankerAlgorithm::setMaximum(const std::vector<std::vector<int>>& max) {
    maximum = max;
}

void BankerAlgorithm::setAvailable(const std::vector<int>& avail) {
    available = avail;
}

void BankerAlgorithm::calculateNeed() {
    for (int i = 0; i < num_processes; ++i)
        for (int j = 0; j < num_resources; ++j)
            need[i][j] = maximum[i][j] - allocate[i][j];
}

bool BankerAlgorithm::checkSafe() {
    std::vector<int> work = available;
    std::vector<bool> finish(num_processes, false);
    int count = 0; // Count of finished processes
    while (count < num_processes) {
        bool found = false;
        for (int i = 0; i < num_processes; ++i) {
            if (!finish[i]) {
                int j;
                for (j = 0; j < num_resources; ++j) {
                    if (need[i][j] > work[j])
                        break;
                }
                if (j == num_resources) { // If all needs are met
                    for (int k = 0; k < num_resources; ++k)
                        work[k] += allocate[i][k];
                    finish[i] = true;
                    found = true;
                    count++;
                }
            }
        }
        if (!found) // If no process can be executed, we're in an unsafe state
            return false;
    }
    return true; // If all processes finish, the state is safe
}

std::vector<int> BankerAlgorithm::findSafeSequence() {
    std::vector<int> safe_sequence;
    std::vector<int> work = available;
    std::vector<bool> finish(num_processes, false);
    int count = 0; // Count of finished processes
    while (count < num_processes) {
        bool found = false;
        for (int i = 0; i < num_processes; ++i) {
            if (!finish[i]) {
                int j;
                for (j = 0; j < num_resources; ++j) {
                    if (need[i][j] > work[j])
                        break;
                }
                if (j == num_resources) { // If all needs are met
                    for (int k = 0; k < num_resources; ++k)
                        work[k] += allocate[i][k];
                    finish[i] = true;
                    found = true;
                    count++;
                    safe_sequence.push_back(i);
                }
            }
        }
        if (!found) // If no process can be executed, we're in an unsafe state
            break;
    }
    return safe_sequence;
}
