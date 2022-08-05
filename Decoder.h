#include "BitReader.h"
#include "BitWriter.h"
#include "HuffmanTree.h"

namespace Huffman {
    class Decoder {
    public:
        void DecodeArchive(std::ifstream &istream);

    private:
        bool DecodeFile(BitReader &reader);
    };
}