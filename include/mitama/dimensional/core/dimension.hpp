#pragma once
#include <mitama/dimensional/mitamagic/type_list.hpp>
#include <mitama/dimensional/core/base_dimension.hpp>
#include <mitama/dimensional/core/core.hpp>
#include <ratio>

/// Dimension types
namespace mitama::dimensional {
  using dimless = mitamagic::type_list<>;

  using length_dimension = mitamagic::type_list<mitamagic::dim<base_dimension::length>>;
  using area_dimension = mitamagic::type_list<mitamagic::dim<base_dimension::length, std::ratio<2>>>;
  using volume_dimension = mitamagic::type_list<mitamagic::dim<base_dimension::length, std::ratio<3>>>;

  using mass_dimension = mitamagic::type_list<mitamagic::dim<base_dimension::mass>>;
  using time_dimension = mitamagic::type_list<mitamagic::dim<base_dimension::time>>;
}
