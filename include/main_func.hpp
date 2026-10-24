#pragma once

#include "display/display.hpp"
#include "food.hpp"
#include "snake.hpp"
#include <cstdint>
#include <hardware/gpio.h>
#include <hardware/i2c.h>
#include <pico/time.h>

constexpr uint32_t i2c_baudrate = 1000 * 1000;
constexpr uint32_t i2c_timeout_us = 100 * 1000;

constexpr uint button_left = 0;
constexpr uint button_right = 1;
constexpr uint button_repeat = 2;
constexpr int64_t debounce_time_us = 150000;
constexpr uint32_t display_power_up_ms = 200;
constexpr uint32_t title_screen_ms = 1000;
constexpr uint32_t game_over_delay_ms = 200;
constexpr uint32_t tick_ms = 250;
constexpr uint8_t win_score = 20;

constexpr char title_text[] = "SNAKE";
constexpr char game_over_text[] = "GAME OVER!";
constexpr char win_text[] = "YOU WON!";
constexpr char start_again_text[] = "TO START AGAIN PRESS LEFT BUTTON";
constexpr Point title_pos = {40, 28};
constexpr Point result_pos = {35, 18};
constexpr Point start_again_pos = {25, 28};
constexpr Point score_pos = {0, 0};
constexpr size_t score_text_size = 20;

template <size_t N>
constexpr uint8_t text_len(const char (&)[N]) { return N - 1; }

inline void init_i2c() {
  i2c_init(i2c_default, i2c_baudrate);

  gpio_set_function(PICO_DEFAULT_I2C_SDA_PIN, GPIO_FUNC_I2C);
  gpio_set_function(PICO_DEFAULT_I2C_SCL_PIN, GPIO_FUNC_I2C);

  gpio_pull_up(PICO_DEFAULT_I2C_SDA_PIN);
  gpio_pull_up(PICO_DEFAULT_I2C_SCL_PIN);
}

inline void win(Snake &snake, Food &food, DISPLAY &display, uint8_t score){
  snake.snake_restart();
  food.clear_food();
  SSD1306::String game_over(result_pos, win_text, text_len(win_text));
  SSD1306::String start_again(start_again_pos, start_again_text, text_len(start_again_text));

  char score_text[score_text_size];
  int len = snprintf(score_text, sizeof(score_text), "Score: %lu", static_cast<unsigned long>(score));
  SSD1306::String score_output(score_pos, score_text, len);
  display.add_string(score_output);
  display.add_string(game_over);
  display.add_string(start_again);
  display.print_data();
}

inline void game_over(Snake &snake, Food &food, DISPLAY &display, uint8_t score) {
  snake.update();
  display.print_data();
  sleep_ms(game_over_delay_ms);
  snake.snake_restart();
  food.clear_food();
  SSD1306::String game_over(result_pos, game_over_text, text_len(game_over_text));
  SSD1306::String start_again(start_again_pos, start_again_text, text_len(start_again_text));

  char score_text[score_text_size];
  int len = snprintf(score_text, sizeof(score_text), "Score: %lu", static_cast<unsigned long>(score));
  SSD1306::String score_output(score_pos, score_text, len);
  display.add_string(score_output);
  display.add_string(game_over);
  display.add_string(start_again);
  display.print_data();
}

