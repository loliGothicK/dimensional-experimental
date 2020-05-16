#pragma once
#include <mitama/dimensional/quantity.hpp>

/// Arithmetic operators
namespace mitama::dimensional {
  // add
  template <class Unit, class T, class U>
    requires requires (T t, U u) { t + u; }
  inline constexpr auto operator+(
      quantity<Unit, T> const& lhs,
      quantity<Unit, U> const& rhs)
    -> quantity<Unit, decltype(std::declval<T>() + std::declval<U>())>
  { return {lhs.value + rhs.value}; }

  // sub
  template <class Unit, class T, class U>
      requires requires (T t, U u) { t - u; }
  inline constexpr auto operator-(
          quantity<Unit, T> const& lhs,
          quantity<Unit, U> const& rhs)
      -> quantity<Unit, decltype(std::declval<T>() - std::declval<U>())>
  { return {lhs.value - rhs.value}; }

  // mul
  template <class System, class DimL, class T, class DimR, class U>
      requires requires (T t, U u) { t * u; }
  inline constexpr auto operator*(
          quantity<mitamagic::unit<DimL, System>, T> const& lhs,
          quantity<mitamagic::unit<DimR, System>, U> const& rhs)
      -> quantity<
          mitamagic::unit_add<mitamagic::unit<DimL, System>, mitamagic::unit<DimR, System>>,
          decltype(std::declval<T>() * std::declval<U>())>
  { return {lhs.value * rhs.value}; }

  // div
  template <class System, class DimL, class T, class DimR, class U>
      requires requires (T t, U u) { t / u; }
  inline constexpr auto operator/(
          quantity<mitamagic::unit<DimL, System>, T> const& lhs,
          quantity<mitamagic::unit<DimR, System>, U> const& rhs)
      -> quantity<
          mitamagic::unit_subtract<mitamagic::unit<DimL, System>, mitamagic::unit<DimR, System>>,
          decltype(std::declval<T>() / std::declval<U>())>
  { return {lhs.value / rhs.value}; }
}
