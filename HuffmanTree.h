#include <bitset>
#include "BitReader.h"

namespace Huffman {
    using Letter = std::bitset<9>;
    using HuffmanCode = std::vector<bool>;

    class HuffmanTree {
    public:

        explicit HuffmanTree(Huffman::Letter letter);

        HuffmanTree();

        static HuffmanTree *Merge(HuffmanTree *left, HuffmanTree *right);

        void AddLetter(const Huffman::Letter &letter,
                       Huffman::HuffmanCode code);

        Huffman::Letter GetNextLetter(BitReader &reader) const;

        std::vector<std::pair<Huffman::Letter, Huffman::HuffmanCode>> GetLettersWithCodes() const;

        ~HuffmanTree();

    private:
        struct Node {
          Huffman::Letter letter;
            Node *left = nullptr;
            Node *right = nullptr;

            Node() = default;

            explicit Node(Huffman::Letter l) {
                letter = l;
            }

            ~Node() {
                delete left;
                delete right;
            }
        };

        Node *root = nullptr;

        void dfs(Node *node, std::vector<std::pair<Huffman::Letter, Huffman::HuffmanCode>> &res,
            HuffmanCode &cur_code) const;
    };
}