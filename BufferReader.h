#pragma once

#include <string>
#include <vector>

class IWordReader {
public:
  virtual std::string GetNext() = 0;
  virtual ~IWordReader() = default;
};

class TBufferReader : public IWordReader {
public:
  explicit TBufferReader(std::vector<char> buffer)
      : buffer_(std::move(buffer)), index_(0) {}

  std::string GetNext() override;

private:
  const std::vector<char> buffer_;
  size_t index_;
};