// BankerAlgorithm.h
#ifndef BANKER_ALGORITHM_H
#define BANKER_ALGORITHM_H

#include <vector>
#include <iostream>

class BankerAlgorithm {
private:
    int num_processes, num_resources;
    std::vector<std::vector<int>> allocate;
    std::vector<std::vector<int>> maximum;
    std::vector<std::vector<int>> need;
    std::vector<int> available;
    std::vector<bool> finish;
    std::vector<std::vector<int>> safeSequences;

    void findAllSafeSequencesUtil(int idx, std::vector<int>& path, std::vector<bool>& visited, std::vector<std::vector<int>>& allSequences, std::vector<int>& work);

public:
    BankerAlgorithm(int p, int r);
    void setAllocation(const std::vector<std::vector<int>>& alloc);
    void setMaximum(const std::vector<std::vector<int>>& max);
    void setAvailable(const std::vector<int>& avail);
    void calculateNeed();
    bool checkSafe();
    void findSafeSequence();
<<<<<<< Updated upstream
    std::vector<std::unordered_set<int>> findAllPossibleSafeSequences();

    void BankerAlgorithm::findAllPossibleSafeSequencesUtil(int process, std::vector<bool>& visited,
                                                        std::vector<int>& seq,
                                                        std::vector<std::unordered_set<int>>& all_sequences);

=======
    void findAllSafeSequences();
>>>>>>> Stashed changes
    void allocateResources(int process, const std::vector<int>& request);
    void deallocateResources(int process, const std::vector<int>& release);
};

#endif
