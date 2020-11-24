#pragma once
#include <ratio>
#include <type_traits>
#include <concepts>
#include <mitama/dimensional/core/type_list.hpp>
#include <mitama/dimensional/core/dimension.hpp>
#include <mitama/dimensional/core/core.hpp>

namespace mitama::dimensional {
  // quantity container
  template </* Phantom type */ class, /* Value type */ core::arithmetic = double>
  struct quantity;

  template <class System, class Dimension, class ValueType>
  struct quantity<
    /* Phantom type */ core::unit<Dimension, System>,
    /*  Value type  */ ValueType>
  {
    using unit_type = core::unit<Dimension, System>;
    using dimension_type = Dimension;
    using system_type = System;
    using value_type = ValueType;

    ValueType value;
  };

  template <class T> struct is_quantity: std::false_type {};
  template <class Unit, class T> struct is_quantity<quantity<Unit, T>>: std::true_type {};
  template <class T> inline constexpr bool is_quantity_v = is_quantity<T>::value;

  template <class T> concept quantity_type = is_quantity_v<T>;
}

namespace mitama::dimensional {
  inline constexpr auto
  operator*(core::arithmetic auto value, core::unit_type auto unit)
  -> quantity<typename decltype(unit)::unit_type, decltype(value)>
  { return {value}; }
}

