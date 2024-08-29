#include "BufferTaskSolver.h"

#include <iostream>

int main(int argc, char **argv) {
    if (argc != 3 && argc != 4) {
        std::cout << "Invalid number of arguments! \nUsage: [input_file] [output_file] [optional:thread_number]" << std::endl;
        return 1;
    }
    size_t threadNumber = std::thread::hardware_concurrency();
    if (argc == 4) {
        threadNumber = std::stoul(argv[3]);
    }

    std::cout << "Use " + std::to_string(threadNumber) << " threads" << std::endl;

    TTaskSolver solver(threadNumber);
    solver.Solve(argv[1], argv[2]);

    return 0;
}