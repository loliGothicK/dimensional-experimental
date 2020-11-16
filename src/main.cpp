#include <iostream>
#include <mitama/dimensional/all.hpp>
#include <mitama/dimensional/systems/mks.hpp>
#include <mitama/dimensional/systems/cgs.hpp>
#include <mitama/dimensional/systems/si/prefixes.hpp>
//#include <boost/type_index.hpp>

template < typename T >
[[deprecated]] void f(T&&){}

int main() {
  using namespace mitama::dimensional;

  constexpr refine::length  auto x = 3.0 * mks::meters;
  constexpr refine::length  auto y = 2.0 * mks::meters;

  constexpr refine::length  auto sum  = x + y;
  constexpr refine::length  auto diff = x - y;
  constexpr refine::area    auto area = x * y;
  constexpr refine::dimless auto rate = x / y;

  constexpr refine::length   auto distance = 6.0 * mks::meters;
  constexpr refine::time     auto took = 2.0 * mks::seconds;
  constexpr refine::velocity auto avg_speed = distance / took;

  std::cout << avg_speed.value << " m/s\n";

  constexpr quantity<auto, auto> mm = 3 * si::milli * mks::meters;
  // constexpr quantity<cgs::length, int> centi_metre = mm;
}
