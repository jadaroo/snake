#pragma once

#include "display/display.hpp"
#include "display/point.hpp"
#include "snake.hpp"
#include <cstdint>

class Food{
  public:
    Food(Snake &snake, DISPLAY &display) : snake_(snake), display_(display){}
    void add_food();
    bool delete_food();
    Point get_pos();
  private:
    Snake &snake_;
    volatile uint8_t x_;
    volatile uint8_t y_;
    DISPLAY &display_;
};