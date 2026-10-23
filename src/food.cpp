#include "food.hpp"
#include "display/display.hpp"
#include "figure/circle.hpp"
#include "snake.hpp"
#include <cstdint>
#include <pico/rand.h>

namespace {
bool out_of_bounds(Point pos) {
  return ((pos.first < 0) || (pos.second < 0) || (pos.first >= disp_width) || (pos.second >= disp_height));
}
} // namespace

void Food::add_food() {
  while(true) {
    auto segment = get_rand_32();
    segment %= 128;
    uint8_t x = (segment * segment_size) % 128;
    uint8_t y = (segment / 16) * segment_size;

    x_ = x + 4;
    y_ = y + 4;

    SSD1306::Circle food({x_, y_}, 3);
    food.draw(display_);
    display_.print_data();
    break;
  }
}

bool Food::delete_food() {
  auto head = snake_.get_segment().back();
  if((head.first == x_ - 4 && head.second == y_ - 4) ||
     (head.first == x_ - 4 && head.second == y_ + 4) ||
     (head.first == x_ + 4 && head.second == y_ - 4) ||
     (head.first == x_ - 4 && head.second == y_ - 4)) {
    SSD1306::Circle food({x_, y_}, 3);
    food.delete_figure(display_);
    display_.print_data();
    return true;
  }

  return false;
}

Point Food::get_pos() {
  return {x_, y_};
}
