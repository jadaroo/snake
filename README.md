# Snake

Classic Snake for the Raspberry Pi Pico W, drawn on a 128×64 SSD1306 OLED over I2C and controlled with three push buttons.

## Gameplay

- The snake starts with 3 segments in the top-left corner, moving right.
- The field is a 16×8 grid of 8×8 px cells.
- The **left** and **right** buttons turn the snake relative to its current heading.
- Each food eaten grows the snake by one segment and adds a point.
- Hitting a wall or the snake's own body ends the game. Reaching a score of 20 wins.
- After a win or a game over, the result and score are shown. Press the restart button to play again.

## Hardware

| Component        | Pico W pin                      |
|------------------|---------------------------------|
| SSD1306 SDA      | GP4 (`PICO_DEFAULT_I2C_SDA_PIN`) |
| SSD1306 SCL      | GP5 (`PICO_DEFAULT_I2C_SCL_PIN`) |
| Left button      | GP0                             |
| Right button     | GP1                             |
| Restart button   | GP2                             |

- The display is at I2C address `0x3C`, and the bus runs at 1 MHz.
- The buttons are active-low: wire each one between its GPIO and GND. Internal pull-ups are enabled.

## Dependencies

- [Pico SDK](https://github.com/raspberrypi/pico-sdk) (`PICO_SDK_PATH` must be set)
- ARM GCC toolchain (`arm-none-eabi-gcc`)
- [SSD1306-lib](https://github.com/jadaroo/SSD1306-lib), expected at `../SSD1306` next to this project
- [ETL](https://github.com/ETLCPP/etl) 20.49.0, fetched automatically by CMake

## Building

```sh
mkdir build && cd build
cmake ..
make
```

To flash, hold BOOTSEL while plugging in the Pico and copy `build/snake.uf2` to the drive that appears.

USB stdio is enabled, so debug output is available over the USB serial port.

## Project layout

```
include/
  direction.hpp   Direction enum (Up/Right/Down/Left)
  snake.hpp       Snake class and grid constants
  food.hpp        Food class
  main_func.hpp   Game constants, I2C init, win/game-over screens
src/
  main.cpp        Button IRQ handling and the main game loop
  snake.cpp       Movement, turning, growth, collision detection
  food.cpp        Random food placement and eat detection
```

## Configuration

The game parameters are `constexpr` values in `include/main_func.hpp`:

- `tick_ms`: game speed. The default is 250 ms per step.
- `win_score`: the score needed to win. The default is 20.
- `debounce_time_us`: button debounce window.
- `button_left`, `button_right`, `button_repeat`: the button GPIOs.
