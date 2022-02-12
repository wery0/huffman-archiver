#include <map>
#include <unordered_map>
#include "Encoder.h"
#include "PriorityQueue.h"

Haffman::Encoder::Encoder(std::ofstream &stream) : writer(stream) {
}

std::ifstream CreateIfstream(std::string &file_name) {
    std::ifstream stream = std::ifstream(file_name, std::ios::binary);
    if (!stream) {
        throw std::invalid_argument("No such file " + file_name);
    }
    return stream;
}

std::vector<bool> LetterToBits(Haffman::Letter letter) {
    std::vector<bool> bits;
    for (size_t i = 0; i < Haffman::LETTER_SIZE; ++i) {
        bits.push_back(letter[Haffman::LETTER_SIZE - 1 - i]);
    }
    return bits;
}

void Haffman::Encoder::MakeCanonicalHuffmanCodes(std::vector<std::pair<Haffman::Letter, Haffman::HaffmanCode>> &codes) {
    fill(codes[0].second.begin(), codes[0].second.end(), false);
    for (size_t i = 0; i + 1 < codes.size(); ++i) {
        Haffman::HaffmanCode &next_code = codes[i + 1].second;
        size_t old_length = next_code.size();
        next_code = codes[i].second;
        int32_t last_zero = -1;
        for (size_t j = 0; j < next_code.size(); ++j) {
            last_zero = next_code[j] == 0 ? j : last_zero;
        }
        if (last_zero == -1) {
            next_code.insert(next_code.begin(), 0);
            last_zero = 0;
        }
        next_code[last_zero] = 1;
        fill(next_code.begin() + last_zero + 1, next_code.end(), 0);
        for (; next_code.size() < old_length;) {
            next_code.push_back(0);
        }
    }
}

void Haffman::Encoder::EncodeFile(std::string file_name) {
    if (cnt_encoded_files > 0) {
        writer.WriteBits(ONE_MORE_FILE_);
    }
    std::ifstream istream1 = CreateIfstream(file_name);
    BitReader reader(istream1);
    std::unordered_map<Haffman::Letter, int32_t> letter_count;
    letter_count[Haffman::FILENAME_END] = 1;
    letter_count[Haffman::ONE_MORE_FILE] = 1;
    letter_count[Haffman::ARCHIVE_END] = 1;
    for (unsigned char c: file_name) {
        ++letter_count[c];
    }
    for (; reader.CanReadBits(8);) {
        std::vector<bool> byte = reader.ReadBits(8);
        Haffman::Letter letter;
        for (size_t i = 0; i < 8; ++i) {
            letter[i] = byte[7 - i];
        }
        ++letter_count[letter];
    }
    PriorityQueue pq;
    for (const auto&[letter, cnt]: letter_count) {
        pq.Insert({cnt, new Haffman::HaffmanTree(letter)});
    }
    for (; pq.Size() > 1;) {
        auto[cnt1, tree1] = pq.ExtractMin();
        auto[cnt2, tree2] = pq.ExtractMin();
        pq.Insert({cnt1 + cnt2, Haffman::HaffmanTree::Merge(tree1, tree2)});
    }
    Key remain = pq.ExtractMin();
    std::vector<std::pair<Haffman::Letter, Haffman::HaffmanCode>> codes = remain.second->GetLettersWithCodes();
    MakeCanonicalHuffmanCodes(codes);
    delete remain.second;
    std::unordered_map<Haffman::Letter, Haffman::HaffmanCode> codes_map;
    for (auto[letter, code]: codes) {
        std::reverse(code.begin(), code.end());
        codes_map[letter] = code;
    }
    Haffman::Letter SYMBOLS_COUNT = codes.size();
    writer.WriteBits(LetterToBits(SYMBOLS_COUNT));
    size_t max_code_size = 0;
    std::map<size_t, int32_t> code_sizes_count;
    for (auto&[letter, code]: codes) {
        ++code_sizes_count[code.size()];
        max_code_size = std::max(max_code_size, code.size());
        writer.WriteBits(LetterToBits(letter));
    }
    for (size_t i = 1; i <= max_code_size; ++i) {
        if (code_sizes_count.count(i) == 0) {
            code_sizes_count[i] = 0;
        }
    }
    for (const auto&[size, cnt]: code_sizes_count) {
        writer.WriteBits(LetterToBits(cnt));
    }
    for (unsigned char c: file_name) {
        writer.WriteBits(codes_map[c]);
    }
    writer.WriteBits(codes_map[Haffman::FILENAME_END]);
    std::ifstream istream2 = CreateIfstream(file_name);
    BitReader reader2(istream2);
    for (; reader2.CanReadBits(8);) {
        std::vector<bool> byte = reader2.ReadBits(8);
        Haffman::Letter l;
        for (size_t i = 0; i < 8; ++i) {
            l[i] = byte[7 - i];
        }
        writer.WriteBits(codes_map[l]);
    }
    ARCHIVE_END_ = codes_map[Haffman::ARCHIVE_END];
    ONE_MORE_FILE_ = codes_map[Haffman::ONE_MORE_FILE];
    std::cout << "Successfully encode file " << file_name << std::endl;
    ++cnt_encoded_files;
}

void Haffman::Encoder::Close() {
    writer.WriteBits(ARCHIVE_END_);
    writer.Close();
}
