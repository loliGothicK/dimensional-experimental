#pragma once
#include <mitama/dimensional/core/dimension.hpp>

namespace mitama::dimensional::cgs {
  struct length_base_unit : core::base_unit<base_dimension::length> {};
  struct mass_base_unit : core::base_unit<base_dimension::mass> {};
  struct time_base_unit : core::base_unit<base_dimension::time> {};

  using cgs_system = core::homogeneous_system<length_base_unit, mass_base_unit, time_base_unit>;

  using length = core::unit<length_dimension, cgs_system>;
  using mass = core::unit<length_dimension, cgs_system>;
  using time = core::unit<length_dimension, cgs_system>;
}
