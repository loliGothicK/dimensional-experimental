#include <ratio>
#include <type_traits>
#include <concepts>

namespace mitama::dimensional::mitamagic {
  template <class... ElemTypes> struct type_list {};

  template <class, class> struct findee;

  template <template <class, class> class Pair, class ToFind, class Key, class Value>
  struct findee<ToFind, Pair<Key, Value>>
  {
    inline static constexpr struct hack {
      constexpr operator bool() const noexcept { return std::is_same_v<ToFind, Key>; }
      constexpr bool operator()() const noexcept { return std::is_same_v<ToFind, Key>; }
      using type = Value;
    } value{};
  };

  template <class, class, class> struct get_value_or;
  template <class Default, class ToFind, class ...Pairs>
  struct get_value_or<Default, ToFind, type_list<Pairs...>>
    : std::conditional<
        std::disjunction_v<findee<ToFind, Pairs>...>,
        typename decltype(std::disjunction<findee<ToFind, Pairs>...>::value)::type,
        Default
      >
  {};

  template <class Default, class ToFind, class Dict>
  using get_value_or_t = typename get_value_or<Default, ToFind, Dict>::type;

  template <class...> struct concat;

  template <template <class...> class Pack, class... Elems>
  struct concat<Pack<Elems...>> {
    using type = Pack<Elems...>;
  };

  template <template <class...> class Pack, class... Left, class... Right>
  struct concat<Pack<Left...>, Pack<Right...>> {
    using type = Pack<Left..., Right...>;
  };

  template <class Pack1st, class Pack2nd, class... PackTail>
  struct concat<Pack1st, Pack2nd, PackTail...>
    : concat<typename concat<Pack1st, Pack2nd>::type, PackTail...> {
  };

  template <class... Packs>
  using concat_t = typename concat<Packs...>::type;

  template <auto, class> struct filter;

  template <auto Pred, template <class...> class List, class ...ElemTypes>
    requires (requires { { Pred(std::type_identity<ElemTypes>{}) } -> std::convertible_to<bool>; } && ...)
  struct filter<Pred, List<ElemTypes...>>
    : std::type_identity<concat_t<std::conditional_t< Pred(std::type_identity<ElemTypes>{}), type_list<ElemTypes>, type_list<> >...>>
  {};

  template <auto Pred, class List>
  using filter_t = typename filter<Pred, List>::type;

}

namespace mitama::dimensional::base_dimension {
  struct length {};
  struct mass {};
  struct time {};
}

namespace mitama::dimensional::mitamagic {
  template <class Base, class Ratio = std::ratio<1>>
  struct dim : Base {
    using dimension_type = dim<Base, Ratio>;
    using base_type = Base;
    using ratio = Ratio;
  };

  template <class...> struct system {};

  template<typename Dim, typename System>
  struct unit;

  template<typename ...Terms, typename System>
  struct unit<type_list<Terms...>, System> {
    using unit_type = unit<type_list<Terms...>, System>;
    using dimension_type = type_list<Terms...>;
    using system_type = System;
  };

  template<typename Dim>
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
    : std::type_identity<unit<type_list<dim<typename Dims::base_type, std::ratio_subtract<std::ratio<0>, typename Dims::ratio>>...>, System>>
  {};

  template <class U>
  using unit_negate = typename unit_negate_impl<U>::type;

  template <class L, class R>
  struct unit_subtract_impl: unit_add_impl<L, unit_negate<R>> {};

  template <class L, class R>
  using unit_subtract = typename unit_subtract_impl<L, R>::type;
}

namespace mitama::dimensional {
  using dimless = mitamagic::type_list<>;

  using length_dimension = mitamagic::type_list<mitamagic::dim<base_dimension::length>>;
  using area_dimension = mitamagic::type_list<mitamagic::dim<base_dimension::length, std::ratio<2>>>;
  using volume_dimension = mitamagic::type_list<mitamagic::dim<base_dimension::length, std::ratio<3>>>;

