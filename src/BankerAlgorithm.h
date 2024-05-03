#ifndef BANKER_ALGORITHM_H
#define BANKER_ALGORITHM_H

#include <vector>

class BankerAlgorithm {
private:
    std::vector<std::vector<int>> allocate;
    std::vector<std::vector<int>> maximum;
    std::vector<std::vector<int>> need;
    std::vector<int> available;
    int num_processes, num_resources;

public:
    BankerAlgorithm(int p, int r);

    void setAllocation(const std::vector<std::vector<int>>& alloc);
    void setMaximum(const std::vector<std::vector<int>>& max);
    void setAvailable(const std::vector<int>& avail);
    void calculateNeed();
    bool checkSafe();
};

#endif
