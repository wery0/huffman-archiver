#include <algorithm>
#include <bitset>
#include "BitWriter.h"

namespace Haffman {

    const size_t LETTER_SIZE = 9;

    using Letter = std::bitset<LETTER_SIZE>;
    using HaffmanCode = std::vector<bool>;

    const Letter FILENAME_END = 256;
    const Letter ONE_MORE_FILE = 257;
    const Letter ARCHIVE_END = 258;

    class Encoder {
    public:

        explicit Encoder(std::ofstream &stream);

        static void MakeCanonicalHuffmanCodes(std::vector<std::pair<Haffman::Letter, Haffman::HaffmanCode>> &codes);

        void EncodeFile(std::string file_name);

        void Close();

    private:
        int32_t cnt_encoded_files = 0;
        BitWriter writer;
        Haffman::HaffmanCode ONE_MORE_FILE_;
        Haffman::HaffmanCode ARCHIVE_END_;
    };
}