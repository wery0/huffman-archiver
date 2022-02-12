#pragma once

#include <iostream>
#include <vector>

class BitReader {
public:

    explicit BitReader(std::istream &str);

    std::vector<bool> ReadBits(size_t cnt);

    bool CanReadBits(size_t cnt);

    ~BitReader();

private:
    std::istream &stream;
    std::vector<bool> buf;
};