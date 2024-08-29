#include "BufferTaskSolver.h"

#include <iostream>

std::unordered_map<std::string, int> ChunkRead(const std::string &input_name,
                                               size_t threadNumber) {
  Timer timer("chunk solution");

  auto mapsMerger = std::make_shared<TConcurrentMapsMerger>();

  std::ifstream file(input_name, std::ifstream::ate | std::ifstream::binary);
  size_t fileSize = file.tellg();
  file.close();

  constexpr size_t maxChunkSize = 1024u * 1024 * 128; // 128 MB
  size_t chunkSize = std::min(maxChunkSize, fileSize / threadNumber);

  std::ifstream fin(input_name, std::ifstream::binary);

  constexpr unsigned long long maxRAMUsage = 1024ULL * 1024 * 1024 * 4; // 4 GB
  constexpr long long maxSimultaneousBuffersNumber = maxRAMUsage / maxChunkSize;

  TTasksDispatcher taskPool(threadNumber, maxSimultaneousBuffersNumber);

  while (!fin.eof()) {
    std::vector<char> buffer(chunkSize);
    fin.read(buffer.data(), buffer.size());

    std::streamsize s = fin.gcount();
    if (static_cast<size_t>(s) != chunkSize) {
      buffer.resize(s);
      if (s != 0) {
        auto threadReader = std::make_shared<TBufferReader>(std::move(buffer));
        auto job = std::make_shared<TWordsCounter>(threadReader, mapsMerger);
        taskPool.AddJob(job);
      }
      break;
    }
    if (!isTaskSpace(buffer.back())) {
      char c;
      while (!fin.eof() && fin.get(c)) {
        if (isTaskSpace(c)) {
          break;
        }
        buffer.emplace_back(c);
      }
    }

    auto threadReader = std::make_shared<TBufferReader>(std::move(buffer));
    auto job = std::make_shared<TWordsCounter>(threadReader, mapsMerger);
    taskPool.AddJob(job);
  }

  taskPool.Join();
  return mapsMerger->GetData();
}

void WriteAnswer(const std::unordered_map<std::string, int> &freq_map,
                 std::ofstream &ofile) {
  std::vector<std::pair<std::string, int>> items(freq_map.begin(),
                                                 freq_map.end());

  std::sort(items.begin(), items.end(),
            [](const std::pair<std::string, int> &a,
               const std::pair<std::string, int> &b) {
              if (a.second != b.second)
                return a.second > b.second;
              return a.first < b.first;
            });

  for (const auto &item : items)
    ofile << item.second << " " << item.first << '\n';
}

void TTaskSolver::Solve(const std::string &inputPath,
                        const std::string &outputPath) {

  std::ofstream outputFile(outputPath);

  if (!outputFile.is_open()) {
    throw std::runtime_error("Could not open output file!");
  }

  const auto freqMap = ChunkRead(inputPath, threadNumber_);
  WriteAnswer(freqMap, outputFile);

  outputFile.close();
}