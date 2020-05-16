#pragma once

namespace mitama
{
  template <class> struct pair_type_impl: std::false_type {};
  template <template <class, class> class Pair, class T, class U>
  struct pair_type_impl<Pair<T, U>>: std::true_type {};

  // concept for pair-type requirement
  template <class T> concept pair_type = pair_type_impl<T>::value;

  template <bool ...B> concept satisfy_all = (B && ...);
  template <bool ...B> concept satisfy_any = (B || ...);
} // namespace mitama
