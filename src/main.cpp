
#include "BankerAlgorithm.h"

using namespace std;

int main() {
    int pipefd1[2], pipefd2[2];
    pid_t cpid;
    char buf;

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
        const char *msg = safe ? "Safe state\n" : "Deadlock\n";
        write(pipefd2[1], msg, strlen(msg));

        close(pipefd1[0]);
        close(pipefd2[1]);
        _exit(EXIT_SUCCESS);

    } else { // Parent process
        close(pipefd1[0]); // Close unused read end
        close(pipefd2[1]); // Close unused write end

        // Example data setup
        int num_processes = 5;
        int num_resources = 3;
        write(pipefd1[1], &num_processes, sizeof(num_processes));
        write(pipefd1[1], &num_resources, sizeof(num_resources));

        std::vector<std::vector<int>> alloc = {{0, 1, 0}, {2, 0, 0}, {3, 0, 2}, {2, 1, 1}, {0, 0, 2}};
        std::vector<std::vector<int>> max = {{7, 5, 3}, {3, 2, 2}, {9, 0, 2}, {2, 2, 2}, {4, 3, 3}};
        std::vector<int> avail = {3, 3, 2};

        for (int i = 0; i < num_processes; ++i)
            write(pipefd1[1], alloc[i].data(), sizeof(int) * num_resources);
        for (int i = 0; i < num_processes; ++i)
            write(pipefd1[1], max[i].data(), sizeof(int) * num_resources);
        write(pipefd1[1], avail.data(), sizeof(int) * num_resources);

        close(pipefd1[1]); // Reader will see EOF

        while (read(pipefd2[0], &buf, 1) > 0)
            write(STDOUT_FILENO, &buf, 1);

        close(pipefd2[0]); // Close read end
        wait(NULL); // Wait for child
        exit(EXIT_SUCCESS);
    }
}
