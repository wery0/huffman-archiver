#include <fstream>
#include "BitReader.h"

BitReader::BitReader(std::istream &str) : stream(str) {

}

bool BitReader::CanReadBits(size_t cnt) {
    for (; !stream.eof() && buf.size() < cnt;) {
        char c;
        stream.get(c);
        if (stream.eof()) {
            break;
        }
        for (size_t i = 0; i < 8; ++i) {
            buf.push_back((c >> i) & 1);
        }
    }
    return buf.size() >= cnt;
}

std::vector<bool> BitReader::ReadBits(size_t cnt) {
    if (!CanReadBits(cnt)) {
        throw std::length_error("Can't read " + std::to_string(cnt) + " bits, because file has ended.");
    }
    std::vector<bool> res(cnt);
    std::copy(buf.begin(), buf.begin() + cnt, res.begin());
    buf.erase(buf.begin(), buf.begin() + cnt);
    return res;
}

BitReader::~BitReader() {

}
