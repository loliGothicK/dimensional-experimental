#pragma once
#include <mitama/dimensional/quantity.hpp>

/// Arithmetic operators
namespace mitama::dimensional {
  // add
  template <quantity_type T, quantity_type U>
      requires std::same_as<typename T::unit_type, typename U::unit_type>
            && requires (T lhs, U rhs) { { lhs.value + rhs.value } -> core::arithmetic; }
  inline constexpr auto operator+(T const& lhs, U const& rhs)
      -> quantity<typename T::unit_type, std::common_type_t<typename T::value_type, typename U::value_type>>
  { return {lhs.value + rhs.value}; }

  // sub
  template <quantity_type T, quantity_type U>
      requires std::same_as<typename T::unit_type, typename U::unit_type>
            && requires (T t, U u) { { t.value - u.value } -> core::arithmetic; }
  inline constexpr auto operator-(T const& lhs, U const& rhs)
      -> quantity<typename T::unit_type, std::common_type_t<typename T::value_type, typename U::value_type>>
  { return {lhs.value - rhs.value}; }

  // mul
  template <quantity_type T, quantity_type U>
      requires std::same_as<typename T::system_type, typename U::system_type>
            && requires (T t, U u) { { t.value * u.value } -> core::arithmetic; }
  inline constexpr auto operator*(T const& lhs, U const& rhs)
      -> quantity<
          core::unit_add<typename T::unit_type, typename U::unit_type>,
          std::common_type_t<typename T::value_type, typename U::value_type>>
  { return {lhs.value * rhs.value}; }

  // div
  template <quantity_type T, quantity_type U>
      requires std::same_as<typename T::system_type, typename U::system_type>
            && requires (T t, U u) { { t.value / u.value } -> core::arithmetic; }
  inline constexpr auto operator/(T const& lhs, U const& rhs)
      -> quantity<
          core::unit_subtract<typename T::unit_type, typename U::unit_type>,
          std::common_type_t<typename T::value_type, typename U::value_type>>
  { return {lhs.value / rhs.value}; }
}
