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

  template <class Quantity>
  concept time = has_dimension<Quantity, time_dimension>;

  template <class Quantity>
  concept velocity
    = has_dimension<
        Quantity,
        core::type_list<
          core::dim<base_dimension::length>,
          core::dim<base_dimension::time, std::ratio<-1>>
        >
      >;
} // namespace mitama::dimensional::refine
