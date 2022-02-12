#include <vector>
#include "HaffmanTree.h"

using Key = std::pair<size_t, Haffman::HaffmanTree *>;

class PriorityQueue {
public:

    PriorityQueue() = default;

    Key ExtractMin();

    size_t Size();

    void Insert(Key key);

private:
    std::vector<Key> heap;

    void SiftDown(size_t cur);

    void SiftUp(size_t cur);

    static size_t LeftChild(size_t n);

    static size_t RightChild(size_t n);

    static size_t Parent(size_t n);
};
