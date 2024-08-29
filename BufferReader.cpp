#include "BufferReader.h"

std::string TBufferReader::GetNext() {
  std::string res;
  constexpr auto toLowerShift = ('a' - 'A');
  while (index_ < buffer_.size()) {
    const char currChar = buffer_[index_++];
    if (currChar >= 'a' && currChar <= 'z') {
      res += currChar;
    } else if (currChar >= 'A' && currChar <= 'Z') {
      res += (currChar + toLowerShift);
    } else {
      if (!res.empty())
        return res;
    }
  }
  return res;
}