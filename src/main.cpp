#include <iostream>
#include <dimensional/dimensional.hpp>

int main() {
  using namespace mitama::dimensional;
  quantity<mks::length, double> x = { 2 };
  quantity<mks::length, double> y = { 2 };
  refine::length auto sum = x + y;
  refine::length auto diff = x - y;
  refine::area auto area = x * y;
  quantity rate = x / y;
  std::cout << area.value << " [m^2]\n";
}
