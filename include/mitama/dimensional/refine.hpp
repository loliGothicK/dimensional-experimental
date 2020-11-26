#pragma once
#include <concepts>
#include <mitama/dimensional/core/dimension.hpp>

namespace mitama::dimensional::core {
  template <class Quantity, class Dimension>
  concept has_dimension = std::same_as<typename Quantity::dimension_type, Dimension>;
}

// concepts for refinement
namespace mitama::dimensional::refine {
  template <class Quantity>
  concept dimless = core::has_dimension<Quantity, dimless>;

  template <class Quantity>
  concept length = core::has_dimension<Quantity, length_dim>;

  template <class Quantity>
  concept area = core::has_dimension<Quantity, area_dim>;

  template <class Quantity>
  concept volume = core::has_dimension<Quantity, volume_dim>;

  template <class Quantity>
  concept time = core::has_dimension<Quantity, time_dim>;

  template <class Quantity>
  concept velocity = core::has_dimension<Quantity, velocity_dim>;
} // namespace mitama::dimensional::refine
