#include "food.hpp"
#include "figure/circle.hpp"
#include "snake.hpp"
#include <cstdint>
#include <pico/rand.h>
#include <pico/time.h>
#include <pico/types.h>

constexpr uint8_t food_radius = 3;
constexpr uint8_t food_center_offset = segment_size / 2;

void Food::clear_food() {
  SSD1306::Circle food({x_ + food_center_offset, y_ + food_center_offset}, food_radius);
  food.clear_figure(display_);
}

void Food::add_food() {
  bool is_cell_valid = false;
  while(!is_cell_valid) {
    auto segment = get_rand_32();
    segment %= grid_cells;
    uint8_t x = (segment % grid_cols) * segment_size;
    uint8_t y = (segment / grid_cols) * segment_size;

    x_ = x;
    y_ = y;

    is_cell_valid = true;

    for(auto &s : snake_.get_segment()) {
      if(s.first == x_ && s.second == y_) {
        is_cell_valid = false;
      }
    }
  }

  SSD1306::Circle food({x_ + food_center_offset, y_ + food_center_offset}, food_radius);
  food.draw(display_);
}

bool Food::is_eaten() {
  for(auto& s : snake_.get_segment()){
    if(s.first == x_ && s.second == y_) return true;
  }

  return false;
}

Point Food::get_pos() {
  return {x_, y_};
}
