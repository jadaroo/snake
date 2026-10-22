#include "direction.hpp"
#include "display/display.hpp"
#include "etl/circular_buffer.h"

#pragma once

constexpr uint8_t segment_size = 4;

class Snake
{
public:
  Snake(DISPLAY &display);
  bool turn(Direction turn_dir);
  bool go();
  void update();
  Point get_head(){
    return segments_.back();
  }
private:
  etl::circular_buffer<Point, 512> segments_;
  Direction diretion_;
  DISPLAY &display_;
};
