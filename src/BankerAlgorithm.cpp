// BankerAlgorithm.cpp
#include "BankerAlgorithm.h"
#include <iostream>

BankerAlgorithm::BankerAlgorithm(int p, int r) : num_processes(p), num_resources(r),
allocate(p, std::vector<int>(r, 0)), maximum(p, std::vector<int>(r, 0)),
need(p, std::vector<int>(r, 0)), available(r, 0), finish(p, false) {
    std::cout << "Initialized with " << num_processes << " processes and " << num_resources << " resources." << std::endl;
}

void BankerAlgorithm::setAllocation(const std::vector<std::vector<int>>& alloc) {
    allocate = alloc;
    calculateNeed();
    std::cout << "Allocation matrix has been set." << std::endl;
}

void BankerAlgorithm::setMaximum(const std::vector<std::vector<int>>& max) {
    maximum = max;
    calculateNeed();
    std::cout << "Maximum matrix has been set and Need matrix calculated." << std::endl;
}

void BankerAlgorithm::setAvailable(const std::vector<int>& avail) {
    available = avail;
    std::cout << "Available resources have been updated." << std::endl;
}

void BankerAlgorithm::calculateNeed() {
    for (int i = 0; i < num_processes; ++i) {
        for (int j = 0; j < num_resources; ++j) {
            need[i][j] = maximum[i][j] - allocate[i][j];
        }
    }
    std::cout << "Need matrix has been calculated based on maximum and allocation matrices." << std::endl;
}

bool BankerAlgorithm::checkSafe() {
    std::vector<int> work(available);
    std::fill(finish.begin(), finish.end(), false);
    int count = 0;

    while (count < num_processes) {
        bool found = false;
        for (int i = 0; i < num_processes; ++i) {
            if (!finish[i]) {
                int j;
                for (j = 0; j < num_resources; ++j) {
                    if (need[i][j] > work[j]) {
                        break;
                    }
                }
                if (j == num_resources) { // If all needs are met
                    for (int k = 0; k < num_resources; k++) {
                        work[k] += allocate[i][k];
                    }
                    finish[i] = true;
                    found = true;
                    count++;
                    std::cout << "Process " << i << " can finish safely, releasing resources." << std::endl;
                }
            }
        }
        if (!found) {
            std::cout << "Failed to find a safe sequence after checking all processes." << std::endl;
            return false;
        }
    }
    return true;
}

void BankerAlgorithm::findSafeSequence() {
    std::cout << "Finding one safe sequence..." << std::endl;
    if (checkSafe()) {
        std::cout << "System is in a safe state. Safe Sequence: ";
        for (int i = 0; i < num_processes; ++i) {
            if (finish[i]) {
                std::cout << "P" << i << " ";
            }
        }
        std::cout << std::endl;
    } else {
        std::cout << "No safe sequence found." << std::endl;
    }
}

void BankerAlgorithm::findAllSafeSequences() {
    std::vector<int> path;
    std::vector<bool> visited(num_processes, false);
    std::vector<int> work = available;
    std::cout << "Finding all possible safe sequences..." << std::endl;
    findAllSafeSequencesUtil(0, path, visited, safeSequences, work);

    if (!safeSequences.empty()) {
        std::cout << "All Possible Safe Sequences:" << std::endl;
        for (auto &seq : safeSequences) {
            for (int id : seq) {
                std::cout << "P" << id << " ";
            }
            std::cout << std::endl;
        }
    } else {
        std::cout << "No safe sequences could be found." << std::endl;
    }
}

void BankerAlgorithm::findAllSafeSequencesUtil(int idx, std::vector<int>& path, std::vector<bool>& visited, std::vector<std::vector<int>>& allSequences, std::vector<int>& work) {
    if (path.size() == num_processes) {
        allSequences.push_back(path);
        return;
    }

    for (int i = 0; i < num_processes; ++i) {
        if (!visited[i]) {
            bool canProceed = true;
            for (int j = 0; j < num_resources; j++) {
                if (need[i][j] > work[j]) {
                    canProceed = false;
                    break;
                }
            }
            if (canProceed) {
                visited[i] = true;
                path.push_back(i);
                for (int j = 0; j < num_resources; j++) {
                    work[j] -= need[i][j];
                }

                findAllSafeSequencesUtil(idx + 1, path, visited, allSequences, work);

                // Backtrack
                for (int j = 0; j < num_resources; j++) {
                    work[j] += need[i][j];
                }
                path.pop_back();
                visited[i] = false;
            }
        }
    }
}

void BankerAlgorithm::allocateResources(int process, const std::vector<int>& request) {
    std::cout << "Attempting to allocate resources to process " << process << "..." << std::endl;
    bool validRequest = true;
    for (int i = 0; i < num_resources; ++i) {
        if (request[i] > need[process][i] || request[i] > available[i]) {
            validRequest = false;
            std::cout << "Allocation failed: Request exceeds needs or available resources for resource " << i << std::endl;
            break;
        }
    }

    if (validRequest) {
        for (int i = 0; i < num_resources; ++i) {
            available[i] -= request[i];
            allocate[process][i] += request[i];
            need[process][i] -= request[i];
        }
        if (!checkSafe()) {
            std::cout << "Allocation leaves system in unsafe state, rolling back." << std::endl;
            for (int i = 0; i < num_resources; ++i) {
                available[i] += request[i];
                allocate[process][i] -= request[i];
                need[process][i] += request[i];
            }
        } else {
            std::cout << "Resources allocated successfully." << std::endl;
        }
    }
}

void BankerAlgorithm::deallocateResources(int process, const std::vector<int>& release) {
    std::cout << "Deallocating resources from process " << process << "..." << std::endl;
    for (int i = 0; i < num_resources; ++i) {
        if (release[i] > allocate[process][i]) {
            std::cout << "Error: Trying to release more resources than allocated for resource " << i << std::endl;
            continue;
        }
        allocate[process][i] -= release[i];
        available[i] += release[i];
        need[process][i] += release[i];
    }
    std::cout << "Resources deallocated successfully." << std::endl;
}
