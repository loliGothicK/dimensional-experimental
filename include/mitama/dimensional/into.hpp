#pragma once
#include <concepts>
#include <functional>
#include <mitama/dimensional/quantity.hpp>
#include <mitama/dimensional/core/into_trait.hpp>

namespace mitama::dimensional::core {
  // for implicit unit conversion
  template <quantity_type From>
  struct into_closure {
    From from;

    template <quantity_type To>
          requires std::convertible_to<typename std::remove_cvref_t<From>::value_type, typename To::value_type>
    constexpr operator To() const {
      using trait = into_trait<typename std::remove_cvref_t<From>::unit_type, typename To::unit_type>;
      static_assert(braced_initializable<typename std::remove_cvref_t<From>::value_type, typename To::value_type>,
              "Error: conversion from 'From::value_type' to 'To::value_type' requires a narrowing conversion.");
      return { std::invoke(trait{}, from.value) };
    }
  };

  // for tag dispatch
  template <class Unit>
  struct into_result {};

  // class for ADL magic
  struct into_impl {
    template <quantity_type From, class To>
    friend constexpr auto operator|(From const& from, into_result<To>(*)(into_impl)) {
      if constexpr (std::is_null_pointer_v<To>) {
        return into_closure{from};
      } else {
        using ret_t = quantity<To, typename std::remove_cvref_t<From>::value_type>;
        return ret_t{ std::invoke(into_trait<typename std::remove_cvref_t<From>::unit_type, To>{}, from.value) };
      }
    }
  };
  // Constraints for into_result
  template <class T>
  concept unit_type_or_nullptr = unit_type<T> || std::is_null_pointer_v<T>;
}

namespace mitama::dimensional {
  // Usage: `quantity | into<Unit>` or `quantity | into<>`
  template <core::unit_type_or_nullptr auto To = nullptr>
  inline constexpr auto into(core::into_impl = {})
    -> core::into_result<std::remove_cvref_t<decltype(To)>>
    { return {}; }
}
