/* This conio.h file is from https://stackoverflow.com/a/23035044 */

#include <termios.h>
#include <unistd.h>
#include <stdio.h>
#include <random>
#include <iostream>

int update_row(std::vector<int>& vec) {
    int sum = 0;
    std::vector<int> tmp;
    while (true) {
        tmp.clear();
        // slide
        for (auto val : vec) {
            if (val) {
                tmp.push_back(val);
            }
        }
        // merge
        tmp.resize(vec.size());
        vec = tmp;
        bool flag = false;
        for (int i = 0; i < static_cast<int>(vec.size()) - 1; ++i) {
            if (vec[i] != 0 && vec[i] == vec[i + 1]) {
                vec[i] *= 2;
                vec[i + 1] = 0;
                sum += vec[i];
                flag = true;
            }
        }
        if (!flag) {  // 'merge' did nothing.
            break;
        }
    }
    return sum;
}

int main() {
  std::printf("\033[%dmHello world!\033[0m\n", 32);return 0;

  std::vector<int> vec;
  for (int i = 0; i < 4; ++i) {
    int n;
    std::cin >> n;
    vec.push_back(n);
  }
  int sum = update_row(vec);
  for (auto val : vec) {
    std::cout << val << " ";
  }
  std::cout << std::endl << sum << std::endl;
  return 0;
}