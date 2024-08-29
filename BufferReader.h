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
    explicit TBufferReader(std::vector<char> buffer) : buffer_(std::move(buffer)), index_(0) {}

    std::string GetNext() override {
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

private:
    const std::vector<char> buffer_;
    size_t index_;
};