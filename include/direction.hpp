#pragma once
#include <cstdint>

enum class Direction : uint8_t {
  Up = 0,
  Right = 1,
  Down = 2,
  Left = 3
};

constexpr uint8_t direction_count = 4;
