#pragma once

#include <iostream>
#include <concepts>
#include <ratio>

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

  template<typename Dim, typename System>

  struct unit {
    using unit_type = unit<Dim, System>;
    using dimension_type = Dim;
    using system_type = System;
  };

  template<typename Dim>
  class base_unit { using dimension_type = Dim; };

  template <class... dims> struct dim_list : dims... {};
}

namespace mitama::dimensional {
  using length_dimension = mitamagic::dim_list<mitamagic::dim<base_dimension::length>>;
  using mass_dimension = mitamagic::dim_list<mitamagic::dim<base_dimension::mass>>;
  using time_dimension = mitamagic::dim_list<mitamagic::dim<base_dimension::time>>;
}

namespace mitama::dimensional {
  template <class, class>
  struct quantity;

  template <class System, class Dimension, class UnderlyingType>
  struct quantity<mitamagic::unit<Dimension, System>, UnderlyingType>
  {
    UnderlyingType value;
  };
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

