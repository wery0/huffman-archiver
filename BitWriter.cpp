#include "BitWriter.h"

BitWriter::BitWriter(std::ostream &str) : stream(str) {

}

void BitWriter::PutCharFromBuf() {
    unsigned char c = 0;
    for (size_t j = 0; j < 8; ++j) {
        c |= (int32_t) buf[j] << j;
    }
    buf.clear();
    stream.put(c);
}

void BitWriter::WriteBits(std::vector<bool> bits) {
    for (size_t i = 0; i < bits.size(); ++i) {
        buf.push_back(bits[bits.size() - 1 - i]);
        if (buf.size() == 8) {
            PutCharFromBuf();
        }
    }
}

void BitWriter::Close() {
    if (!stream || buf.empty()) {
        return;
    }
    for (; buf.size() < 8;) {
        buf.push_back(0);
    }
    PutCharFromBuf();
    stream.flush();
}

BitWriter::~BitWriter() {
    Close();
}

