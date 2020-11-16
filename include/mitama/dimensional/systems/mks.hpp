#pragma once
#include <mitama/dimensional/core/dimension.hpp>
#include <mitama/dimensional/quantity.hpp>

namespace mitama::dimensional::mks {
  struct length_base_unit : core::base_unit<base_dimension::length> {};
  struct mass_base_unit : core::base_unit<base_dimension::mass> {};
  struct time_base_unit : core::base_unit<base_dimension::time> {};

  using mks_system = core::homogeneous_system<length_base_unit, mass_base_unit, time_base_unit>;

  using length = core::unit<length_dimension, mks_system>;
  using mass = core::unit<mass_dimension, mks_system>;
  using time = core::unit<time_dimension, mks_system>;

  [[maybe_unused]] inline constexpr auto meter     = length{};
  [[maybe_unused]] inline constexpr auto meters    = length{};
  [[maybe_unused]] inline constexpr auto kilogram  = mass{};
  [[maybe_unused]] inline constexpr auto kilograms = mass{};
  [[maybe_unused]] inline constexpr auto second    = time{};
  [[maybe_unused]] inline constexpr auto seconds   = time{};

  inline constexpr auto
  operator*(core::arithmetic auto value, core::unit_type auto unit)
    -> quantity<decltype(unit), decltype(value)>
    { return {value}; }
}

#include <mitama/dimensional/systems/si/prefixes.hpp>

namespace mitama::dimensional::mks {
  inline constexpr auto
  operator*(si::value_with_prefix<auto, auto> value, core::unit_type auto unit)
    -> quantity<
        core::unit<
          typename decltype(unit)::dimension_type,
          core::heterogeneous_system<
            core::heterogeneous_system_info<
              typename core::transform<
                []<class D, class E>(std::type_identity<core::dim<D, E>>)
                  -> core::fst<core::filter_by<D, typename decltype(unit)::system_type>>
                { return {}; },
                typename decltype(unit)::dimension_type
              >::type,
              typename decltype(unit)::dimension_type,
              typename decltype(value)::ratio_type
            >
          >
        >,
        typename decltype(value)::value_type>
    { return {value.value}; }
}
