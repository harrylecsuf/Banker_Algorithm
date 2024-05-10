#include <iostream>
#include <unistd.h>
#include <cstring>
#include <fstream>
#include "BankerAlgorithm.h"

int main() {
    int pipefd1[2], pipefd2[2];
    pid_t cpid;
    char buf[256];

    if (pipe(pipefd1) == -1 || pipe(pipefd2) == -1) {
        perror("pipe");
        exit(EXIT_FAILURE);
    }

    cpid = fork();
    if (cpid == -1) {
        perror("fork");
        exit(EXIT_FAILURE);
    }

    if (cpid == 0) { // Child process
        close(pipefd1[1]); // Close unused write end
        close(pipefd2[0]); // Close unused read end

        int num_processes, num_resources;
        read(pipefd1[0], &num_processes, sizeof(num_processes));
        read(pipefd1[0], &num_resources, sizeof(num_resources));

        BankerAlgorithm ba(num_processes, num_resources);
        std::vector<std::vector<int>> alloc(num_processes, std::vector<int>(num_resources));
        std::vector<std::vector<int>> max(num_processes, std::vector<int>(num_resources));
        std::vector<int> avail(num_resources);

        for (int i = 0; i < num_processes; ++i)
            read(pipefd1[0], alloc[i].data(), sizeof(int) * num_resources);
        for (int i = 0; i < num_processes; ++i)
            read(pipefd1[0], max[i].data(), sizeof(int) * num_resources);
        read(pipefd1[0], avail.data(), sizeof(int) * num_resources);

        ba.setAllocation(alloc);
        ba.setMaximum(max);
        ba.setAvailable(avail);
        ba.calculateNeed();

        bool safe = ba.checkSafe();
        if (safe) {
            std::vector<int> safe_sequence = ba.findSafeSequence();
            if (!safe_sequence.empty()) {
                write(pipefd2[1], "Safe state\n", strlen("Safe state\n"));
                for (int i = 0; i < safe_sequence.size(); ++i) {
                    snprintf(buf, sizeof(buf), "%d ", safe_sequence[i]);
                    write(pipefd2[1], buf, strlen(buf));
                }
                write(pipefd2[1], "\n", strlen("\n"));
            } else {
                const char* err_msg = "Oops! Looks like we're stuck in a deadlock!";
                write(pipefd2[1], err_msg, strlen(err_msg));
            }
        } else {
            const char* err_msg = "Oops! Looks like we're stuck in a deadlock!";
            write(pipefd2[1], err_msg, strlen(err_msg));
        }

        close(pipefd1[0]);
        close(pipefd2[1]);
        _exit(EXIT_SUCCESS);

    } else { // Parent process
        close(pipefd1[0]); // Close unused read end
        close(pipefd2[1]); // Close unused write end

        std::ifstream input("src/input.txt");
        if (!input.is_open()) {
            std::cerr << "Error: Unable to open input file!" << std::endl;
            exit(EXIT_FAILURE);
        }

        int num_processes, num_resources;
        input >> num_processes >> num_resources;

        std::vector<std::vector<int>> alloc(num_processes, std::vector<int>(num_resources));
        std::vector<std::vector<int>> max(num_processes, std::vector<int>(num_resources));
        std::vector<int> avail(num_resources);

        for (int i = 0; i < num_processes; ++i)
            for (int j = 0; j < num_resources; ++j)
                input >> alloc[i][j];
        for (int i = 0; i < num_processes; ++i)
            for (int j = 0; j < num_resources; ++j)
                input >> max[i][j];
        for (int i = 0; i < num_resources; ++i)
            input >> avail[i];

        std::cout << "Num Processes: " << num_processes << std::endl;
        std::cout << "Num Resources: " << num_resources << std::endl;
        std::cout << "Allocation Matrix:" << std::endl;
        for (const auto& row : alloc) {
            for (int cell : row) {
                std::cout << cell << " ";
            }
            std::cout << std::endl;
        }
        std::cout << "Max Matrix:" << std::endl;
        for (const auto& row : max) {
            for (int cell : row) {
                std::cout << cell << " ";
            }
            std::cout << std::endl;
        }
        std::cout << "Available Resources:" << std::endl;
        for (int cell : avail) {
            std::cout << cell << " ";
        }
        std::cout << std::endl;

        close(pipefd1[1]); // Close write end

        // Send data to child process
        write(pipefd1[1], &num_processes, sizeof(num_processes));
        write(pipefd1[1], &num_resources, sizeof(num_resources));
        for (int i = 0; i < num_processes; ++i)
            write(pipefd1[1], alloc[i].data(), sizeof(int) * num_resources);
        for (int i = 0; i < num_processes; ++i)
            write(pipefd1[1], max[i].data(), sizeof(int) * num_resources);
        write(pipefd1[1], avail.data(), sizeof(int) * num_resources);

        char child_output[256];
        ssize_t nbytes;
        while ((nbytes = read(pipefd2[0], child_output, sizeof(child_output))) > 0) {
            child_output[nbytes] = '\0';
            std::cout << child_output;
        }

        close(pipefd2[0]); // Close read end
        wait(NULL); // Wait for child
        exit(EXIT_SUCCESS);
    }
}
