#pragma once
#include <type_traits>
#include <concepts>

namespace mitama
{
  template <class> struct pair_type_impl: std::false_type {};
  template <template <class, class> class Pair, class T, class U>
  struct pair_type_impl<Pair<T, U>>: std::true_type {};

  template <class> struct rational_impl: std::false_type {};
  template <template <auto, auto> class Rational, auto N, auto D>
    requires std::integral<decltype(N)> && std::integral<decltype(D)>
  struct rational_impl<Rational<N, D>>: std::true_type {};

  template <class T> concept rational = rational_impl<T>::value;
  
  // concept for pair-type requirement
  template <class T> concept pair_type = pair_type_impl<T>::value;

  template <bool ...B> concept satisfy_all = (B && ...);
  template <bool ...B> concept satisfy_any = (B || ...);
} // namespace mitama
