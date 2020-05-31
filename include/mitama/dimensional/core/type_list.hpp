#pragma once
#include <type_traits>
#include <concepts>
#include <mitama/dimensional/core/concepts.hpp>

namespace mitama::dimensional::core {
  template <class... ElemTypes> struct type_list {};

  // internal only
  namespace _secrets
  {
    template <class, class> struct disj_hack;

    template <template <class, class> class Pair, class ToFind, class Key, class Value>
    struct disj_hack<ToFind, Pair<Key, Value>>
    {
      static constexpr struct hack {
        constexpr operator bool() const noexcept { return std::is_same_v<ToFind, Key>; }
        using type = Value;
      } value{};
    };
  } // namespace _secrets

  namespace _secrets {
    // * -> * -> [(*, *)] -> *
    template <class K, class V>
    struct my_pair;

    template <class, class, class> struct get_or_default_impl;
    template <class ToFind, class Default, class ...Pairs>
    struct get_or_default_impl<ToFind, Default, type_list<Pairs...>>
            : std::type_identity<typename decltype(std::disjunction<
                    _secrets::disj_hack<ToFind, Pairs>...,                  // terms
                    _secrets::disj_hack<Default, my_pair<Default, Default>> // gurdian
            >::value)::type>
    {};
  }

  template <class Default, class ToFind, class Dict>
  using get_or_default = _secrets::get_or_default_impl<Default, ToFind, Dict>::type;

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
