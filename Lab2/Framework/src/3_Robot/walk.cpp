#include "walk.h"

#include <algorithm>
#include <cmath>
#include <vector>

namespace DATA_STRUCTURE {

void walk::print_para() {
  std::cout << steps << std::endl;
  for (int i = 0; i < gold_num; i++) {
    std::cout << *(gold + 2 * i) << " " << *(gold + 2 * i + 1) << " "
              << *(gold_value + i) << std::endl;
  }
}

int walk::compute_distance(int i, int x, int y) {
  int deltax = std::abs(x - gold[2 * i]);
  int deltay = std::abs(y - gold[2 * i + 1]);
  return deltax - (deltax / 2) + deltay - (deltay / 2);
}

int walk::get_value() {
  int max_value = 0;
  q.push({0, 0, 0, 0, unordered_set<int>()});
  while (!q.empty()) {
    auto [step, value, x, y, got] = q.top();
    max_value = std::max(max_value, value);
    q.pop();
    for (int i = 0; i < gold_num; ++i) {
      if (got.find(i)) {
        continue;
      }
      auto got2 = got;
      auto value2 = value;
      auto step2 = step + compute_distance(i, x, y);
      if (step2 > steps) {
        continue;
      }
      for (int j = 0; j < gold_num; ++j) {
        if (got.find(j)) {
          continue;
        }
        if (gold[2 * j] == gold[2 * i] && gold[2 * j + 1] == gold[2 * i + 1]) {
          got2.insert(j);
          value2 += gold_value[j];
        }
      }
      q.push({step2, value2, gold[2 * i], gold[2 * i + 1], got2});
    }
  }
  return max_value;
}

}  // namespace DATA_STRUCTURE