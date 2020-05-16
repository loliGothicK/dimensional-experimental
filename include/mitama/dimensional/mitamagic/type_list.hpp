#pragma once
#include <type_traits>
#include <concepts>
#include <mitama/dimensional/mitamagic/concepts.hpp>

namespace mitama::dimensional::mitamagic {
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
        constexpr operator bool() const noexcept { return std::is_same_v<ToFind, Key>; }
        constexpr bool operator()() const noexcept { return std::is_same_v<ToFind, Key>; }
        using type = Value;
      } value{};
    };
  } // namespace _secrets


  // * -> * -> [(*, *)] -> *
  template <class, class, class> struct get_value_or;
  template <class Default, class ToFind, class ...Pairs>
    requires mitama::satisfy_all<mitama::pair_type<Pairs>...>
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
    : concat<typename concat<Pack1st, Pack2nd>::type, PackTail...> {
  };

  template <class... Packs>
  using concat_t = typename concat<Packs...>::type;


  // (* -> Bool) -> [*] -> [*]
  template <auto, class> struct filter;

  template <auto Pred, template <class...> class List, class ...ElemTypes>
    requires (requires { { Pred(std::type_identity<ElemTypes>{}) } -> std::convertible_to<bool>; } && ...)
  struct filter<Pred, List<ElemTypes...>>
    : std::type_identity<concat_t<std::conditional_t< Pred(std::type_identity<ElemTypes>{}), type_list<ElemTypes>, type_list<> >...>>
  {};

  template <auto Pred, class List>
  using filter_t = typename filter<Pred, List>::type;
}
