#pragma once
#include <mitama/dimensional/core/type_list.hpp>
#include <mitama/dimensional/core/base_dimension.hpp>
#include <mitama/dimensional/core/core.hpp>
#include <ratio>

/// Dimension types
namespace mitama::dimensional {
  // dimensionless dimension
  using dimless = core::type_list<>;

  // SI Base Dimensions
  using length_dim = core::type_list<core::dim<base_dimension::length>>;
  using mass_dim = core::type_list<core::dim<base_dimension::mass>>;
  using time_dim = core::type_list<core::dim<base_dimension::time>>;
  using electric_current_dim = core::type_list<core::dim<base_dimension::electric_current>>;
  using thermodynamic_temperature_dim = core::type_list<core::dim<base_dimension::thermodynamic_temperature>>;
  using amount_of_substance_dim = core::type_list<core::dim<base_dimension::amount_of_substance>>;
  using luminous_intensity_dim = core::type_list<core::dim<base_dimension::luminous_intensity>>;

  // Derived Dimensions
  using area_dim = core::type_list<core::dim<base_dimension::length, std::ratio<2>>>;
  using volume_dim = core::type_list<core::dim<base_dimension::length, std::ratio<3>>>;

  using velocity_dim = core::type_list<core::dim<base_dimension::length>, core::dim<base_dimension::time, std::ratio<-1>>>;
}
