#include <iostream>
#include <mitama/dimensional/prelude.hpp>
#include <mitama/dimensional/systems/mks/base_units.hpp>
#include <mitama/dimensional/systems/cgs/base_units.hpp>
#include <mitama/dimensional/systems/si/prefixes.hpp>
#include <mitama/dimensional/systems/si/base_units.hpp>
#include <mitama/dimensional/systems/si/derived_units.hpp>
#include <mitama/dimensional/into.hpp>
//#include <boost/type_index.hpp>

template < typename T >
[[deprecated]] void f(T&&){}

namespace mitama::dimensional {
  template <>
  struct into_trait<std::remove_cvref_t<decltype(si::milli * si::meters)>, si::length> {
    constexpr auto operator ()(core::arithmetic auto from) const {
      return from / 1000;
    }
  };
  template <>
  struct into_trait<si::length, std::remove_cvref_t<decltype(si::milli * si::meters)>> {
    constexpr auto operator ()(core::arithmetic auto from) const {
      return from * 1000;
    }
  };
}

int main() {
  using namespace mitama::dimensional;

  constexpr refine::length  auto x = 3.0 * si::meters;
  constexpr refine::length  auto y = 2.0 * si::meters;

  constexpr refine::length  auto sum  = x + y;
  constexpr refine::length  auto diff = x - y;
  constexpr refine::area    auto area = x * y;
  constexpr refine::dimless auto rate = x / y;

  constexpr refine::length   auto distance = 6.0 * si::meters;
  constexpr refine::time     auto took = 2.0 * si::seconds;
  constexpr refine::velocity auto avg_speed = distance / took;

  std::cout << avg_speed.value << " m/s\n";

  {
    // explicit unit conversion with pipeline operator
    constexpr quantity mm = 3.0 * si::milli * si::meters | into<si::meter>;
    std::cout << mm.value << " m" << std::endl;

    // implicit unit conversion with pipeline operator
    using millimeter = std::remove_cvref_t<decltype(si::milli * si::meter)>;
    constexpr quantity<millimeter> m = 3.0 * si::meters | into<>;
    std::cout << m.value << " mm" << std::endl;

    // explicit unit conversion with function call
    { [[maybe_unused]] constexpr quantity _ = into<si::meter>(3.0 * si::milli * si::meters); }
    // implicit unit conversion with function call
    { [[maybe_unused]] constexpr quantity<millimeter> _ = into(3.0 * si::meters); }
  }
// An implicit narrowing conversion in value type should result in a compilation error.
//  { constexpr quantity<si::length, int> _ = 3.0 * si::meters | into<>; }
//                                   ^^^      ^^^
// Conversion from double to int is narrowing.
}
