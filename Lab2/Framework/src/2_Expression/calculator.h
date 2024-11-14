#include <math.h>
#include <string>

#include "stack.h"

namespace DATA_STRUCTURE {

class calculator {
 public:
  struct element {
    int flag;  // flag = 0 => number has the type of int, else double
    int num_int;
    double num_double;
  };

 public:
  calculator() = default;

  calculator(std::string _expr) { expr = _expr; }

  ~calculator() = default;

  std::string get_expr();
  element get_ans();
  bool legal() noexcept;

 private:
  std::string expr = "1+2*(3-1+4)";
  int expr_index = 0;
  stack<element> num;
  stack<char> op;

  element read_num();
  int priority_regular(char c);
  int priority(char c1,
               char c2) noexcept(noexcept(calculator::priority_regular));
  element operate(element element1, char c, element element2);
};

}  // namespace DATA_STRUCTURE
