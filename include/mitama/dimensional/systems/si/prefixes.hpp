#pragma once
#include <ratio>
#include <mitama/dimensional/core/core.hpp>

namespace mitama::dimensional::si {
  template <core::rational T>
  struct prefix_constant { using type = T; };

  template <class T> struct is_prefix_constant: std::false_type {};
  template <core::rational Ratio> struct is_prefix_constant<prefix_constant<Ratio>>: std::true_type {};

  template <class T> concept prefix_type = is_prefix_constant<T>::value;
}

namespace mitama::dimensional::si {
  inline constexpr auto pico  = prefix_constant<std::pico>{};
  inline constexpr auto nano  = prefix_constant<std::nano>{};
  inline constexpr auto micro = prefix_constant<std::micro>{};
  inline constexpr auto milli = prefix_constant<std::milli>{};
  inline constexpr auto centi = prefix_constant<std::centi>{};
  inline constexpr auto deci  = prefix_constant<std::deci>{};
  inline constexpr auto deca  = prefix_constant<std::deca>{};
  inline constexpr auto hecto = prefix_constant<std::hecto>{};
  inline constexpr auto kilo  = prefix_constant<std::kilo>{};
  inline constexpr auto mega  = prefix_constant<std::mega>{};
  inline constexpr auto giga  = prefix_constant<std::giga>{};
  inline constexpr auto tera  = prefix_constant<std::tera>{};
}

namespace mitama::dimensional::si {
  template <core::arithmetic ValueType, prefix_type Prefix>
  struct value_with_prefix {
    ValueType value;
    using prefix_type = Prefix;
    using ratio_type = typename Prefix::type;
    using value_type = ValueType;
    explicit operator ValueType() const { return value * prefix_type::num / prefix_type::den; }
  };

  template <class T> struct is_scaled_value: std::false_type {};
  template <core::arithmetic ValueType, prefix_type Prefix>
  struct is_scaled_value<value_with_prefix<ValueType, Prefix>>: std::true_type {};

  template <class T> concept scaled_value = is_scaled_value<T>::value;

  template <core::rational Scale>
  inline constexpr auto
  operator*(core::arithmetic auto value, prefix_constant<Scale> prefix)
    -> value_with_prefix<decltype(value), decltype(prefix)>
    { return {value}; }
}

namespace mitama::dimensional {
  inline constexpr auto operator*(si::prefix_type auto prefix, core::unit_type auto unit)
  -> core::unit<
      typename decltype(unit)::dimension_type,
          core::heterogeneous_system<
              core::heterogeneous_system_dim<
                  core::scaled_base_unit<
                      typename decltype(unit)::unit_type,
                      typename decltype(prefix)::type
                  >,
                  std::ratio<1, 1>
              >
          >
      >
  { return {}; }

  inline constexpr auto
  operator*(si::scaled_value auto scale, core::unit_type auto unit)
  -> quantity<
      core::unit<
          typename decltype(unit)::dimension_type,
          core::heterogeneous_system<
              core::heterogeneous_system_dim<
                  core::scaled_base_unit<
                      typename decltype(unit)::unit_type,
                      typename decltype(scale)::ratio_type
                  >,
                  std::ratio<1, 1>
              >
          >
      >,
      typename decltype(scale)::value_type
  >
  { return {scale.value}; }
}
