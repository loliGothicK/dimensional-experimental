#pragma once

namespace mitama::dimensional {
  // This class is a trait class for describing the rules of unit conversion.
  // In order to describe new conversion rules,
  // you can partially or fully specialize this class to implement with the following signature.
  //
  // [ -- Example: impl trait for conversion from meter to millimeter.
  //      template <>
  //      struct into_trait<meter, millimeter> {
  //          constexpr auto operator()(core::arithmetic auto from) { return from * 1000; };
  //      };
  //  -- end example ]
  template <class /* From */, class /* To */>
  struct into_trait;

  // Partial specialization for identity conversions
  template <class U>
  struct into_trait<U, U> {
      constexpr auto operator()(core::arithmetic auto from) { return from; };
  };
}
