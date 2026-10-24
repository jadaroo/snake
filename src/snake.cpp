#include "snake.hpp"
#include "direction.hpp"
#include "display/display.hpp"
#include "display/point.hpp"
#include "figure/rect.hpp"
#include <cstdint>
#include <etl/circular_buffer.h>

namespace{
  Direction update_dir_right(const Direction &dir){
    return static_cast<Direction>((static_cast<uint8_t>(dir) + 1) % direction_count);
  }

  Direction update_dir_left(const Direction &dir){
    return static_cast<Direction>((static_cast<uint8_t>(dir) + direction_count - 1) % direction_count);
  }

  bool out_of_bounds(Point pos) {
    return ((pos.first < 0) || (pos.second < 0) || (pos.first >= disp_width) || (pos.second >= disp_height));
  }
}

const etl::circular_buffer<Point, grid_cells>& Snake::get_segment(){
  return segments_;
}

Snake::Snake(DISPLAY &display) : diretion_(Direction::Right), display_(display) {
  for(uint8_t i = 0; i < initial_snake_length; i++) {
    segments_.push({i * segment_size, 0});
  }
}

void Snake::turn(Direction turn_dir) {
  auto head = segments_.back();
  SSD1306::Rect tail = {segments_.front(), segment_size, segment_size};
  tail.delete_figure(display_);
  segments_.pop();
  Point new_head;
  switch(diretion_) {
  case Direction::Up:
    if(turn_dir == Direction::Right) {
      new_head = {head.first + segment_size, head.second};
    } else if(turn_dir == Direction::Left) {
      new_head = {head.first - segment_size, head.second};
    }
    break;
  case Direction::Right:
    if(turn_dir == Direction::Right) {
      new_head = {head.first, head.second + segment_size};
    } else if(turn_dir == Direction::Left) {
      new_head = {head.first, head.second - segment_size};
    }
    break;
  case Direction::Down:
    if(turn_dir == Direction::Right) {
      new_head = {head.first - segment_size, head.second};
    } else if(turn_dir == Direction::Left) {
      new_head = {head.first + segment_size, head.second};
    }
    break;
  case Direction::Left:
    if(turn_dir == Direction::Right) {
      new_head = {head.first, head.second - segment_size};
    } else if(turn_dir == Direction::Left) {
      new_head = {head.first, head.second + segment_size};
    }
    break;
  }

  if(turn_dir == Direction::Right){
    diretion_ = update_dir_right(diretion_);
  } else{
    diretion_ = update_dir_left(diretion_);
  }
  
  segments_.push(new_head);
}

void Snake::snake_restart(){
  snake_erase();
  segments_.clear();
  for(uint8_t i = 0; i < initial_snake_length; i++) {
    segments_.push({i * segment_size, 0});
  }
  diretion_ = Direction::Right;
}

bool Snake::go() {
  auto head = segments_.back();
  SSD1306::Rect tail = {segments_.front(), segment_size, segment_size};
  tail.delete_figure(display_);
  segments_.pop();
  Point new_head;
  
  switch(diretion_) {
  case Direction::Up:
    new_head = {head.first, head.second - segment_size};
    break;
  case Direction::Right:
    new_head = {head.first + segment_size, head.second};
    break;
  case Direction::Down:
    new_head = {head.first, head.second + segment_size};
    break;
  case Direction::Left:
    new_head = {head.first - segment_size, head.second};
    break;
  }
  
  for(auto &s : segments_) {
    if(s.first == new_head.first && s.second == new_head.second){
      segments_.push(new_head);
      return false;
    }
  }

  if(out_of_bounds(new_head)){
    return false;
  }

  segments_.push(new_head);
  
  return true;
}

void Snake::grow(){
  auto head = segments_.back();
  Point new_head;
  
  switch(diretion_) {
  case Direction::Up:
    new_head = {head.first, head.second - segment_size};
    break;
  case Direction::Right:
    new_head = {head.first + segment_size, head.second};
    break;
  case Direction::Down:
    new_head = {head.first, head.second + segment_size};
    break;
  case Direction::Left:
    new_head = {head.first - segment_size, head.second};
    break;
  }

  segments_.push(new_head);
}

void Snake::update() {
  for(auto &s : segments_) {
    SSD1306::Rect segment(s, segment_size, segment_size);
    segment.draw(display_);
  }
}

void Snake::snake_erase(){
  for(auto &s : segments_) {
    SSD1306::Rect segment(s, segment_size, segment_size);
    segment.delete_figure(display_);
  }
}
