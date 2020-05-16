#pragma once
#include <mitama/dimensional/dimension.hpp>

namespace mitama::dimensional::cgs {
  struct length_base_unit : mitamagic::base_unit<base_dimension::length> {};
  struct mass_base_unit : mitamagic::base_unit<base_dimension::mass> {};
  struct time_base_unit : mitamagic::base_unit<base_dimension::time> {};

  using cgs_system = mitamagic::system<length_base_unit, mass_base_unit, time_base_unit>;

  using length = mitamagic::unit<length_dimension, cgs_system>;
  using mass = mitamagic::unit<length_dimension, cgs_system>;
  using time = mitamagic::unit<length_dimension, cgs_system>;
}
