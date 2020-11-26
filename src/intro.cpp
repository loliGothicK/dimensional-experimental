//
// Created by mitama on 11/26/20.
//

#include <mitama/dimensional/prelude.hpp>
#include <mitama/dimensional/systems/si/all.hpp>
#include <iostream>

using namespace mitama::dimensional;

void test(refine::area auto area) { // <- refine::areaは引数を面積の次元に成約するコンセプト
  std::cout << "area = " << area.value << " m^2" << std::endl;
}

int main() {
  // `value * si::meters` によって
  // x, yは長さという次元とメートルという単位の情報が付加され
  // quantity型になる
  constexpr quantity x = 3.0 * si::meters;
  constexpr quantity y = 2.0 * si::meters;

  test(x * y); // <- 長さ × 長さ = 面積

  // 次のような呼び出しは、testが面積を要求しているのに対して長さを渡しているのでコンパイルエラーになる
  // test(x)
}
