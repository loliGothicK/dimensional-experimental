#pragma once
#include <mitama/dimensional/core/dimension.hpp>
#include <mitama/dimensional/quantity.hpp>

namespace mitama::dimensional::si {
  struct length_base_unit : core::base_unit<base_dimension::length> {};
  struct mass_base_unit : core::base_unit<base_dimension::mass> {};
  struct time_base_unit : core::base_unit<base_dimension::time> {};
  struct electric_current_base_unit : core::base_unit<base_dimension::electric_current> {};
  struct thermodynamic_temperature_base_unit : core::base_unit<base_dimension::thermodynamic_temperature> {};
  struct amount_of_substance_base_unit : core::base_unit<base_dimension::amount_of_substance> {};
  struct luminous_intensity_base_unit : core::base_unit<base_dimension::luminous_intensity> {};

  using si_system = core::homogeneous_system<
          length_base_unit,
          mass_base_unit,
          time_base_unit,
          electric_current_base_unit,
          thermodynamic_temperature_base_unit,
          amount_of_substance_base_unit,
          luminous_intensity_base_unit
  >;

  using length = core::unit<length_dimension, si_system>;
  using mass = core::unit<mass_dimension, si_system>;
  using time = core::unit<time_dimension, si_system>;
  using electric_current = core::unit<electric_current_dimension, si_system>;
  using thermodynamic_temperature = core::unit<thermodynamic_temperature_dimension, si_system>;
  using amount_of_substance = core::unit<amount_of_substance_dimension, si_system>;
  using luminous_intensity = core::unit<luminous_intensity_dimension, si_system>;

  [[maybe_unused]] inline constexpr auto meter     = length{};
  [[maybe_unused]] inline constexpr auto meters    = length{};
  [[maybe_unused]] inline constexpr auto kilogram  = mass{};
  [[maybe_unused]] inline constexpr auto kilograms = mass{};
  [[maybe_unused]] inline constexpr auto second    = time{};
  [[maybe_unused]] inline constexpr auto seconds   = time{};
  [[maybe_unused]] inline constexpr auto ampere    = electric_current{};
  [[maybe_unused]] inline constexpr auto amperes   = electric_current{};
  [[maybe_unused]] inline constexpr auto kelvin    = thermodynamic_temperature{};
  [[maybe_unused]] inline constexpr auto kelvins   = thermodynamic_temperature{};
  [[maybe_unused]] inline constexpr auto mol       = amount_of_substance{};
  [[maybe_unused]] inline constexpr auto candela   = luminous_intensity{};
}
