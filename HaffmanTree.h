#include <bitset>
#include "BitReader.h"

namespace Haffman {
    using Letter = std::bitset<9>;
    using HaffmanCode = std::vector<bool>;

    class HaffmanTree {
    public:

        explicit HaffmanTree(Haffman::Letter letter);

        HaffmanTree();

        static HaffmanTree *Merge(HaffmanTree *left, HaffmanTree *right);

        void AddLetter(const Haffman::Letter &letter, Haffman::HaffmanCode code);

        Haffman::Letter GetNextLetter(BitReader &reader) const;

        std::vector<std::pair<Haffman::Letter, Haffman::HaffmanCode>> GetLettersWithCodes() const;

        ~HaffmanTree();

    private:
        struct Node {
            Haffman::Letter letter;
            Node *left = nullptr;
            Node *right = nullptr;

            Node() = default;

            explicit Node(Haffman::Letter l) {
                letter = l;
            }

            ~Node() {
                delete left;
                delete right;
            }
        };

        Node *root = nullptr;

        void dfs(Node *node, std::vector<std::pair<Haffman::Letter, Haffman::HaffmanCode>> &res,
                 HaffmanCode &cur_code) const;
    };
}