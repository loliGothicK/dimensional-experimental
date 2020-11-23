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
              concat< // Just Collects Just Dim
                std::conditional_t< // Constructs Maybe Dim ---------------------------------------------+
                  std::is_same_v<BaseDim, typename Dims::base_type>, // if Dim is same as BaseDim        |
                  type_list<Dims>, // then; type_list<Dim> instead of Just Dim                           |
                  type_list<>      // otherwise; empty type_list instead of Nothing                      |
                >... // <--------------------------------------------------------------------------------+
              >
          >
  {};

  // * -> [*] -> [*]
  template <auto, class> struct transform;
  template <auto Map, class ...Dims>
  struct transform<Map, type_list<Dims...>>
          : std::type_identity<type_list<decltype(Map(std::type_identity<Dims>{}))...>>
  {};

  template <class T> struct to_base_dim { using type = T::dimension_type; };

  // internal only
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

  template <class ...Ratios>
          requires satisfy_all_of<rational<Ratios>...>
  using ratio_sum = _secrets::ratio_sum_impl<Ratios...>::type;

  // phantom type to distinguish systems of measurement
  template <class...> struct homogeneous_system {};

  template <class> struct heterogeneous_system {};

  template <class, // Dim List
            class, // Base Units
            class> // Scales
  struct heterogeneous_system_info {};

  template <class BaseUnit, class Scale>
  struct heterogeneous_system_dim {};

  // top-level phantom type
  template <class Dim, class System>
  struct unit;

  // internal only
  namespace _secrets {
    template<class>
    struct reduce_dim_impl;
    template<>
    struct reduce_dim_impl<type_list<>>
      { using type = type_list<>; };
    template<class Base, core::rational ... Exponents>
    struct reduce_dim_impl<type_list<dim<Base, Exponents>...>>
      { using type = type_list<dim<Base, ratio_sum<Exponents...>>>; };
  }

  // Reduces same dimensions
  template <class, class> struct reduce_dim;
  template <class ...Terms, class ...Bases>
  struct reduce_dim<type_list<Terms...>, homogeneous_system<Bases...>>
          : std::type_identity<
            concat<
              typename _secrets::reduce_dim_impl<
                typename filter_by<typename Bases::dimension_type, type_list<Terms...>>::type
              >::type...
            >
          >
  {};

  template <class S, class D, class ...Terms, class ...Bases>
  struct reduce_dim<
            type_list<Terms...>,
            heterogeneous_system<
              heterogeneous_system_info<D, type_list<Bases...>, S>
            >
          >
          : std::type_identity<
            concat<
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
    // dimension-type (unit without system)
    using dimension_type = type_list<Terms...>;
    // homogeneous_system-type
    using system_type    = System;
  };
  struct base_unit_tag {};
  // base class for base units
  template <class Dim>
  struct base_unit : private base_unit_tag { using dimension_type = Dim; };

  // base unit with scale
  template <class BaseUnit, rational Scale>
  struct scaled_base_unit : private base_unit_tag {
    using dimension_type = BaseUnit::dimension_type;
    using scale_type = Scale;
  };

  // helper type function
  template <class BaseUnit, class Scale>
  struct make_scaled_unit
    { using type = scaled_base_unit<BaseUnit, Scale>; };

  // internal only
  namespace _secrets {
    template <class, class> struct unit_add_impl;

    template <class ...LeftDims, class ...RightDims, class ...BaseDims>
    struct unit_add_impl<
            unit<type_list<LeftDims...>, homogeneous_system<BaseDims...>>,
            unit<type_list<RightDims...>, homogeneous_system<BaseDims...>>>
            : std::type_identity<
              unit<
                filtered<
                  []<class _, rational E>(std::type_identity<dim<_, E>>)
                  /* => */ { return !std::ratio_equal_v<E, std::ratio<0>>; },
                  type_list<dim<typename BaseDims::dimension_type, std::ratio_add<
                    get_or_default<typename BaseDims::dimension_type, std::ratio<0>, type_list<LeftDims...>>,
                    get_or_default<typename BaseDims::dimension_type, std::ratio<0>, type_list<RightDims...>>
                >>...>>,
                homogeneous_system<BaseDims...>
              >
            >
    {};
  }

  template <class L, class R>
  using unit_add = _secrets::unit_add_impl<reduced_unit<L>, reduced_unit<R>>::type;

  // internal only
  namespace _secrets {
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
  }

  template <class U>
  using unit_negate = _secrets::unit_negate_impl<reduced_unit<U>>::type;

  template <class L, class R>
  using unit_subtract = unit_add<L, unit_negate<R>>;
}


// concepts for internal use
namespace mitama::dimensional::core {
  namespace _secrets {
    template <class> struct is_unit : std::false_type {};
    template <class D, class S> struct is_unit<unit<D, S>> : std::true_type {};
  }

  template <class T> concept unit_type = _secrets::is_unit<T>::value;

  template <unit_type U1, unit_type U2>
  struct is_same_system_with: std::is_same<typename U1::system_type, typename U2::system_type> {};

  template <class T, class U>
  concept same_system_with = is_same_system_with<T, U>::value;
}

namespace mitama::dimensional::core {
  template <unit_type U1, unit_type U2> requires same_system_with<U1, U2>
  inline constexpr auto operator*(U1, U2) -> unit_add<U1, U2>
  { return {}; }

  template <unit_type U1, unit_type U2> requires same_system_with<U1, U2>
  inline constexpr auto operator/(U1, U2) -> unit_subtract<U1, U2>
  { return {}; }
}