  using mass_dimension = mitamagic::type_list<mitamagic::dim<base_dimension::mass>>;
  using time_dimension = mitamagic::type_list<mitamagic::dim<base_dimension::time>>;
}

namespace mitama::dimensional {
  template <class, class>
  struct quantity;

  template <class System, class Dimension, class UnderlyingType>
  struct quantity<mitamagic::unit<Dimension, System>, UnderlyingType>
  {
    using unit_type = mitamagic::unit<Dimension, System>;
    using dimension_type = Dimension;
    using system_type = System;

    UnderlyingType value;
  };
}

/// operations
namespace mitama::dimensional {
    template <class Unit, class T, class U>
        requires requires (T t, U u) { t + u; }
    inline constexpr auto operator+(
            quantity<Unit, T> const& lhs,
            quantity<Unit, U> const& rhs)
        -> quantity<Unit, decltype(std::declval<T>() * std::declval<U>())>
    { return {lhs.value + rhs.value}; }

    template <class Unit, class T, class U>
        requires requires (T t, U u) { t - u; }
    inline constexpr auto operator-(
            quantity<Unit, T> const& lhs,
            quantity<Unit, U> const& rhs)
        -> quantity<Unit, decltype(std::declval<T>() * std::declval<U>())>
    { return {lhs.value - rhs.value}; }

    template <class System, class DimL, class T, class DimR, class U>
        requires requires (T t, U u) { t * u; }
    inline constexpr auto operator*(
            quantity<mitamagic::unit<DimL, System>, T> const& lhs,
            quantity<mitamagic::unit<DimR, System>, U> const& rhs)
        -> quantity<
            mitamagic::unit_add<mitamagic::unit<DimL, System>, mitamagic::unit<DimR, System>>,
            decltype(std::declval<T>() * std::declval<U>())>
    { return {lhs.value * rhs.value}; }

    template <class System, class DimL, class T, class DimR, class U>
        requires requires (T t, U u) { t / u; }
    inline constexpr auto operator/(
            quantity<mitamagic::unit<DimL, System>, T> const& lhs,
            quantity<mitamagic::unit<DimR, System>, U> const& rhs)
        -> quantity<
            mitamagic::unit_subtract<mitamagic::unit<DimL, System>, mitamagic::unit<DimR, System>>,
            decltype(std::declval<T>() / std::declval<U>())>
    { return {lhs.value / rhs.value}; }
}

namespace mitama::dimensional::mks {
  struct length_base_unit : mitamagic::base_unit<base_dimension::length> {};
  struct mass_base_unit : mitamagic::base_unit<base_dimension::mass> {};
  struct time_base_unit : mitamagic::base_unit<base_dimension::time> {};

  using mks_system = mitamagic::system<length_base_unit, mass_base_unit, time_base_unit>;

  using length = mitamagic::unit<length_dimension, mks_system>;
  using mass = mitamagic::unit<mass_dimension, mks_system>;
  using time = mitamagic::unit<time_dimension, mks_system>;
}

namespace mitama::dimensional::cgs {
  struct length_base_unit : mitamagic::base_unit<base_dimension::length> {};
  struct mass_base_unit : mitamagic::base_unit<base_dimension::mass> {};
  struct time_base_unit : mitamagic::base_unit<base_dimension::time> {};

  using cgs_system = mitamagic::system<length_base_unit, mass_base_unit, time_base_unit>;

  using length = mitamagic::unit<length_dimension, cgs_system>;
  using mass = mitamagic::unit<length_dimension, cgs_system>;
  using time = mitamagic::unit<length_dimension, cgs_system>;
}

namespace mitama::dimensional::refine {
    template <class Quantity, class Dimension>
    concept has_dimension = std::same_as<typename Quantity::dimension_type, Dimension>;

    template <class Quantity>
    concept length = has_dimension<Quantity, length_dimension>;

    template <class Quantity>
    concept area = has_dimension<Quantity, area_dimension>;

    template <class Quantity>
    concept volume = has_dimension<Quantity, volume_dimension>;
}

