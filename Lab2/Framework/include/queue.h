#pragma once

#include "list.h"

#include <format>
#include <stdexcept>

namespace DATA_STRUCTURE {

template <class data_base>
class queue : public list<data_base> {
 public:
  queue() = default;
  ~queue() = default;

  bool empty() noexcept override { return top_index == tail_index; }

  void pop() override {
    if (empty()) [[unlikely]] {
      throw std::out_of_range(
          "DATA_STRUCTURE::queue::pop: Attempt to pop the top of an empty "
          "queue.");
    }
    top_index++;
    if (top_index == MAX_LINK_LIST_NUM) [[unlikely]] {
      top_index = 0;
    }
    return;
  }

  data_base top() override {
    if (empty()) [[unlikely]] {
      throw std::out_of_range(
          "DATA_STRUCTURE::queue::top: Attempt to access the top of an empty "
          "queue.");
    }
    return list_data[top_index];
  }

  void push(data_base push_data) override {
    if (tail_index + 1 == top_index ||
        (tail_index == MAX_LINK_LIST_NUM - 1 && top_index == 0)) [[unlikely]] {
      throw std::length_error(
          std::format("DATA_STRUCTURE::queue::push: Attempt to exceed the "
                      "maxmium length MAX_LINK_LIST_NUM-1 ({}).",
                      MAX_LINK_LIST_NUM - 1));
    }
    list_data[tail_index] = push_data;
    tail_index++;
    if (tail_index == MAX_LINK_LIST_NUM) [[unlikely]] {
      tail_index = 0;
    }
    return;
  }

  void clear() noexcept override {
    tail_index = top_index = 0;
    return;
  }

 private:
  // index range: [top_index, tail_index) (modular MAX_LINK_LIST_NUM)
  int top_index = 0;
  int tail_index = 0;
  data_base list_data[MAX_LINK_LIST_NUM];
};

}  // namespace DATA_STRUCTURE
