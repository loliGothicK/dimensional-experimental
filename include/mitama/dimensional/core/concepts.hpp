#pragma once
#include <type_traits>
#include <concepts>

namespace mitama::dimensional::core
{
  // internal use only
  namespace _secrets
  {
    template <class> struct pair_type_impl: std::false_type {};
    template <template <class, class> class Pair, class T, class U>
    struct pair_type_impl<Pair<T, U>>: std::true_type {};
  } // namespace _secrets
  
  // concept for pair-type requirements
  template <class T> concept pair_type = _secrets::pair_type_impl<T>::value;

  // internal use only
  namespace _secrets
  {
    template <class> struct rational_impl: std::false_type {};
    template <template <auto, auto> class Rational, auto N, auto D>
      requires std::integral<decltype(N)>
            && std::integral<decltype(D)>
            && requires {
              Rational<N, D>::num; // required nested member `num`
              Rational<N, D>::den; // required nested member `den`
            }
    struct rational_impl<Rational<N, D>>: std::true_type {};
  } // namespace _secrets

  // concept for type-level rational type requirements
  template <class T> concept rational = _secrets::rational_impl<T>::value;

  // concept for arithmetic type
  template <class T> concept arithmetic = std::integral<T> or std::floating_point<T>;

  template <bool ...B> concept satisfy_all_of = (... && B);
  template <bool ...B> concept satisfy_any_of = (... || B);
} // namespace mitama::dimensional::core
