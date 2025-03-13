#include <stdlib.h>
#include <string.h>
#include "ssd1306.h"
#include "font.h"
#include "hardware/i2c.h"


void ssd1306_init(ssd1306_t *display, i2c_inst_t *i2c, uint8_t height, uint8_t width, uint8_t addr, bool external_vcc){

    display->addr = addr;
    display->i2c = i2c;
    display->height = height;
    display->width = width;
    
    display->buffer = (uint8_t *)malloc(height*width/8);

    if(!display->buffer){
        //Report error
    }


    // inspired from https://github.com/makerportal/rpi-pico-ssd1306
    uint8_t cmds[]= {
        SET_DISP,
        // timing and driving scheme
        SET_DISP_CLK_DIV,
        0x80,
        SET_MUX_RATIO,
        height - 1,
        SET_DISP_OFFSET,
        0x00,
        // resolution and layout
        SET_DISP_START_LINE,
        // charge pump
        SET_CHARGE_PUMP,
        display->external_vcc?0x10:0x14,
        SET_SEG_REMAP | 0x01,           // column addr 127 mapped to SEG0
        SET_COM_OUT_DIR | 0x08,         // scan from COM[N] to COM0
        SET_COM_PIN_CFG,
        width>2*height?0x02:0x12,
        // display
        SET_CONTRAST,
        0xff,
        SET_PRECHARGE,
        display->external_vcc?0x22:0xF1,
        SET_VCOM_DESEL,
        0x30,                           // or 0x40?
        SET_ENTIRE_ON,                  // output follows RAM contents
        SET_NORM_INV,                   // not inverted
        SET_DISP | 0x01,
        // address setting
        SET_MEM_ADDR,
        0x00,  // horizontal
    };

    // Send all commands 
    for(size_t i=0; i<sizeof(cmds); ++i){
        ssd1306_send_command(display, cmds[i]);
    }

    ssd1306_clear_display(display);
    ssd1306_update(display);

}


void ssd1306_deinit(ssd1306_t *display) {
    if (display->buffer) {
        free(display->buffer);  
        display->buffer = NULL;
    }
}


void ssd1306_send_command(ssd1306_t *display, uint8_t command) {
    uint8_t msg[2] = {0x00, command};  // 0x00: Control byte (command mode)
    i2c_write_blocking(display->i2c, display->addr, msg, 2, false);
}


void ssd1306_send_data(ssd1306_t *display, uint8_t *data, uint16_t size) {
    // The first byte of the data is the control byte (0x40 for data mode)
    uint8_t buffer[size + 1];
    buffer[0] = 0x40; // Data mode

    // Copy the data into the buffer
    memcpy(&buffer[1], data, size);

    // Send the data over I2C
    i2c_write_blocking(display->i2c, display->addr, buffer, size + 1, false);
}


void ssd1306_update(ssd1306_t *display) {
    // Set the column address range (from 0 to width-1)
    ssd1306_send_command(display, 0x21); // Set column address command
    ssd1306_send_command(display, 0);    // Start column address
    ssd1306_send_command(display, display->width - 1); // End column address

    // Set the page address range (from 0 to (height/8) - 1)
    ssd1306_send_command(display, 0x22); // Set page address command
    ssd1306_send_command(display, 0);    // Start page address
    ssd1306_send_command(display, (display->height / 8) - 1); // End page address

    // Send the buffer data to the display
    ssd1306_send_data(display, display->buffer, display->width * display->height / 8);
}


void ssd1306_draw_pixel(ssd1306_t *display, uint8_t x, uint8_t y, bool on) {
    
    // Check if provided coordinates are in the bounds of the display.
    if (x >= display->width || y >= display->height) {
        return;
    }

    
    // To better understand this, please refer to the datasheet as the pixels are not mapped row by row.
    // They use a 'page' system, where there is 64/8 = 8 pages. In which page there are 128 columns. 
    // Each byte in the buffer represent 8 vertical bits. So a page is, in fact, composed by 128 columns of 8 vertical bits.
    uint16_t byte_index = (y / 8) * display->width + x; // Calculate the byte index
    uint8_t bit_position = y % 8; // Calculate the bit position in that byte

    if(on)
        display->buffer[byte_index] |= (1 << bit_position); // Set bit to on
    else 
        display->buffer[byte_index] &= ~(1 << bit_position); // Set bit to off
}


