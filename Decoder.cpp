#include "Decoder.h"
#include "Encoder.h"

int32_t bits_to_int(std::vector<bool> bits) {
    int32_t res = 0;
    for (size_t i = 0; i < bits.size(); ++i) {
        res |= bits[i] << i;
    }
    return res;
}

bool Haffman::Decoder::DecodeFile(BitReader &reader) {
    int32_t alphabet_size = bits_to_int(reader.ReadBits(Haffman::LETTER_SIZE));
    std::vector<std::pair<Haffman::Letter, Haffman::HaffmanCode>> codes;
    for (size_t i = 0; i < alphabet_size; ++i) {
        std::vector<bool> bits_of_char = reader.ReadBits(Haffman::LETTER_SIZE);
        Haffman::Letter l;
        for (size_t j = 0; j < Haffman::LETTER_SIZE; ++j) {
            l[j] = bits_of_char[j];
        }
        codes.push_back({l, {}});
    }
    Haffman::HaffmanTree haffman_tree;
    for (int32_t sum = 0, code_size = 1;; ++code_size) {
        int32_t cnt_codes_with_fixed_length = bits_to_int(reader.ReadBits(Haffman::LETTER_SIZE));
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
    Haffman::Encoder::MakeCanonicalHuffmanCodes(codes);
    for (auto&[letter, code]: codes) {
        haffman_tree.AddLetter(letter, code);
    }
    std::string file_name;
    for (;;) {
        Haffman::Letter letter = haffman_tree.GetNextLetter(reader);
        if (letter == Haffman::FILENAME_END) {
            break;
        }
        file_name.push_back(static_cast<unsigned char>(letter.to_ullong()));
    }
    std::ofstream ostream = std::ofstream(file_name, std::ios::binary);
    BitWriter writer(ostream);
    Haffman::Letter last_letter;
    for (;;) {
        Haffman::Letter letter = haffman_tree.GetNextLetter(reader);
        if (letter == Haffman::ONE_MORE_FILE || letter == Haffman::ARCHIVE_END) {
            last_letter = letter;
            break;
        }
        std::vector<bool> bits;
        for (size_t i = Haffman::LETTER_SIZE - 1; i > 0; --i) {
            bits.push_back(letter[Haffman::LETTER_SIZE - 1 - i]);
        }
        writer.WriteBits(bits);
    }
    writer.Close();
    std::cout << "Successfully decoded file " << file_name << std::endl;
    return last_letter == Haffman::ONE_MORE_FILE;
}

void Haffman::Decoder::DecodeArchive(std::ifstream &istream) {
    BitReader reader(istream);
    for (;;) {
        bool is_next_file = DecodeFile(reader);
        if (!is_next_file) {
            break;
        }
    }
    std::cout << "Archive decoded successfully." << std::endl;
}
