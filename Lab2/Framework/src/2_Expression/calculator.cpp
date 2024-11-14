#include "calculator.h"

#include <algorithm>
#include <cctype>
#include <cmath>
#include <format>
#include <limits>
#include <stdexcept>
#include <utility>

namespace DATA_STRUCTURE {

std::string calculator::get_expr() {
  return expr;
}

/**
 * Checking if the expression is legal.
 * Note that there are ten illegal character pattern occurance, different from 
 * what the manual states: "\(\)", "([^0-9]).([^0-9])", "\((\+|-|\*|/|\^)", 
 * "(\+|-|\*|/|\^)\)", "([0-9]|.)\(", "\)([0-9]|.)", 
 * "(\+|-|\*|/|\^)(\+|-|\*|/|\^)", "\)\(", "^(\+|-|\*|/|\^)", "(\+|-|\*|/|\^)$".
*/
bool calculator::legal() noexcept {
  int bracket = 0;
  int len = expr.length();
  constexpr auto is_operator = [](unsigned char ch) {
    return ch == '+' || ch == '-' || ch == '*' || ch == '/' || ch == '^';
  };
  if (std::any_of(expr.cbegin(), expr.cend(), [](char ch) {
        return ch != '(' && ch != ')' && ch != '.' &&
               !std::isdigit(static_cast<unsigned char>(ch)) &&
               !is_operator(ch);
      })) {
    return false;
  }
  bool has_dot = false;  // Current number token already has a dot or not.
  for (int i = 0; i < len; ++i) {
    unsigned char ch = expr[i];
    if (i != len - 1) {
      unsigned char ch2 = expr[i + 1];
      if (ch == '(') [[unlikely]] {
        if (ch2 == ')' || is_operator(ch2)) {
          return false;  // '()' or '(+'
        }
      } else if (ch == ')') [[unlikely]] {
        if (ch2 == '(' || std::isdigit(ch2) || ch2 == '.') {
          return false;  // ')(' or ')0' or ').'
        }
      } else if (std::isdigit(ch) || ch == '.') {
        if (ch2 == '(') {
          return false;  // '0(' or '.('
        }
      } else {
        if (is_operator(ch2) || ch2 == ')') {
          return false;  // '++' or '+)'
        }
      }
    }
    if (ch == '.') [[unlikely]] {
      if (!(i != 0 && std::isdigit(static_cast<unsigned char>(expr[i - 1]))) &&
          !(i != len - 1 &&
            std::isdigit(static_cast<unsigned char>(expr[i + 1]))))
          [[unlikely]] {
        return false;  // a standalone '.' not attached to any digit
      }
      if (has_dot) {
        return false;  // two dots in a single number token
      }
      has_dot = true;
    } else {
      has_dot &= std::isdigit(static_cast<unsigned char>(expr[i]));
    }
    if (is_operator(ch)) {
      if (i == 0 || i == len - 1) [[unlikely]] {
        return false;  // an operator in the begin or the end
      }
    }
    if (ch == '(') [[unlikely]] {
      bracket++;
    }
    if (ch == ')') [[unlikely]] {
      bracket--;
    }
    if (bracket < 0) [[unlikely]] {
      return false;
    }
  }
  return bracket == 0;
}

int calculator::priority_regular(char c) {
  if (c == '+' || c == '-') {
    return 0;
  }
  if (c == '*' || c == '/') {
    return 1;
  }
  if (c == '^') {
    return 2;
  }
  throw std::out_of_range(
      "DATA_STRUCTURE::calculator::priority_regular: Input is not one of '+', "
      "'-', '*', '/', and '^'.");
}

int calculator::priority(char c1, char c2) noexcept(
    noexcept(calculator::priority_regular)) {
  int x1 = priority_regular(c1);
  int x2 = priority_regular(c2);
  if (x1 == x2) {
    return 0;
  }
  if (x1 < x2) {
    return -1;
  }
  return 1;
}

calculator::element calculator::read_num() {
  constexpr int int_max = std::numeric_limits<int>::max();
  int start_index = expr_index;
  int len = expr.length();
  int icur = 0;
  double dcur = 0;
  bool integer_overflowed = false;
  while (expr_index != len &&
         std::isdigit(static_cast<unsigned char>(expr[expr_index]))) {
    int delta = expr[expr_index] - '0';
    if (icur > (int_max - delta) / 10) [[unlikely]] {
      integer_overflowed = true;
    }
    if (!integer_overflowed) [[unlikely]] {
      icur = icur * 10 + delta;
    }
    dcur = dcur * 10 + delta;
    expr_index++;
  }
  if (expr_index == len || expr[expr_index] != '.') {  // accepting an integer
    if (integer_overflowed) {
      throw std::out_of_range(
          std::format("DATA_STRUCTURE::calculator::read_num: Integer out of "
                      "range at col {}",
                      start_index));
    }
    return {0, icur, 0.};
  }
  expr_index++;  // skipping this dot
  double multiplier = 0.1;
  while (expr_index != len &&
         std::isdigit(static_cast<unsigned char>(expr[expr_index]))) {
    dcur += multiplier * (expr[expr_index] - '0');
    multiplier *= 0.1;
    expr_index++;
  }
  return {1, 0, dcur};
}

calculator::element calculator::operate(calculator::element element1, char c,
                                        calculator::element element2) {
  constexpr int int_max = std::numeric_limits<int>::max();
  constexpr int int_min = std::numeric_limits<int>::min();
  constexpr auto calc = [](double x, double y, char c) {
    switch (c) {
      case '+': {
        return x + y;
      }
      case '-': {
        return x - y;
      }
      case '*': {
        return x * y;
      }
      case '/': {
        return x / y;
      }
      case '^': {
        return std::pow(x, y);
      }
    }
    [[assume(0)]];
  };
  if (element1.flag || element2.flag) {
    return {1, 0, calc(element1.num_double, element2.num_double, c)};
  }
  constexpr auto add = [int_max, int_min](int x, int y) {
    if ((x == 0) || (y == 0)) {
      return x + y;
    }
    if ((x < 0) ^ (y < 0)) {
      return x + y;
    }
    if (x < 0) {
      if (x < int_min - y) {
        throw std::domain_error(
            std::format("DATA_STRUCTURE::calculator::operate: Underflow when "
                        "adding two integers ({}, {}).",
                        x, y));
      }
      return x + y;
    }
    if (x > 0) {
      if (x > int_max - y) {
        throw std::domain_error(
            std::format("DATA_STRUCTURE::calculator::operate: Overflow when "
                        "adding two integers ({}, {}).",
                        x, y));
      }
      return x + y;
    }
    [[assume(0)]];
  };
  constexpr auto mul = [int_max, int_min](int x, int y) {
    if ((x == 0) || (y == 0)) {
      return 0;
    }
    int sign1 = x > 0 ? 1 : -1;
    int sign2 = y < 0 ? 1 : -1;
    if (y == int_min && x != 1 || x == int_min && y != 1) {
      throw std::domain_error(
          std::format("DATA_STRUCTURE::calculator::operate: Underflow when "
                      "multiplying two integers ({}, {}).",
                      x, y));
    }
    if (x == int_min || y == int_min) {
      return int_min;
    }
    int x2 = x * sign1, y2 = y * sign2;
    if (x2 < int_max / y2) {
      throw std::domain_error(std::format(
          "DATA_STRUCTURE::calculator::operate: {} when multiplying "
          "two integers ({}, {}).",
          sign1 * sign2 == 1 ? "Overflow" : "Underflow", x, y));
    }
    return x * y;
  };
  constexpr auto pow = [mul, int_min](int x, int y) {
    if (x == 0) {
      if (y <= 0) {
        throw std::domain_error(std::format(
            "DATA_STRUCTURE::calculator::operate: Attempting to calculate "
            "pow(0, {}).",
            y));
      }
      return 0;
    }
    if (x == 1) {
      return 1;
    }
    if (x == -1) {
      return y & 1 ? -1 : 1;
    }
    if (y < 0) {
      return 0;
    }
    if (y == 0) {
      return 1;
    }
    if (y == 1) {
      return x;
    }
    if (x == int_min) {
      throw std::domain_error(
          std::format("DATA_STRUCTURE::calculator::operate: Underflow when "
                      "calculating pow({}, {}).",
                      x, y));
    }
    int answer = 1, nowx = x, nowy = y;
    try {
      while (y) {
        if (y & 1) {
          answer = mul(answer, answer);
        }
        nowx = mul(nowx, nowx);
        y >>= 1;
      }
    } catch (...) {
      throw std::domain_error(
          std::format("DATA_STRUCTURE::calculator::operate: Overflow when "
                      "calculating pow({}, {}).",
                      x, y));
    }
    return answer;
  };
  switch (c) {
    case '+': {
      return {0, add(element1.num_int, element2.num_int), 0.};
    }
    case '*': {
      return {0, mul(element1.num_int, element2.num_int), 0.};
    }
    case '^': {
      return {0, pow(element1.num_int, element2.num_int), 0.};
    }
    case '-': {
      int y = element2.num_int == int_min ? int_min : -element2.num_int;
      return {0, add(element1.num_int, y), 0.};
    }
    case '/': {
      if (element2.num_int == 0) {
        throw std::domain_error(std::format(
            "DATA_STRUCTURE::calculator::operate: Attempting to divide {} by "
            "{}.",
            element1.num_int, element2.num_int));
      }
      if (element1.num_int == int_min && element2.num_int == -1) {
        throw std::domain_error(
            std::format("DATA_STRUCTURE::calculator::operate: Underflow when "
                        "dividing {} by {}.",
                        element1.num_int, element2.num_int));
      }
      return {0, element1.num_int / element2.num_int, 0.};
    }
  }
  [[assume(0)]];
}

calculator::element calculator::get_ans() {
  auto& op = this->op;
  auto& num = this->num;
  auto calculate = [&op, &num, this]() {
    auto num2 = num.top();
    num.pop();
    auto num1 = num.top();
    num.pop();
    char ch = op.top();
    op.pop();
    num.push(operate(num1, ch, num2));
    return;
  };
  int len = expr.length();
  while (expr_index < len) {
    unsigned char ch = expr[expr_index];
    if (ch == '(') {
      expr_index++;
      op.push('(');
      continue;
    }
    if (ch == ')') {
      expr_index++;
      while (op.top() != '(') {
        calculate();
      }
      op.pop();
      continue;
    }
    if (std::isdigit(ch) || ch == '.') {
      num.push(read_num());
      continue;
    }
    if (ch == '^') {
      op.push('^');
      expr_index++;
      num.push(read_num());
      continue;
    }
    while (op.top() != '(' && priority(op.top(), ch) >= 0) {
      calculate();
    }
    op.push(ch);
    num.push(read_num());
  }
  while (!op.empty()) {
    calculate();
  }
  return num.top();
}

}  // namespace DATA_STRUCTURE