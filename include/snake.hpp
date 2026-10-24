#include "direction.hpp"
#include "display/display.hpp"
#include "display/point.hpp"
#include "etl/circular_buffer.h"

#pragma once

constexpr uint8_t segment_size = 8;
constexpr uint8_t grid_cols = disp_width / segment_size;
constexpr uint8_t grid_rows = disp_height / segment_size;
constexpr uint16_t grid_cells = grid_cols * grid_rows;
constexpr uint8_t initial_snake_length = 3;

class Snake
{
public:
  Snake(DISPLAY &display);
  void turn(Direction turn_dir);
  bool go();
  void update();
  void grow();
  void snake_clear();
  void snake_restart();
  const etl::circular_buffer<Point, grid_cells>& get_segment();
  Point get_head(){
    return segments_.back();
  }
private:
  etl::circular_buffer<Point, grid_cells> segments_;
  Direction diretion_;
  DISPLAY &display_;
};
