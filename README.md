# SSD1306 OLED Display Library

A lightweight C library for interfacing with the SSD1306 OLED display using I2C on embedded systems. 

The creation of this library was proposed as an project for the Capacitação em Sistemas Embarcados do Virtus-CC / UFCG. It can be used 
with the BitDogLab board.

## Features

- Initialize and deinitialize the display
- Draw individual pixels
- Draw lines and rectangles (empty and filled)
- Display characters and strings
- Clear the entire screen or specific areas
- Adjust contrast and invert the display
- Power on/off the display

## Requirements

- Embedded system with I2C support (e.g., Raspberry Pi Pico, STM32, ESP32)
- SSD1306 OLED display (128x64 or 128x32 resolution)
- Pico SDK or equivalent for your platform

## Installation

Clone or download this repository and add the library to your project:

```sh
 git clone <repository-url>
```

Include the library in your project:

```c
#include "ssd1306.h"
```

## Usage

### Initialization

```c
ssd1306_t display;
ssd1306_init(&display, i2c_default, 64, 128, 0x3C, false);
```

### Drawing Pixels

```c
ssd1306_draw_pixel(&display, 10, 10, true);
ssd1306_update(&display);
```

### Drawing a Line

```c
ssd1306_draw_line(&display, 0, 0, 127, 63);
ssd1306_update(&display);
```

### Displaying Text

```c
ssd1306_draw_string(&display, "Hello!", 10, 20);
ssd1306_update(&display);
```

### Clearing the Display

```c
ssd1306_clear_display(&display);
ssd1306_update(&display);
```

## API Reference

### Display Control

- `void ssd1306_init(ssd1306_t *display, i2c_inst_t *i2c, uint8_t height, uint8_t width, uint8_t addr, bool external_vcc);`
- `void ssd1306_deinit(ssd1306_t *display);`
- `void ssd1306_update(ssd1306_t *display);`
- `void ssd1306_power_on(ssd1306_t *display);`
- `void ssd1306_power_off(ssd1306_t *display);`

### Drawing Functions

- `void ssd1306_draw_pixel(ssd1306_t *display, uint8_t x, uint8_t y, bool on);`
- `void ssd1306_draw_line(ssd1306_t *display, int16_t x0, int16_t y0, int16_t x1, int16_t y1);`
- `void ssd1306_draw_empty_rectangle(ssd1306_t *display, uint8_t x0, uint8_t y0, uint8_t x1, uint8_t y1);`
- `void ssd1306_draw_filled_rectangle(ssd1306_t *display, uint8_t x0, uint8_t y0, uint8_t x1, uint8_t y1);`
- `void ssd1306_clear_display(ssd1306_t *display);`

### Text Functions

- `void ssd1306_draw_char(ssd1306_t *display, char c, uint8_t x, uint8_t y);`
- `void ssd1306_draw_string(ssd1306_t *display, const char *text, uint8_t x, uint8_t y);`

### Additional Functions

- `void ssd1306_set_contrast(ssd1306_t *display, uint8_t value);`
- `void ssd1306_invert_display(ssd1306_t *display, bool invert);`

## Future Enhancements

- Hardware scrolling support
- Circle drawing support


## Contributing

Feel free to submit issues or pull requests to improve the library!


