#ifndef BANKERALGORITHM_H
#define BANKERALGORITHM_H

#include <iostream>
#include <vector>
#include <fstream>
#include <unistd.h>
#include <sys/wait.h>
#include <cstring>

class BankerAlgorithm {
private:
    std::vector<std::vector<int>> allocate;
    std::vector<std::vector<int>> maximum;
    std::vector<std::vector<int>> need;
    std::vector<int> available;
    int num_processes, num_resources;

public:
    BankerAlgorithm(int p, int r) : num_processes(p), num_resources(r), 
                                    allocate(p, std::vector<int>(r, 0)), 
                                    maximum(p, std::vector<int>(r, 0)), 
                                    need(p, std::vector<int>(r, 0)), 
                                    available(r, 0) {}

    void setAllocation(const std::vector<std::vector<int>>& alloc) {
        allocate = alloc;
    }

    void setMaximum(const std::vector<std::vector<int>>& max) {
        maximum = max;
    }

    void setAvailable(const std::vector<int>& avail) {
        available = avail;
    }

    void calculateNeed() {
        for (int i = 0; i < num_processes; ++i)
            for (int j = 0; j < num_resources; ++j)
                need[i][j] = maximum[i][j] - allocate[i][j];
    }

    bool checkSafe() {
        std::vector<int> work = available;
        std::vector<bool> finish(num_processes, false);
        bool found = true;
        while (found) {
            found = false;
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
                    }
                }
            }
        }

        for (bool f : finish)
            if (!f)
                return false;
        return true;
    }
};

#endif 
