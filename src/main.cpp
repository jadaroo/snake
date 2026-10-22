#include "direction.hpp"
#include "display/display.hpp"
#include "display/strings.hpp"
#include "i2c_bus/i2c_transport.hpp"
#include "snake.hpp"
#include <cstdint>
#include <cstdio>
#include <hardware/gpio.h>
#include <hardware/i2c.h>
#include <pico/platform/common.h>
#include <pico/stdlib.h>
#include <pico/time.h>
#include <pico/types.h>

constexpr uint8_t display_address = 0x3C;
constexpr uint32_t i2c_baudrate = 1000 * 1000;
constexpr uint32_t i2c_timeout_us = 100 * 1000;
constexpr uint button_left = 0;
constexpr uint button_right = 1;
constexpr int64_t debounce_time_us = 150000; 

volatile bool turn = false;
volatile Direction turn_dir;
volatile absolute_time_t last_press = 0;

void init_i2c() {
  i2c_init(i2c_default, i2c_baudrate);

  gpio_set_function(PICO_DEFAULT_I2C_SDA_PIN, GPIO_FUNC_I2C);
  gpio_set_function(PICO_DEFAULT_I2C_SCL_PIN, GPIO_FUNC_I2C);

  gpio_pull_up(PICO_DEFAULT_I2C_SDA_PIN);
  gpio_pull_up(PICO_DEFAULT_I2C_SCL_PIN);
}

void turn_callback(uint gpio, uint32_t events){
  auto now = get_absolute_time();
  if(absolute_time_diff_us(last_press, now) < debounce_time_us) return;
  
  if(gpio == button_left && (events & GPIO_IRQ_EDGE_FALL)){
    turn = true;
    turn_dir = Direction::Left;
  } else if(gpio == button_right && (events & GPIO_IRQ_EDGE_FALL)){
    turn = true;
    turn_dir = Direction::Right;
  }
  
  last_press = now;
}

int main() {
  stdio_init_all();

  init_i2c();

  gpio_init(button_left);
  gpio_set_dir(button_left, GPIO_IN);
  gpio_pull_up(button_left);

  gpio_init(button_right);
  gpio_set_dir(button_right, GPIO_IN);
  gpio_pull_up(button_right);

  gpio_set_irq_enabled_with_callback(button_left, GPIO_IRQ_EDGE_FALL, true, &turn_callback);

  gpio_set_irq_enabled(button_right, GPIO_IRQ_EDGE_FALL, true);

  I2CTRANSPORT i2c(i2c_default, display_address, i2c_timeout_us);
  DISPLAY display(&i2c);

  sleep_ms(200);
  display.init_disp();

  SSD1306::String title({40, 28}, "SNAKE", 5);
  display.add_string(title);
  display.print_data();

  sleep_ms(1000);

  display.delete_string(title);
  display.print_data();

  Snake snake(display);

  while(true) {
    if(turn){
      printf("Turn dir: %u\n", turn_dir);
      bool turn_snake = snake.turn(turn_dir);
      if(!turn_snake){
        SSD1306::String game_over({35, 28}, "GAME OVER!", 10);
        display.add_string(game_over);
        display.print_data();
        while (true);
      } 
      turn = false;
    }
    bool go = snake.go();
    if(!go){
      SSD1306::String game_over({35, 28}, "GAME OVER!", 10);
      display.add_string(game_over);
      display.print_data();
      while (true);
    }
    snake.update();
    
    display.print_data();
    sleep_ms(100);
  }
}
