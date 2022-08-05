#include "Decoder.h"
#include "Encoder.h"

int32_t bits_to_int(std::vector<bool> bits) {
    int32_t res = 0;
    for (size_t i = 0; i < bits.size(); ++i) {
        res |= bits[i] << i;
    }
    return res;
}

bool Huffman::Decoder::DecodeFile(BitReader &reader) {
    int32_t alphabet_size = bits_to_int(reader.ReadBits(Huffman::LETTER_SIZE));
    std::vector<std::pair<Huffman::Letter, Huffman::HuffmanCode>> codes;
    for (size_t i = 0; i < alphabet_size; ++i) {
        std::vector<bool> bits_of_char = reader.ReadBits(Huffman::LETTER_SIZE);
        Huffman::Letter l;
        for (size_t j = 0; j < Huffman::LETTER_SIZE; ++j) {
            l[j] = bits_of_char[j];
        }
        codes.push_back({l, {}});
    }
    Huffman::HuffmanTree huffman_tree;
    for (int32_t sum = 0, code_size = 1;; ++code_size) {
        int32_t cnt_codes_with_fixed_length = bits_to_int(reader.ReadBits(Huffman::LETTER_SIZE));
        sum += cnt_codes_with_fixed_length;
        if (sum > alphabet_size) {
            throw std::invalid_argument("Sum of codes_with_fixed_length != alphabet_size. Archive is incorrect");
        }
        for (size_t i = sum - cnt_codes_with_fixed_length; i < sum; ++i) {
            codes[i].second = std::vector<bool>(code_size);
        }
        if (sum == alphabet_size) {
            break;
        }
    }
    Huffman::Encoder::MakeCanonicalHuffmanCodes(codes);
    for (auto&[letter, code]: codes) {
      huffman_tree.AddLetter(letter, code);
    }
    std::string file_name;
    for (;;) {
      Huffman::Letter letter = huffman_tree.GetNextLetter(reader);
        if (letter == Huffman::FILENAME_END) {
            break;
        }
        file_name.push_back(static_cast<unsigned char>(letter.to_ullong()));
    }
    std::ofstream ostream = std::ofstream(file_name, std::ios::binary);
    BitWriter writer(ostream);
    Huffman::Letter last_letter;
    for (;;) {
      Huffman::Letter letter = huffman_tree.GetNextLetter(reader);
        if (letter == Huffman::ONE_MORE_FILE || letter == Huffman::ARCHIVE_END) {
            last_letter = letter;
            break;
        }
        std::vector<bool> bits;
        for (size_t i = Huffman::LETTER_SIZE - 1; i > 0; --i) {
            bits.push_back(letter[Huffman::LETTER_SIZE - 1 - i]);
        }
        writer.WriteBits(bits);
    }
    writer.Close();
    std::cout << "Successfully decoded file " << file_name << std::endl;
    return last_letter == Huffman::ONE_MORE_FILE;
}

void Huffman::Decoder::DecodeArchive(std::ifstream &istream) {
    BitReader reader(istream);
    for (;;) {
        bool is_next_file = DecodeFile(reader);
        if (!is_next_file) {
            break;
        }
    }
    std::cout << "Archive decoded successfully." << std::endl;
}
