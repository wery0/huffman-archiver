#include "PriorityQueue.h"

void PriorityQueue::SiftDown(size_t cur) {
    for (; LeftChild(cur) < heap.size();) {
        size_t left = LeftChild(cur);
        size_t right = RightChild(cur);
        size_t swap_with = left;
        if (right < heap.size() && heap[right] < heap[left]) {
            swap_with = right;
        }
        if (heap[cur] < heap[swap_with]) {
            break;
        }
        swap(heap[cur], heap[swap_with]);
        cur = swap_with;
    }
}

void PriorityQueue::SiftUp(size_t cur) {
    for (; cur > 0 && heap[cur] < heap[(Parent(cur))];) {
        swap(heap[cur], heap[Parent(cur)]);
        cur = Parent(cur);
    }
}

Key PriorityQueue::ExtractMin() {
    Key res = heap[0];
    heap[0] = heap.back();
    heap.pop_back();
    SiftDown(0);
    return res;
}

void PriorityQueue::Insert(Key key) {
    heap.push_back(key);
    SiftUp(heap.size() - 1);
}

size_t PriorityQueue::Size() {
    return heap.size();
}

size_t PriorityQueue::LeftChild(size_t n) {
    return n * 2 + 1;
}

size_t PriorityQueue::RightChild(size_t n) {
    return n * 2 + 2;
}

size_t PriorityQueue::Parent(size_t n) {
    return (n - 1) / 2;
}
