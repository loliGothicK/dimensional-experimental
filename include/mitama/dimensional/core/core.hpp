#pragma once
#include <mitama/dimensional/mitamagic/type_list.hpp>
#include <mitama/dimensional/mitamagic/concepts.hpp>
#include <ratio>
#include <type_traits>

// core interfaces
namespace mitama::dimensional::mitamagic {
  // pair of Dim and Ratio
  template <class Base, mitama::rational Ratio = std::ratio<1>>
  struct dim : Base {
    using dimension_type = dim<Base, Ratio>;
    using base_type = Base;
    using ratio = Ratio;
  };

  template <class, class> struct filter_by;
  template <class BaseDim, class ...Dims>
  struct filter_by<BaseDim, type_list<Dims...>>
    : std::type_identity<
        concat_t<
          std::conditional_t<
            std::is_same_v<BaseDim, typename Dims::dimension_type>,
            type_list<Dims>,
            type_list<>
          >...
        >
      >
  {};

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

  template <class ...Ratios> using ratio_sum = ratio_sum_impl<Ratios...>::type;

  // phantom type to distinguish systems of measurement
  template <class...> struct system {};

  // toplevel phantom type
  template <class Dim, class System>
  struct unit;

  template <class> struct reduce_unit_impl;
  template <> struct reduce_unit_impl<type_list<>>
    { using type = type_list<>; };
  template <class Base, mitama::rational ... Exponents>
  struct reduce_unit_impl<type_list<dim<Base, Exponents>...>>
    { using type = type_list<dim<Base, ratio_sum<Exponents...>>>; };

  template <class, class> struct reduce_unit;
  template <class ...Terms, class ...Bases>
  struct reduce_unit<type_list<Terms...>, system<Bases...>>
    : std::type_identity<
        concat_t<
          typename reduce_unit_impl<
            typename filter_by<Bases, type_list<Terms...>>::type
          >::type...
        >
      >
  {};

  template<typename ...Terms, typename System>
  struct unit<type_list<Terms...>, System> {
    // reduced unit-type
    using unit_type      = unit<typename reduce_unit<type_list<Terms...>, System>::type, System>;
    // dimension-type (unit without system)
    using dimension_type = type_list<Terms...>;
    // system-type
    using system_type    = System;
  };

  template <class Dim>
  struct base_unit { using dimension_type = Dim; };

  template <class BaseUnit, class Scale>
  struct scaled_base_unit {};

  template <auto Base, mitama::rational Exponet>
    requires (std::is_arithmetic_v<decltype(Base)>)
  struct scale {};

  template <class BaseUnit, class Scale>
  struct make_scaled_unit
    { using type = scaled_base_unit<BaseUnit, Scale>; }; 

  template <class, class> struct unit_add_impl;
  
  template <class ...LeftDims, class ...RightDims, class ...BaseDims>
  struct unit_add_impl<
    unit<type_list<LeftDims...>, system<BaseDims...>>,
    unit<type_list<RightDims...>, system<BaseDims...>>
  >
    : std::type_identity<
        unit<
          filter_t<[]<class D, auto N, auto _>(std::type_identity<dim<D, std::ratio<N, _>>>)
                   /* => */ { return !!N; },
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
