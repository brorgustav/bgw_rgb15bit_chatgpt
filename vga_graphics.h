/**
 *   CODIGO ORIGINAL BY Hunter Adams (vha3@cornell.edu)
 *   Modificado by San Tarcisio (https://www.instagram.com/san_tarcisio/)
 * 
 *
 * HARDWARE CONNECTIONS
 *  - GPIO 16 ---> VGA Hsync
 *  - GPIO 17 ---> VGA Vsync
 *  - GPIO 18 ---> 330 ohm resistor ---> VGA Red
 *  - GPIO 19 ---> 330 ohm resistor ---> VGA Green
 *  - GPIO 20 ---> 330 ohm resistor ---> VGA Blue
 *  - RP2040 GND ---> VGA GND
 *
 * RESOURCES USED
 *  - PIO state machines 0, 1, and 2 on PIO instance 0
 *  - DMA channels 0, 1, 2, and 3
 *  - 153.6 kBytes of RAM (for pixel color data)
 *
 * NOTE
 *  - This is a translation of the display primitives
 *    for the PIC32 written by Bruce Land and students
 *
 */


// Give the I/O pins that we're using some names that make sense - usable in main()
// enum vga_pins {HSYNC=16, VSYNC, RED_PIN, GREEN_PIN, BLUE_PIN} ;
//**adapted to vga demo base
enum vga_pins {
  HSYNC = 16,
  VSYNC = 17,
  RGB_BASE_PIN = 0  // Use GPIO0 as the start for Red, then Green and Blue follow.
}
// Converts a 15-bit RGB (RGB555) pixel into a 32-bit word with bits arranged as follows:
//   - Red (bits 10-14 of pixel)  → bits [4:0] (for GPIO0–4)
//   - Green (bits 5-9)  → bits [10:6] (for GPIO6–10)
//   - Blue (bits 0-4)   → bits [15:11] (for GPIO11–15)
// This assumes that the unused GPIO5 remains untouched.
uint32_t map_rgb555_to_gpio_bits(uint16_t rgb) {
    uint32_t mapped = 0;
    mapped |= ((rgb >> 10) & 0x1F) << 0;   // red into bits 0-4 (GPIO0-4)
    mapped |= ((rgb >> 5)  & 0x1F) << 6;    // green into bits 6-10 (GPIO6-10)\n    mapped |= (rgb & 0x1F)        << 11; // blue into bits 11-15 (GPIO11-15)
    return mapped;
}
// We can only produce 8 (3-bit) colors, so let's give them readable names - usable in main()
enum colors {BLACK, RED, GREEN, YELLOW, BLUE, MAGENTA, CYAN, WHITE} ;

// Pixel color array that is DMA's to the PIO machines and
// a pointer to the ADDRESS of this color array.
// Note that this array is automatically initialized to all 0's (black)

// Screen width/height
#define screenWidth 640 //in pixels
#define screenHeight 240 //in pixels

extern unsigned char vga_data_array[];
extern volatile uint32_t currentFrame;  // frame counter

// VGA primitives - usable in main
void initVGA(void) ;
void clearScreen(void);
void nextFrame(void);
void drawPixel(short x, short y, char color) ;
void drawVLine(short x, short y, short h, char color) ;
void drawHLine(short x, short y, short w, char color) ;
void drawLine(short x0, short y0, short x1, short y1, char color) ;
void drawRect(short x, short y, short w, short h, char color);
void drawRectCenter(short x, short y, short w, short h, char color);
void drawCircle(short x0, short y0, short r, char color) ;
void drawCircleHelper( short x0, short y0, short r, unsigned char cornername, char color) ;
void fillCircle(short x0, short y0, short r, char color) ;
void fillCircleHelper(short x0, short y0, short r, unsigned char cornername, short delta, char color) ;
void drawRoundRect(short x, short y, short w, short h, short r, char color) ;
void fillRoundRect(short x, short y, short w, short h, short r, char color) ;
void fillRect(short x, short y, short w, short h, char color) ;
void drawChar(short x, short y, unsigned char c, char color, char bg, unsigned char size) ;
void setTextCursor(short x, short y);
void setTextColor(char c);
void setTextColor2(char c, char bg);
void setTextSize(unsigned char s);
void setTextWrap(char w);
void tft_write(unsigned char c) ;
void writeString(char* str) ;

//conways
int getPixel(short x, short y) ;
