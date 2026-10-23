#include "direction.hpp"
#include "display/display.hpp"
#include "display/point.hpp"
#include "etl/circular_buffer.h"

#pragma once

constexpr uint8_t segment_size = 8;

class Snake
{
public:
  Snake(DISPLAY &display);
  bool turn(Direction turn_dir);
  bool go();
  void update();
  const etl::circular_buffer<Point, 128>& get_segment();
  Point get_head(){
    return segments_.back();
  }
private:
  etl::circular_buffer<Point, 128> segments_;
  Direction diretion_;
  DISPLAY &display_;
};
