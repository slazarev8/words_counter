#pragma once

#include "BufferReader.h"
#include <memory>
#include <mutex>
#include <unordered_map>
#include <valarray>

class TConcurrentMapsMerger {
public:
  void AddValues(std::unordered_map<std::string, int> input);

  std::unordered_map<std::string, int> GetData();

private:
  std::unordered_map<std::string, int> freqMap_;
  std::mutex dataMutex_;
};

class TWordsCounter {
public:
  TWordsCounter(std::shared_ptr<IWordReader> wordReader,
                std::shared_ptr<TConcurrentMapsMerger> merger)
      : wordReader_(std::move(wordReader)), mapsMerger_(std::move(merger)) {}
  TWordsCounter() = delete;
  void CountWords();

private:
  std::shared_ptr<IWordReader> wordReader_;
  std::shared_ptr<TConcurrentMapsMerger> mapsMerger_;
};
