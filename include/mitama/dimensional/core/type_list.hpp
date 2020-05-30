#pragma once
#include <type_traits>
#include <concepts>
#include <mitama/dimensional/core/concepts.hpp>

namespace mitama::dimensional::core {
  // type list
  template <class... ElemTypes> struct type_list {};


  // internal use only
  namespace _secrets
  {
    template <class, class> struct findee;

    template <template <class, class> class Pair, class ToFind, class Key, class Value>
    struct findee<ToFind, Pair<Key, Value>>
    {
      static constexpr struct hack {
        constexpr explicit operator bool() const noexcept { return std::is_same_v<ToFind, Key>; }
        constexpr bool operator()() const noexcept { return std::is_same_v<ToFind, Key>; }
        using type = Value;
      } value{};
    };
  } // namespace _secrets


  // * -> * -> [(*, *)] -> *
  template <class, class, class> struct get_value_or;
  template <class Default, class ToFind, class ...Pairs>
    requires core::satisfy_all<core::pair_type<Pairs>...>
  struct get_value_or<Default, ToFind, type_list<Pairs...>>
    : std::conditional<
        std::disjunction_v<_secrets::findee<ToFind, Pairs>...>,
        typename decltype(std::disjunction<_secrets::findee<ToFind, Pairs>...>::value)::type,
        Default
      >
  {};

  template <class Default, class ToFind, class Dict>
  using get_value_or_t = typename get_value_or<Default, ToFind, Dict>::type;

  // Concatenation for TypeList
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
    : concat<typename concat<Pack1st, Pack2nd>::type, PackTail...>
  {};

  template <class... Packs>
  using concat_t = concat<Packs...>::type;


  // (* -> Bool) -> [*] -> [*]
  template <auto, class> struct filter;

  template <auto Pred, template <class...> class List, class ...ElemTypes>
    requires (... && requires { { Pred(std::type_identity<ElemTypes>{}) } -> std::convertible_to<bool>; })
  struct filter<Pred, List<ElemTypes...>>
    : std::type_identity<concat_t<std::conditional_t< Pred(std::type_identity<ElemTypes>{}), type_list<ElemTypes>, type_list<> >...>>
  {};

  template <auto Pred, class List>
  using filter_t = filter<Pred, List>::type;

  template <template <class> class Pred>
  [[maybe_unused]] inline constexpr auto meta_pred = []<class T>(std::type_identity<T>){ return Pred<T>::value; };
}

// Set operations for type_list
namespace mitama::dimensional::core {
  namespace _secrets {
    // Union
    template<class ...Ty>
    struct set_union_impl;

    template<template<class...> class List, class ...Ty>
    struct set_union_impl<List<Ty...>>
            : std::type_identity<List<Ty...>>
    {};
    template <template <class...> class List, class, class = List<>> struct union_helper;

    template <template <class...> class List, class Head, class ...Input, class ...Output>
    struct union_helper<List, List<Head, Input...>, List<Output...>>
            : std::conditional_t<
                    std::disjunction_v<std::is_same<Head, Output>...>,
                    union_helper<List, List<Input...>, List<Output...>>,
                    union_helper<List, List<Input...>, List<Head, Output...>>
            >
    {};

    template <template <class...> class List, class ...Output>
    struct union_helper<List, List<>, List<Output...>>
            : std::type_identity<List<Output...>>
    {};

    template<template<class...> class List1, template<class...> class List2, class ...Ty1, class ...Ty2>
    struct set_union_impl<List1<Ty1...>, List2<Ty2...>>
            : std::type_identity<typename union_helper<type_list, type_list<Ty1..., Ty2...>>::type>
    {};

    template <class First, class Second, class ...Rest>
    struct set_union_impl<First, Second, Rest...>
            : set_union_impl<typename set_union_impl<First, Second>::type, Rest...> {};
  } // _secrets

  template <class ...Lists>
  using set_union = _secrets::set_union_impl<Lists...>::type;
} // mitama::dimensional::core
