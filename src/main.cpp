#include <iostream>
#include <mitama/dimensional/all.hpp>
#include <mitama/dimensional/systems/cgs.hpp>
#include <mitama/dimensional/systems/mks.hpp>
#include <boost/type_index.hpp>
int main() {
  using namespace mitama::dimensional;
  constexpr quantity<mks::length, double> x = { 2 };
  constexpr quantity<mks::length, double> y = { 2 };
  constexpr refine::length auto sum = x + y;
  constexpr refine::length auto diff = x - y;
  constexpr refine::area auto area = x * y;
  constexpr refine::dimless auto rate = x / y;
  std::cout << area.value << " [m^2]\n";
}
