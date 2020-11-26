#pragma once
#include <mitama/dimensional/systems/si/base_units.hpp>

namespace mitama::dimensional::si {
  [[maybe_unused]] inline constexpr auto velocity = meters * seconds.recip;
  [[maybe_unused]] inline constexpr auto acceleration = meters * seconds.pow<2>.recip;
}
