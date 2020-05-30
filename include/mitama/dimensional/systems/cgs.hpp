#pragma once
#include <mitama/dimensional/core/dimension.hpp>

namespace mitama::dimensional::mks {
  struct length_base_unit : core::base_unit<base_dimension::length> {};
  struct mass_base_unit : core::base_unit<base_dimension::mass> {};
  struct time_base_unit : core::base_unit<base_dimension::time> {};

  using mks_system = core::homogeneous_system<length_base_unit, mass_base_unit, time_base_unit>;

  using length = core::unit<length_dimension, mks_system>;
  using mass = core::unit<mass_dimension, mks_system>;
  using time = core::unit<time_dimension, mks_system>;
}

