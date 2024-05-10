#include <algorithm>
#include <iostream>
#include <math.h>
#include <vector>

template <typename T> class Heap {
  std::vector<T> heap;

  int left(int i) { return 2 * i + 1; }
  int right(int i) { return 2 * i + 2; }

  void min_heapify(int at) {
    std::size_t l = left(at);
    std::size_t r = right(at);
    std::size_t largest = at;
    if (l < heap.size() && heap[largest] > heap[l])
      largest = l;
    if (r < heap.size() && heap[largest] > heap[r])
      largest = r;
    if (largest != at) {
      std::swap(heap[at], heap[largest]);
      min_heapify(largest);
    }
  }

  void build_min_heap() {
    for (int i = (heap.size() / 2) - 1; i >= 0; --i) {
      min_heapify(i);
    }
  }

public:
  using Const_heap_iter = typename std::vector<T>::const_iterator;
  using Heap_iter = typename std::vector<T>::iterator;

  Heap(std::initializer_list<T> const &list) : heap(list) { build_min_heap(); }
  Heap() = default;

  Const_heap_iter cbegin() const { return heap.cbegin(); }
  Const_heap_iter cend() const { return heap.cend(); }
  Heap_iter begin() { return heap.begin(); }
  Heap_iter end() { return heap.end(); }
  bool empty() const { return heap.empty(); }
  std::size_t size() const { return heap.size(); }
  T const &top() const { return heap.front(); }
  T pop();
  void update();
  void push(T const &);
  Heap_iter find(T const &);
  int heap_height() const;
  void print_heap() const;
};

template <typename T> T Heap<T>::pop() {
  T rv = heap.front();
  heap.front() = heap.back();
  heap.pop_back();
  min_heapify(0);
  return rv;
}

template <typename T> void Heap<T>::push(T const &elem) {
  heap.push_back(elem);
  build_min_heap();
}

template <typename T> int Heap<T>::heap_height() const {
  return std::ceil(std::log2(heap.size()));
}

template <typename T> void Heap<T>::print_heap() const {
  for (int i = 0; i < heap_height(); ++i) {
    for (int j = std::pow(2, i) - 1, k = 0;
         k < std::pow(2, i) && j < heap.size(); ++j, ++k) {
      std::cout << heap[j] << " ";
    }
    std::cout << std::endl;
  }
}

template <typename T> typename Heap<T>::Heap_iter Heap<T>::find(T const &elem) {
  return std::find(heap.begin(), heap.end(), elem);
}

template <typename T> void Heap<T>::update() { build_min_heap(); }
