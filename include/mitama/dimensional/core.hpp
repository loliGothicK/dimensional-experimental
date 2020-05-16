#pragma once
#include <mitama/dimensional/mitamagic/type_list.hpp>
#include <ratio>
#include <type_traits>

// core interfaces
namespace mitama::dimensional::mitamagic {
  // pair of Dim and Ratio
  template <class Base, class Ratio = std::ratio<1>>
  struct dim : Base {
    using dimension_type = dim<Base, Ratio>;
    using base_type = Base;
    using ratio = Ratio;
  };

  // phantom type to distinguish systems of measurement
  template <class...> struct system {};

  // toplevel phantom type
  template <class Dim, class System>
  struct unit;

  template<typename ...Terms, typename System>
  struct unit<type_list<Terms...>, System> {
    // self type
    using unit_type      = unit<type_list<Terms...>, System>;
    // dimension type (without system)
    using dimension_type = type_list<Terms...>;
    // system type
    using system_type    = System;
  };

  template <class Dim>
  struct base_unit { using dimension_type = Dim; };

  template <class, class> struct unit_add_impl;
  
  template <class ...LeftDims, class ...RightDims, class ...BaseDims>
  struct unit_add_impl<
    unit<type_list<LeftDims...>, system<BaseDims...>>,
    unit<type_list<RightDims...>, system<BaseDims...>>
  >
    : std::type_identity<
        unit<
          filter_t<[]<class D, auto N, auto _>(std::type_identity<dim<D, std::ratio<N, _>>>){ return !!N; },
            type_list<dim<typename BaseDims::dimension_type, std::ratio_add<
              get_value_or_t<std::ratio<0>, typename BaseDims::dimension_type, type_list<LeftDims...>>,
              get_value_or_t<std::ratio<0>, typename BaseDims::dimension_type, type_list<RightDims...>>
            >>...>>,
          system<BaseDims...>
        >
      >
  {};

  template <class L, class R>
  using unit_add = typename unit_add_impl<L, R>::type;

  template <class> struct unit_negate_impl;
  
  template <class ...Dims, class System>
  struct unit_negate_impl<
    unit<type_list<Dims...>, System>
  >
    : std::type_identity<
      unit<type_list<
        dim<typename Dims::base_type,
          std::ratio_subtract<
            std::ratio<0>,
            typename Dims::ratio
        >>...
      >,
      System>>
  {};

  template <class U>
  using unit_negate = typename unit_negate_impl<U>::type;

  template <class L, class R>
  struct unit_subtract_impl: unit_add_impl<L, unit_negate<R>> {};

  template <class L, class R>
  using unit_subtract = typename unit_subtract_impl<L, R>::type;
}
