#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/i2c.h"
#include "ssd1306.h"



#define I2C_PORT i2c1
#define I2C_SDA 14
#define I2C_SCL 15

ssd1306_t display;

void i2c_setup();

int main()
{
    stdio_init_all();

    // I2C need to be set up correctly before using the SSD1306
    i2c_setup();

    // Call the display initialization 
    ssd1306_init(&display, I2C_PORT, 64, 128, 0x3C, false);

    // Draw a pixel at (63,31)
    ssd1306_draw_pixel(&display, 63, 31, true);
    
    // Call display update every time you want show new drawings
    ssd1306_update(&display);

    // Sleep a little bit to appreciate the new pixel beauty :)
    sleep_ms (2000); 

    // Draw horizontal line starting from (0,15) to (127,15)
    ssd1306_draw_line(&display, 0, 15, 127, 15);
    ssd1306_update(&display);

    sleep_ms(2000);

    // Draw rectangle on the display borders 
    ssd1306_draw_empty_rectangle(&display, 0, 0, 127, 63);
    ssd1306_update(&display);

    sleep_ms(2000);

    // Draw filled rectangle starting from (29, 35) to (97,55)
    ssd1306_draw_filled_rectangle(&display, 29, 35, 97, 55);
    ssd1306_update(&display);

    sleep_ms(2000);
    
    // Draw char starting from (2,2) position
    ssd1306_draw_char(&display, 'A', 2, 2);
    ssd1306_update(&display);

    sleep_ms(2000);

    // Clear entire display
    ssd1306_clear_display(&display);
    ssd1306_update(&display);

    sleep_ms(2000);

    // Draw text starting from (10,20) position
    ssd1306_draw_string(&display, "Hello world! :)", 10, 20);
    ssd1306_update(&display);

    sleep_ms(2000);

    // Set new contrast 
    ssd1306_set_contrast(&display, 10);

    sleep_ms(2000);
    
    // Invert display pixels
    ssd1306_invert_display(&display, true);

    
    while (true) {
        // Do nothing
    }
}


void i2c_setup(void){

    // I2C Initialisation. Using it at 400Khz.
    i2c_init(I2C_PORT, 400*1000);
    
    gpio_set_function(I2C_SDA, GPIO_FUNC_I2C);
    gpio_set_function(I2C_SCL, GPIO_FUNC_I2C);
    gpio_pull_up(I2C_SDA);
    gpio_pull_up(I2C_SCL);
}