void ssd1306_clear_display(ssd1306_t *display) {
    memset(display->buffer, 0, display->width*display->height/8);
}

void ssd1306_clear_rectangle(ssd1306_t *display, uint8_t x0, uint8_t y0, uint8_t x1, uint8_t y1){
    for(int x=0; x < (x1-x0); x++){
        for (int y=0; y < (y1-y0); y++){
            ssd1306_draw_pixel(display, x, y, false);
        }
    }
}

void ssd1306_draw_line(ssd1306_t *display, int16_t x0, int16_t y0, int16_t x1, int16_t y1) {
    int16_t dx = abs(x1 - x0);  
    int16_t dy = abs(y1 - y0);  
    int16_t sx = (x0 < x1) ? 1 : -1;  
    int16_t sy = (y0 < y1) ? 1 : -1;  
    int16_t err = dx - dy;  

    while (1) {
        ssd1306_draw_pixel(display, x0, y0, true); 

        if (x0 == x1 && y0 == y1) break; 

        int16_t e2 = 2 * err; 

        if (e2 > -dy) { 
            err -= dy;
            x0 += sx;
        }
        if (e2 < dx) { 
            err += dx;
            y0 += sy;
        }
    }
}

void ssd1306_draw_empty_rectangle(ssd1306_t *display, uint8_t x0, uint8_t y0, uint8_t x1, uint8_t y1){

    ssd1306_draw_line(display, x0, y0, x1, y0); // Higher horizontal line
    ssd1306_draw_line(display, x0, y1, x1, y1); // Lower horizontal line 
    ssd1306_draw_line(display, x0, y0, x0, y1); // Vertical line on the left 
    ssd1306_draw_line(display, x1, y0, x1, y1); // Vertical line on the right 

}

void ssd1306_draw_filled_rectangle(ssd1306_t *display, uint8_t x0, uint8_t y0, uint8_t x1, uint8_t y1){

    if(x1<x0){
        uint8_t temp = x0;
        x0 = x1;
        x1 = temp;
    } 

    if(y1<y0){
        uint8_t temp = y0;
        y0 = y1;
        y1 = temp;
    }

    for(int x=x0; x < x1; x++){
        for (int y=y0; y < y1; y++){
            ssd1306_draw_pixel(display, x, y, true);
        }
    }
}

void ssd1306_draw_char(ssd1306_t *display, char c, uint8_t x, uint8_t y) {
    if (c < font_8x5[3] || c > font_8x5[4]) return;  // Check if it is supported
    
    uint8_t char_width = font_8x5[1];  // get font width
    uint8_t char_height = font_8x5[0]; // get font height
    uint16_t index = 5 + (c - 32) * char_width;  // Skip header (5 bytes)

    for (uint8_t col = 0; col < char_width; col++) {
        uint8_t column_data = font_8x5[index + col];  

        for (uint8_t row = 0; row < char_height; row++) {
            if (column_data & (1 << row)) {  // Check bit for pixel state
                ssd1306_draw_pixel(display, x + col, y + row, true);
            } else {
                ssd1306_draw_pixel(display, x + col, y + row, false); // Ensure the char will be visible by cleaning around it
            }
        }
    }
}

void ssd1306_draw_string(ssd1306_t *display, const char *text, uint8_t x, uint8_t y){
    while(*text){ // When it is null (\0) stop; 
        ssd1306_draw_char(display, *text, x, y);
        x = x + font_8x5[1] + 1; // Update position by using char width + 1 
        text++;
    }
}

void ssd1306_set_contrast(ssd1306_t *display, uint8_t value){
    
    ssd1306_send_command(display, SET_CONTRAST);
    ssd1306_send_command(display, value);

}

void ssd1306_invert_display(ssd1306_t *display, bool invert){
    if(invert){
        ssd1306_send_command(display, SET_NORM_INV | 0x01);
    } else {
        ssd1306_send_command(display, SET_NORM_INV);
    }
}

void ssd1306_power_on(ssd1306_t *display){
    ssd1306_send_command(display, SET_DISP);
}

void ssd1306_power_off(ssd1306_t *display){
    ssd1306_send_command(display, SET_DISP | 0x01);
}