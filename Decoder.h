#include "BitReader.h"
#include "BitWriter.h"
#include "HaffmanTree.h"

namespace Haffman {
    class Decoder {
    public:
        void DecodeArchive(std::ifstream &istream);

    private:
        bool DecodeFile(BitReader &reader);
    };
}