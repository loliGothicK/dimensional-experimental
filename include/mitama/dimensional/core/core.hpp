#pragma once
#include <mitama/dimensional/core/type_list.hpp>
#include <mitama/dimensional/core/concepts.hpp>
#include <ratio>
#include <type_traits>

// core interfaces
namespace mitama::dimensional::core {
  // pair of Dim and Ratio
  template <class Base, core::rational Ratio = std::ratio<1>>
  struct dim : Base {
    // Self type
    using dimension_type = dim<Base, Ratio>;
    // base dimension-type
    using base_type = Base;
    // exponent
    using ratio = Ratio;
  };

  // * -> [*] -> [*]
  template <class, class> struct filter_by;
  template <class BaseDim, class ...Dims>
  struct filter_by<BaseDim, type_list<Dims...>>
          : std::type_identity<
              concat_t< // Collects Just Dim: [Maybe *] -> [*]
                std::conditional_t< // Maybe Dim --------------------------------------------------------+
                  std::is_same_v<BaseDim, typename Dims::dimension_type>, // if Dim is same as BaseDim   |
                  type_list<Dims>, // then; Just Dim                                                     |
                  type_list<>      // otherwise; Nothing                                                 |
                >... // <--------------------------------------------------------------------------------+
              >
          >
  {};

  // internal use only
  namespace _secrets {
    template <class ...>
    struct ratio_sum_impl;
    template <>
    struct ratio_sum_impl<> { using type = std::ratio<0>; };
    template <class First>
    struct ratio_sum_impl<First> { using type = First; };
    template <class First, class Second>
    struct ratio_sum_impl<First, Second> { using type = std::ratio_add<First, Second>; };
    template <class First, class Second, class ...Tail>
    struct ratio_sum_impl<First, Second, Tail...>: ratio_sum_impl<std::ratio_add<First, Second>, Tail...> {};
  }

  template <class ...Ratios> using ratio_sum = _secrets::ratio_sum_impl<Ratios...>::type;

  // phantom type to distinguish systems of measurement
  template <class...> struct homogeneous_system {};

  // [ Example:
  //  unit<
  //    Compatible Dimension,
  //    heterogeneous_system<
  //      type_list<
  //        System_0,
  //        System_1,
  //        ...,
  //        System_n
  //  >>>
  // -- end example ]
  template <class...> struct heterogeneous_system {};

// top-level phantom type
  template <class Dim, class System>
  struct unit;

  namespace _secrets {
    template<class>
    struct reduce_dim_impl;
    template<>
    struct reduce_dim_impl<type_list<>>
      { using type = type_list<>; };
    template<class Base, core::rational ... Exponents>
    struct reduce_dim_impl<type_list < dim < Base, Exponents>...>>
      { using type = type_list<dim<Base, ratio_sum<Exponents...>>>; };
  }

  // Reduces same dimensions
  template <class, class> struct reduce_dim;
  template <class ...Terms, class ...Bases>
  struct reduce_dim<type_list<Terms...>, homogeneous_system<Bases...>>
          : std::type_identity<
            concat_t<
              typename _secrets::reduce_dim_impl<
                typename filter_by<Bases, type_list<Terms...>>::type
              >::type...
            >
          >
  {};

  template <class Dimensions, class System> using reduced_dim = reduce_dim<Dimensions, System>::type;

  template<class Unit>
  using reduced_unit = Unit::unit_type;

  template<typename ...Terms, typename System>
  struct unit<type_list<Terms...>, System> {
    // reduced unit-type
    using unit_type      = unit<reduced_dim<type_list<Terms...>, System>, System>;
    // dimension-type (unit without homogeneous_system)
    using dimension_type = type_list<Terms...>;
    // homogeneous_system-type
    using system_type    = System;
  };
  struct base_unit_tag {};
  // base class for base units
  template <class Dim>
  struct base_unit : private base_unit_tag { using dimension_type = Dim; };

  // base unit with scale
  template <class BaseUnit, class Scale>
  struct scaled_base_unit : private base_unit_tag { using dimension_type = BaseUnit::dimension_type; };

  // for scaled_base_unit
  template <auto Base, core::rational Exponet>
    requires (std::is_arithmetic_v<decltype(Base)>)
  struct scale {};

  // helper type function
  template <class BaseUnit, class Scale>
  struct make_scaled_unit
    { using type = scaled_base_unit<BaseUnit, Scale>; };

  template <class, class> struct unit_add_impl;

  template <class ...LeftDims, class ...RightDims, class ...BaseDims>
  struct unit_add_impl<
          unit<type_list<LeftDims...>, homogeneous_system<BaseDims...>>,
          unit<type_list<RightDims...>, homogeneous_system<BaseDims...>>>
          : std::type_identity<
            unit<
              filter_t<
                []<class D, auto N, auto _>(std::type_identity<dim<D, std::ratio<N, _>>>)
                /* => */ { return !!N; },
                type_list<dim<typename BaseDims::dimension_type, std::ratio_add<
                  get_value_or_t<std::ratio<0>, typename BaseDims::dimension_type, type_list<LeftDims...>>,
                  get_value_or_t<std::ratio<0>, typename BaseDims::dimension_type, type_list<RightDims...>>
              >>...>>,
              homogeneous_system<BaseDims...>
            >
          >
  {};

  template <class L, class R>
  using unit_add = unit_add_impl<L, R>::type;

  template <class> struct unit_negate_impl;

  template <class ...Dims, class System>
  struct unit_negate_impl<unit<type_list<Dims...>, System>>
          : std::type_identity<
            unit<
              type_list<
                dim<
                  typename Dims::base_type,
                  std::ratio_subtract<
                    std::ratio<0>,
                    typename Dims::ratio
                >>...
              >,
              System
            >
          >
  {};

  template <class U>
  using unit_negate = unit_negate_impl<U>::type;

  template <class L, class R>
  struct unit_subtract_impl: unit_add_impl<L, unit_negate<R>> {};

  template <class L, class R>
  using unit_subtract = unit_subtract_impl<L, R>::type;
}
