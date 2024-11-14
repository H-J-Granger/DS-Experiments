#pragma once

#include "list.h"

#include <format>
#include <stdexcept>

namespace DATA_STRUCTURE {

template <class data_base>
class stack : public list<data_base> {
 public:
  stack() = default;
  ~stack() = default;

  bool empty() noexcept override { return top_index == 0; }

  void pop() override {
    if (empty()) [[unlikely]] {
      throw std::out_of_range(
          "DATA_STRUCTURE::stack::pop: Attempt to pop the top of an empty "
          "stack.");
    }
    top_index--;
    return;
  }

  data_base top() override {
    if (empty()) {
      throw std::out_of_range(
          "DATA_STRUCTURE::stack::top: Attempt to access the top of an empty "
          "stack.");
    }
    return list_data[top_index - 1];
  }

  void push(data_base push_data) override {
    if (top_index == MAX_LINK_LIST_NUM) [[unlikely]] {
      throw std::length_error(
          std::format("DATA_STRUCTURE::stack::push: Attempt to exceed the "
                      "maxmium length MAX_LINK_LIST_NUM ({}).",
                      MAX_LINK_LIST_NUM));
    }
    list_data[top_index] = push_data;
    top_index++;
    return;
  }

  void clear() noexcept override {
    top_index = 0;
    return;
  }

 private:
  // index range: [0, top_index)
  int top_index = 0;
  data_base list_data[MAX_LINK_LIST_NUM];
};

}  // namespace DATA_STRUCTURE
