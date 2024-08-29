#include "WordsCounter.h"

void TConcurrentMapsMerger::AddValues(
    std::unordered_map<std::string, int> input) {
  std::lock_guard<std::mutex> lg(dataMutex_);
  for (const auto &pair : input) {
    freqMap_[pair.first] += pair.second;
  }
}

std::unordered_map<std::string, int> TConcurrentMapsMerger::GetData() {
  std::lock_guard<std::mutex> lg(dataMutex_);
  return freqMap_;
}

void TWordsCounter::CountWords() {
  std::unordered_map<std::string, int> localFreq;

  constexpr int fastSearchThreshold = 5;
  int dimension = static_cast<int>(std::pow(27, fastSearchThreshold));
  std::vector<int> wordVector(dimension, 0);

  while (true) {
    auto word = wordReader_->GetNext();
    if (word.empty())
      break;
    if (word.size() <= fastSearchThreshold) {
      size_t index = 0;
      size_t mult = 1;
      for (char c : word) {
        index += (c - 'a' + 1) * mult;
        mult *= 27;
      }
      ++wordVector[index];
    } else {
      localFreq[std::move(word)]++;
    }
  }

  for (size_t i = 0; i < wordVector.size(); ++i) {
    if (wordVector[i] == 0)
      continue;
    std::string currWord;
    size_t value = i;
    for (size_t j = 0; j < fastSearchThreshold; ++j) {
      char c = value % 27;
      if (c > 0)
        currWord += ('a' + c - 1);
      value /= 27;
    }
    localFreq[currWord] += wordVector[i];
  }
  mapsMerger_->AddValues(std::move(localFreq));
}
