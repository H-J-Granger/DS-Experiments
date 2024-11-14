#include <iostream>

#include "calculator.h"

int main() {
#if 0
  DATA_STRUCTURE::calculator a;
  if (a.legal()) {
    std::cout << a.get_expr() << " = ";
    DATA_STRUCTURE::calculator::element tmp = a.get_ans();
    if (tmp.flag == 0)
      std::cout << tmp.num_int << std::endl;
    else
      std::cout << tmp.num_double << std::endl;
  }
  DATA_STRUCTURE::calculator b("3^2");
  if (b.legal()) {
    std::cout << b.get_expr() << " = ";
    DATA_STRUCTURE::calculator::element tmp = b.get_ans();
    if (tmp.flag == 0)
      std::cout << tmp.num_int << std::endl;
    else
      std::cout << tmp.num_double << std::endl;
  }
#endif

  while (1) {
    std::string s;
    std::cin >> s;
    if (s[0] == 'q')
      break;
    DATA_STRUCTURE::calculator b(s);
    DATA_STRUCTURE::calculator::element tmp;
    if (!b.legal()) {
      std::cout << "Illegal input: " << s << std::endl;
      continue;
    }
    try {
      tmp = b.get_ans();
      std::cout << b.get_expr() << " = ";
      if (tmp.flag == 0)
        std::cout << tmp.num_int << std::endl;
      else
        std::cout << tmp.num_double << std::endl;
    } catch (std::exception const& e) {
      std::cout << e.what() << std::endl;
    }
  }
  return 0;
}
