#pragma once
#include <mitama/dimensional/core/dimension.hpp>
#include <mitama/dimensional/quantity.hpp>

namespace mitama::dimensional::mks {
  struct length_base_unit : core::base_unit<base_dimension::length> {};
  struct mass_base_unit : core::base_unit<base_dimension::mass> {};
  struct time_base_unit : core::base_unit<base_dimension::time> {};

  using mks_system = core::homogeneous_system<length_base_unit, mass_base_unit, time_base_unit>;

  using length = core::unit<length_dim, mks_system>;
  using mass = core::unit<mass_dim, mks_system>;
  using time = core::unit<time_dim, mks_system>;

  [[maybe_unused]] inline constexpr auto meter     = length{};
  [[maybe_unused]] inline constexpr auto meters    = length{};
  [[maybe_unused]] inline constexpr auto kilogram  = mass{};
  [[maybe_unused]] inline constexpr auto kilograms = mass{};
  [[maybe_unused]] inline constexpr auto second    = time{};
  [[maybe_unused]] inline constexpr auto seconds   = time{};
}
