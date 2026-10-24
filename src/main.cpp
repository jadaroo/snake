#include "direction.hpp"
#include "display/display.hpp"
#include "display/strings.hpp"
#include "food.hpp"
#include "i2c_bus/i2c_transport.hpp"
#include "snake.hpp"
#include <cstdint>
#include <hardware/gpio.h>
#include <hardware/i2c.h>
#include <pico/platform/common.h>
#include <pico/stdlib.h>
#include <pico/time.h>
#include <pico/types.h>
#include "main_func.hpp"

constexpr uint8_t display_address = 0x3C;

volatile bool turn = false;
volatile Direction turn_dir;
volatile absolute_time_t last_press = 0;
volatile bool is_food = false;
volatile bool is_init = true;
volatile uint8_t score = 0;

void button_callback(uint gpio, uint32_t events) {
  auto now = get_absolute_time();
  if(absolute_time_diff_us(last_press, now) < debounce_time_us) return;

  if(gpio == button_left && (events & GPIO_IRQ_EDGE_FALL)) {
    turn = true;
    turn_dir = Direction::Left;
  } else if(gpio == button_right && (events & GPIO_IRQ_EDGE_FALL)) {
    turn = true;
    turn_dir = Direction::Right;
  } else if(gpio == button_repeat && (events & GPIO_IRQ_EDGE_FALL)) {
    is_init = true;
    is_food = false;
  }

  last_press = now;
}

void buttons_init(){
  gpio_init(button_left);
  gpio_set_dir(button_left, GPIO_IN);
  gpio_pull_up(button_left);

  gpio_init(button_right);
  gpio_set_dir(button_right, GPIO_IN);
  gpio_pull_up(button_right);

  gpio_init(button_repeat);
  gpio_set_dir(button_repeat, GPIO_IN);
  gpio_pull_up(button_repeat);

  gpio_set_irq_enabled_with_callback(button_left, GPIO_IRQ_EDGE_FALL, true, &button_callback);

  gpio_set_irq_enabled(button_right, GPIO_IRQ_EDGE_FALL, true);

  gpio_set_irq_enabled(button_repeat, GPIO_IRQ_EDGE_FALL, true);
}

void init_game(DISPLAY &display){
  score = 0;
  display.clear_display();
  SSD1306::String title(title_pos, title_text, text_len(title_text));
  display.add_string(title);
  display.print_data();

  sleep_ms(title_screen_ms);

  display.delete_string(title);
  display.print_data();
  is_init = false;
}

int main() {
  stdio_init_all();

  init_i2c();

  buttons_init();

  I2CTRANSPORT i2c(i2c_default, display_address, i2c_timeout_us);
  DISPLAY display(&i2c);

  sleep_ms(display_power_up_ms);
  display.init_disp();

  Snake snake(display);
  Food food(snake, display);

  while(true) {
    if(is_init) {
      init_game(display);
    }

    if(!is_food) {
      food.add_food();
      is_food = true;
    }

    if(turn) {
      snake.turn(turn_dir);
      turn = false;
    }

    if(food.is_eaten()) {
      snake.grow();
      score += 1;
      food.clear_food();
      is_food = false;
    } else{
      bool go = snake.go();
      if(!go) {
        game_over(snake, food, display, score);
        while(!is_init);
        continue;
      }

      if(score == win_score){
        win(snake, food, display, score);
        while(!is_init);
        continue;
      }
    }

    snake.update();
    display.print_data();
    sleep_ms(tick_ms);
  }
}
