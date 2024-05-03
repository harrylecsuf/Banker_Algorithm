#include "BankerAlgorithm.h"

#include <vector>

using namespace std;

BankerAlgorithm::BankerAlgorithm() : num_processes(0), num_resources(0) {}

void BankerAlgorithm::initializeData(int processes, int resources) {
    num_processes = processes;
    num_resources = resources;
    allocation.resize(num_processes, vector<int>(num_resources, 0));
    max.resize(num_processes, vector<int>(num_resources, 0));
    need.resize(num_processes, vector<int>(num_resources, 0));
}

void BankerAlgorithm::setAllocation(const vector<vector<int>> &alloc) {
    allocation = alloc;
}

void BankerAlgorithm::setMax(const vector<vector<int>> &maximum) {
    max = maximum;
    for (int i = 0; i < num_processes; ++i) {
        for (int j = 0; j < num_resources; ++j) {
            need[i][j] = max[i][j] - allocation[i][j];
        }
    }
}

void BankerAlgorithm::setAvailable(const vector<int> &avail) {
    available = avail;
}

bool BankerAlgorithm::isSafe(vector<int> &work, vector<bool> &finish) {
    vector<int> tempWork = work;
    vector<bool> tempFinish = finish;

    for (int i = 0; i < num_processes; ++i) {
        if (!tempFinish[i]) {
            bool canAllocate = true;
            for (int j = 0; j < num_resources; ++j) {
                if (need[i][j] > tempWork[j]) {
                    canAllocate = false;
                    break;
                }
            }

            if (canAllocate) {
                for (int j = 0; j < num_resources; ++j) {
                    tempWork[j] += allocation[i][j];
                }
                tempFinish[i] = true;
                i = -1;
            }
        }
    }

    for (int i = 0; i < num_processes; ++i) {
        if (!tempFinish[i])
            return false;
    }

    return true;
}

bool BankerAlgorithm::isSafeState() {
    vector<int> work = available;
    vector<bool> finish(num_processes, false);
    return isSafe(work, finish);
}

vector<int> BankerAlgorithm::getSafeSequence() {
    vector<int> safeSeq;
    vector<int> work = available;
    vector<bool> finish(num_processes, false);

    while (true) {
        bool found = false;
        for (int i = 0; i < num_processes; ++i) {
            if (!finish[i]) {
                bool canAllocate = true;
                for (int j = 0; j < num_resources; ++j) {
                    if (need[i][j] > work[j]) {
                        canAllocate = false;
                        break;
                    }
                }

                if (canAllocate) {
                    for (int j = 0; j < num_resources; ++j) {
                        work[j] += allocation[i][j];
                    }
                    finish[i] = true;
                    safeSeq.push_back(i);
                    found = true;
                }
            }
        }
        if (!found)
            break;
    }

    return safeSeq;
}
