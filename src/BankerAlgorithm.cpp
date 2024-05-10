#include "BankerAlgorithm.h"

BankerAlgorithm::BankerAlgorithm(int p, int r) : num_processes(p), num_resources(r),
                                    allocate(p, std::vector<int>(r, 0)),
                                    maximum(p, std::vector<int>(r, 0)),
                                    need(p, std::vector<int>(r, 0)),
                                    available(r, 0),
                                    finish(p, false),
                                    work(p, std::vector<int>(r, 0)) {}

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
    for (int i = 0; i < num_resources; ++i)
        work[0][i] = available[i];

    for (int i = 0; i < num_processes; ++i)
        finish[i] = false;

    int count = 0;
    while (count < num_processes) {
        bool found = false;
        for (int i = 0; i < num_processes; ++i) {
            if (!finish[i]) {
                bool can_execute = true;
                for (int j = 0; j < num_resources; ++j) {
                    if (need[i][j] > work[i][j]) {
                        can_execute = false;
                        break;
                    }
                }
                if (can_execute) {
                    for (int j = 0; j < num_resources; ++j)
                        work[i + 1][j] = work[i][j] + allocate[i][j];
                    finish[i] = true;
                    found = true;
                    count++;
                }
            }
        }
        if (!found)
            break;
    }
    return count == num_processes;
}

void BankerAlgorithm::findSafeSequence() {
    safe_sequence.clear();
    if (checkSafe()) {
        for (int process = 0; process < num_processes; ++process) {
            if (finish[process])
                safe_sequence.insert(process);
        }
        for (int process : safe_sequence)
            std::cout << process << " ";
        std::cout << std::endl;
    } else {
        std::cout << "Deadlock" << std::endl;
    }
}

std::vector<std::unordered_set<int>> BankerAlgorithm::findAllPossibleSafeSequences() {
    std::vector<std::unordered_set<int>> all_sequences;
    std::vector<int> seq;
    std::vector<bool> visited(num_processes, false);

    findAllPossibleSafeSequencesUtil(0, visited, seq, all_sequences);
    return all_sequences;
}

void BankerAlgorithm::findAllPossibleSafeSequencesUtil(int process, std::vector<bool>& visited,
                                                        std::vector<int>& seq,
                                                        std::vector<std::unordered_set<int>>& all_sequences) {
    if (process == num_processes) {
        safe_sequence.clear();
        for (int p : seq)
            safe_sequence.insert(p);
        if (checkSafe()) {
            all_sequences.push_back(safe_sequence);
        }
        return;
    }

    for (int i = 0; i < num_processes; ++i) {
        if (!visited[i]) {
            visited[i] = true;
            seq.push_back(i);
            findAllPossibleSafeSequencesUtil(process + 1, visited, seq, all_sequences);
            seq.pop_back();
            visited[i] = false;
        }
    }
}

void BankerAlgorithm::allocateResources(int process, const std::vector<int>& request) {
    for (int i = 0; i < num_resources; ++i) {
        if (request[i] > available[i] || request[i] > need[process][i])
            return;
    }

    for (int i = 0; i < num_resources; ++i) {
        available[i] -= request[i];
        allocate[process][i] += request[i];
        need[process][i] -= request[i];
    }
}

void BankerAlgorithm::deallocateResources(int process, const std::vector<int>& release) {
    for (int i = 0; i < num_resources; ++i) {
        if (release[i] > allocate[process][i])
            return;
    }

    for (int i = 0; i < num_resources; ++i) {
        available[i] += release[i];
        allocate[process][i] -= release[i];
        need[process][i] += release[i];
    }
}
