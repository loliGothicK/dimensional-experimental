#pragma once
#include <mitama/dimensional/core/type_list.hpp>
#include <mitama/dimensional/core/base_dimension.hpp>
#include <mitama/dimensional/core/core.hpp>
#include <ratio>

/// Dimension types
namespace mitama::dimensional {
  using dimless = core::type_list<>;

  using length_dimension = core::type_list<core::dim<base_dimension::length>>;
  using area_dimension = core::type_list<core::dim<base_dimension::length, std::ratio<2>>>;
  using volume_dimension = core::type_list<core::dim<base_dimension::length, std::ratio<3>>>;

  using mass_dimension = core::type_list<core::dim<base_dimension::mass>>;

  using time_dimension = core::type_list<core::dim<base_dimension::time>>;

  using electric_current_dimension = core::type_list<core::dim<base_dimension::electric_current>>;

  using thermodynamic_temperature_dimension = core::type_list<core::dim<base_dimension::thermodynamic_temperature>>;

  using amount_of_substance_dimension = core::type_list<core::dim<base_dimension::amount_of_substance>>;

  using luminous_intensity_dimension = core::type_list<core::dim<base_dimension::luminous_intensity>>;

}
