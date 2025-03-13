#include "hardware/i2c.h"

// ==============================
// Define display size 
// ==============================

#define DISPLAY_HEIGHT 64
#define DISPLAY_WIDTH  128

// ==============================
// Define command values 
// ==============================
#define SET_CONTRAST        0x81
#define SET_ENTIRE_ON       0xA4
#define SET_NORM_INV        0xA6
#define SET_DISP            0xAE
#define SET_MEM_ADDR        0x20
#define SET_COL_ADDR        0x21
#define SET_PAGE_ADDR       0x22
#define SET_DISP_START_LINE 0x40
#define SET_SEG_REMAP       0xA0
#define SET_MUX_RATIO       0xA8
#define SET_COM_OUT_DIR     0xC0
#define SET_DISP_OFFSET     0xD3
#define SET_COM_PIN_CFG     0xDA
#define SET_DISP_CLK_DIV    0xD5
#define SET_PRECHARGE       0xD9
#define SET_VCOM_DESEL      0xDB
#define SET_CHARGE_PUMP     0x8D


/**
 * @brief Struct for holding display's configuration
 * 
 */
typedef struct
{
    uint8_t height; 
    uint8_t width;  
    uint8_t addr; 
    i2c_inst_t *i2c; 
    uint8_t *buffer; 
    bool external_vcc;
} ssd1306_t;

/**
 * @brief Initializes the SSD1306 OLED display.
 * 
 * @param display Pointer to the display structure.
 * @param i2c Pointer to the i2c instance.
 * @param height Display heigth (usually 64 or 34).
 * @param width Display width (128).
 * @param addr I2C address of the display.
 * @param external_vcc Set to true if using external VCC, false if using internal.
 */
void ssd1306_init(ssd1306_t *display, i2c_inst_t *i2c, uint8_t height, uint8_t width, uint8_t addr, bool external_vcc);

/**
 * @brief De-initializes the SSD1306 OLED display.
 * 
 * @param display Pointer to the display structure.
 */
void ssd1306_deinit(ssd1306_t *display);

/**
 * @brief Send command to display.
 * 
 * @param display Pointer to the display structure.
 * @param command Command code.
 */
void ssd1306_send_command(ssd1306_t *display, uint8_t command);

/**
 * @brief Send data to display.
 * 
 * @param display Pointer to the display structure.
 * @param data Pointer to the data to be sent.
 * @param size Size of the data to be sent.
 */
void ssd1306_send_data(ssd1306_t *display, uint8_t *data, uint16_t size);

/**
 * @brief Update the display screen.
 * 
 * @param display Pointer to the display structure.
 */
void ssd1306_update(ssd1306_t *display);

/**
 * @brief Draw pixel on the screen. 
 * 
 * @param display Pointer to the display structure.
 * @param x X-coordinate.
 * @param y Y-coordinate.
 * @param on Set pixel to on if true, set pixel to off if false.
 */
void ssd1306_draw_pixel(ssd1306_t *display, uint8_t x, uint8_t y, bool on);

/**
 * @brief Clear entire display screen.
 * 
 * @param display Pointer to the display structure.
 */
void ssd1306_clear_display(ssd1306_t *display);

/**
 * @brief Clear a rectangular area.
 * 
 * @param display Pointer to the display structure.
 * @param x0 Rectangles' X initial coordinate.
 * @param y0 Rectangles' Y initial coordinate.
 * @param x1 Rectangles' X final coordinate.
 * @param y1 Rectangles' Y final coordinate.
 */
void ssd1306_clear_rectangle(ssd1306_t *display, uint8_t x0, uint8_t y0, uint8_t x1, uint8_t y1);

/**
 * @brief Draw line. Used Bresenham’s algorithm https://www.baeldung.com/cs/bresenhams-line-algorithm.
 * 
 * @param display Pointer to the display structure.
 * @param x0 Rectangles' X initial coordinate.
 * @param y0 Rectangles' Y initial coordinate.
 * @param x1 Rectangles' X final coordinate.
 * @param y1 Rectangles' Y final coordinate.
 */
void ssd1306_draw_line(ssd1306_t *display, int16_t x0, int16_t y0, int16_t x1, int16_t y1);

/**
 * @brief Draw empty rectangle. 
 * 
 * @param display Pointer to the display structure.
 * @param x0 Rectangles' X initial coordinate.
 * @param y0 Rectangles' Y initial coordinate.
 * @param x1 Rectangles' X final coordinate.
 * @param y1 Rectangles' Y final coordinate.
 */
void ssd1306_draw_empty_rectangle(ssd1306_t *display, uint8_t x0, uint8_t y0, uint8_t x1, uint8_t y1);

/**
 * @brief Draw filled rectangle.
 * 
 * @param display Pointer to the display structure.
 * @param x0 Rectangles' X initial coordinate.
 * @param y0 Rectangles' Y initial coordinate.
 * @param x1 Rectangles' X final coordinate.
 * @param y1 Rectangles' Y final coordinate.
 */
void ssd1306_draw_filled_rectangle(ssd1306_t *display, uint8_t x0, uint8_t y0, uint8_t x1, uint8_t y1);

/**
 * @brief Draw char.
 * 
 * @param display Pointer to the display structure.
 * @param c Charactere to be drawn. 
 * @param x X initial coordinate.
 * @param y Y initial coordinate.
 */
void ssd1306_draw_char(ssd1306_t *display, char c, uint8_t x, uint8_t y);

/**
 * @brief Draw string (text).
 * 
 * @param display Pointer to the display structure.
 * @param text Pointer to the text to be drawn.
 * @param x X initial coordinate.
 * @param y Y initial coordinate.
 */
void ssd1306_draw_string(ssd1306_t *display, const char *text, uint8_t x, uint8_t y);

/**
 * @brief Set display contrast. It is initialized as the maximum value. 
 * 
 * @param display Pointer to the display structure.
 * @param value Contrast value. The value can be from 0x00 to 0xFF.
 */
void ssd1306_set_contrast(ssd1306_t *display, uint8_t value);

/**
 * @brief Invert display show mode. 
 * 
 * If the background is black (pixels off) and the draws on display are white (pixels on), if the invert parameter is set to true
 * then pixels are inverted. 
 * 
 * @param display Pointer to the display structure.
 * @param invert Set to true if you want to invert pixels (from off to on, and vice-versa).
 */
void ssd1306_invert_display(ssd1306_t *display, bool invert);

/**
 * @brief Power on the display.
 * 
 * @param display Pointer to the display structure.
 */
void ssd1306_power_on(ssd1306_t *display);

/**
 * @brief Power off the display.
 * 
 * @param display Pointer to the display structure.
 */
void ssd1306_power_off(ssd1306_t *display);

/* Future upgrades: hardare scrolling support, draw circles support */ 