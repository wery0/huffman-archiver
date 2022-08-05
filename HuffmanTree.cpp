#include "HuffmanTree.h"
#include <algorithm>

Huffman::HuffmanTree::HuffmanTree(Huffman::Letter letter) {
    root = new Node(letter);
}

Huffman::HuffmanTree::HuffmanTree() {
    root = new Node();
}

Huffman::HuffmanTree *Huffman::HuffmanTree::Merge(HuffmanTree *left,
                                                  HuffmanTree *right) {
  HuffmanTree *new_tree = new HuffmanTree();
    new_tree->root->left = left->root;
    new_tree->root->right = right->root;
    return new_tree;
}

void Huffman::HuffmanTree::dfs(Node *node, std::vector<std::pair<Huffman::Letter, Huffman::HuffmanCode>> &codes,
    HuffmanCode &cur_code) const {
    if (node->left == nullptr) {
        codes.push_back({node->letter, cur_code});
        return;
    }
    cur_code.push_back(0);
    dfs(node->left, codes, cur_code);
    cur_code.pop_back();

    cur_code.push_back(1);
    dfs(node->right, codes, cur_code);
    cur_code.pop_back();
}

std::vector<std::pair<Huffman::Letter, Huffman::HuffmanCode>>
Huffman::HuffmanTree::GetLettersWithCodes() const {
    std::vector<std::pair<Huffman::Letter, Huffman::HuffmanCode>> codes;
    HuffmanCode cur_code;
    dfs(root, codes, cur_code);
    std::sort(codes.begin(), codes.end(),
              [](const std::pair<Huffman::Letter, Huffman::HuffmanCode> &p1,
                 const std::pair<Huffman::Letter, Huffman::HuffmanCode> &p2) {
                  return std::make_pair(p1.second.size(), p1.first.to_ullong()) <
                         std::make_pair(p2.second.size(), p2.first.to_ullong());
              });
    return codes;
}

void Huffman::HuffmanTree::AddLetter(const Huffman::Letter &letter,
                                     Huffman::HuffmanCode code) {
    Node *cur_node = root;
    for (const bool bit: code) {
        if (cur_node->left == nullptr && !bit) {
            cur_node->left = new Node();
        }
        if (cur_node->right == nullptr && bit) {
            cur_node->right = new Node();
        }
        cur_node = bit ? cur_node->right : cur_node->left;
    }
    cur_node->letter = letter;
}

Huffman::Letter Huffman::HuffmanTree::GetNextLetter(BitReader &reader) const {
    Node *cur_node = root;
    for (;;) {
        bool bit = reader.ReadBits(1)[0];
        cur_node = bit ? cur_node->right : cur_node->left;
        bool no_left = cur_node->left == nullptr;
        bool no_right = cur_node->right == nullptr;
        if (no_left != no_right) {
            throw std::invalid_argument("Vertex of haffman tree has 1 descendant. Archive is incorrect.");
        }
        if (no_left && no_right) {
            return cur_node->letter;
        }
    }
}

Huffman::HuffmanTree::~HuffmanTree() {
    delete root;
}
