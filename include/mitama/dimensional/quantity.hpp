#pragma once
#include <ratio>
#include <type_traits>
#include <concepts>
#include <mitama/dimensional/core/type_list.hpp>
#include <mitama/dimensional/core/dimension.hpp>
#include <mitama/dimensional/core/core.hpp>

namespace mitama::dimensional {
  // quantity container
  template </* Phantom type */ class, /* Value type */ class>
  struct quantity;

  template <class System, class Dimension, class UnderlyingType>
  struct quantity<
    /* Phantom type */ core::unit<Dimension, System>,
    /*  Value type  */ UnderlyingType>
  {
    using unit_type = core::unit<Dimension, System>;
    using dimension_type = Dimension;
    using system_type = System;

    UnderlyingType value;
  };
}
