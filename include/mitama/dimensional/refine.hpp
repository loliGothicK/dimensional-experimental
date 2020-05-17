#pragma once
#include <concepts>
#include <mitama/dimensional/core/dimension.hpp>

// concepts for refinement
namespace mitama::dimensional::refine {
  template <class Quantity, class Dimension>
  concept has_dimension = std::same_as<typename Quantity::dimension_type, Dimension>;

  template <class Quantity>
  concept dimless = has_dimension<Quantity, dimless>;

  template <class Quantity>
  concept length = has_dimension<Quantity, length_dimension>;

  template <class Quantity>
  concept area = has_dimension<Quantity, area_dimension>;

  template <class Quantity>
  concept volume = has_dimension<Quantity, volume_dimension>;
} // namespace mitama::dimensional::refine
