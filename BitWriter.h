#pragma once

#include <iostream>
#include <fstream>
#include <vector>

class BitWriter {
public:

    explicit BitWriter(std::ostream &str);

    void WriteBits(std::vector<bool> bits);

    void Close();

    ~BitWriter();

private:
    std::ostream &stream;
    std::vector<bool> buf;

    void PutCharFromBuf();
};