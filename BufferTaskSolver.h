#pragma once

#include "CountDispatcher.h"
#include "Timer.h"

#include <algorithm>
#include <fstream>
#include <string>
#include <unordered_map>

inline bool isTaskSpace(char c) {
    return !((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z'));
}

std::unordered_map<std::string, int>
ChunkRead(const std::string &input_name, size_t threadNumber);
void WriteAnswer(const std::unordered_map<std::string, int> &freq_map, std::ofstream &ofile);

class TTaskSolver {
public:
    explicit TTaskSolver(size_t threadNumber = 1) : threadNumber_(threadNumber) {}
    void Solve(const std::string &inputPath, const std::string &outputPath) {

        std::ofstream outputFile(outputPath);

        if (!outputFile.is_open()) {
            throw std::runtime_error("Could not open output file!");
        }

        const auto freqMap = ChunkRead(inputPath, threadNumber_);
        WriteAnswer(freqMap, outputFile);

        outputFile.close();
    }

private:
    const size_t threadNumber_;
};