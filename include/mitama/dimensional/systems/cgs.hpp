#pragma once
#include <mitama/dimensional/dimension.hpp>

namespace mitama::dimensional::mks {
  struct length_base_unit : mitamagic::base_unit<base_dimension::length> {};
  struct mass_base_unit : mitamagic::base_unit<base_dimension::mass> {};
  struct time_base_unit : mitamagic::base_unit<base_dimension::time> {};

  using mks_system = mitamagic::system<length_base_unit, mass_base_unit, time_base_unit>;

  using length = mitamagic::unit<length_dimension, mks_system>;
  using mass = mitamagic::unit<mass_dimension, mks_system>;
  using time = mitamagic::unit<time_dimension, mks_system>;
}

