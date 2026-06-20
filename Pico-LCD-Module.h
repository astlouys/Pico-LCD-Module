
#ifndef _LCD_MODULE_
#define _LCD_MODULE_
/* ================================================================================================================================================================= *\
   Pico-LCD-Module.h
   St-Louys Andre - June 2023
   Revision 17-JUN-2026
   Langage: Linux gcc
   Version 3.00

   REVISION HISTORY:
   =================
   20-JAN-2021 1.00 - Original source code from Waveshare.
   09-JUN-2023 2.00 - Initial release by Andre St-Louys.
   28-FEB-2025 3.00 - Add support for Pimoroni's Pico-Explorer.
   12-JUN-2025 3.01 - Modify / adjust code to make it compliant to other Pico-Xxx-Module library members.
\* ================================================================================================================================================================= */

/* ================================================================================================================================================================= *\
                      C-Language module to integrate a Pimoroni's Pico Explorer or a Waveshare's 2-inch Pico LCD color display to an existing program.

                                                          Copyright notice from Waveshare's original V1.00:

       Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal
         in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
                     copies of the Software, and to permit persons to  whom the Software is furnished to do so, subject to the following conditions:

                      The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.

          THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
           FITNESS OR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
                               LIABILITY WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION
                                                       WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
\* ================================================================================================================================================================= */



/* ================================================================================================================================================================= *\
                                                                              Include files.
\* ================================================================================================================================================================= */
#include "hardware/spi.h"
#include "hardware/pwm.h"
#include "pico/stdlib.h"
#include "stdarg.h"



/* ================================================================================================================================================================= *\
                                                                             Definitions.
\* ================================================================================================================================================================= */
/* Display mirroring. */
typedef enum
{
  MIRROR_NONE       = 0x00,
  MIRROR_HORIZONTAL = 0x01,
  MIRROR_VERTICAL   = 0x02,
  MIRROR_ORIGIN     = 0x03,
} MIRROR_IMAGE;
#define MIRROR_IMAGE_DFT MIRROR_NONE

/* Size of a point to draw. */
typedef enum
{
  DOT_PIXEL_1X1  = 1,  // 1 x 1
  DOT_PIXEL_2X2,       // 2 X 2
  DOT_PIXEL_3X3,       // 3 X 3
  DOT_PIXEL_4X4,       // 4 X 4
  DOT_PIXEL_5X5,       // 5 X 5
  DOT_PIXEL_6X6,       // 6 X 6
  DOT_PIXEL_7X7,       // 7 X 7
  DOT_PIXEL_8X8,       // 8 X 8
} DOT_PIXEL;
#define DOT_PIXEL_DFT  DOT_PIXEL_1X1  // default dot pixel

/* Point size fill style. */
typedef enum
{
  DOT_FILL_AROUND = 1,  // dot pixel 1 x 1
  DOT_FILL_RIGHTUP,     // dot pixel 2 X 2
} DOT_STYLE;
#define DOT_STYLE_DFT  DOT_FILL_AROUND  // default dot style

/* Whether the graphic is filled or not. */
typedef enum
{
  DRAW_FILL_EMPTY = 0,
  DRAW_FILL_FULL,
} DRAW_FILL;

/* Line style, solid or dashed. */
typedef enum
{
  LINE_STYLE_SOLID = 0,
  LINE_STYLE_DOTTED,
} LINE_STYLE;

#define LCD_RGB_444  1  //   4k colors  (supported by ST7789V but not implemented in firmware).
#define LCD_RGB_565  2  //  65k colors
#define LCD_RGB_666  3  // 262k colors  (supported by ST7789V but not implemented in firmware - Framebuffer in Pico's memory would require almost all available RAM).

/* Color definitions. */
/* NOTE: 65k colors may be defined by specifying specific pixel definitions. */
#define BLACK        0x0000
#define RED          0x00F8
#define GREEN        0xE007
#define BLUE         0x1F00
#define CYAN         0xFF7F
#define MAGENTA      0x1FF8
#define YELLOW       0xE0FF
#define BROWN        0x40BC
#define BROWN_RED    0x07FC
#define GREY         0x3084
#define DARK_BLUE    0x1000
#define DARK_GREEN   0x4002
#define GREEN_YELLOW 0x3007
#define TURQUOISE    0xD025
#define JEANS_BLUE   0x502C
#define LIGHT_GREEN  0x9036
#define WHITE        0xFFFF
#define MAX_COLORS       17


/* LCD display parameters. */
struct struct_lcd
{
  UINT16  PhysicalHeight;     // physical height of display in native mode (portrait).
  UINT16  PhysicalWidth;      // physical width  of display in native mode (portrait).
  UINT16  Height;             // current LCD display height (depend on rotation setting).
  UINT16  Width;              // current LCD display width  (depend on rotation setting).
  UINT16 *FrameBuffer;        // pointer to RAM framebuffer.
  UINT16  LineOffset;         // line pixel offset (for LCD displays smaller than 320 X 240).
  UINT16  ColumnOffset;       // colomn pixel offset (for LCD displays smaller than 320 X 240).
  UINT16  Brightness;         // current LCD display brightness setting.
  UINT16  Rotation;           // current LCD display rotation setting.
  UINT16  MirrorMode;         // current LCD display mirror setting.
  UINT8   Depth;              // LCD display color depth (currently 2-bytes is supported).
  UINT8   DeviceType;         // display type.
  UINT8   GPIOButton[4];      // specify GPIO's used for each device button.
  UINT8   GPIODataCommand;    // data / command control pin.
  UINT8   GPIOChipSelect;     // chip select pin, active low.
  UINT8   GPIOClock;          // SPI clock pin.
  UINT8   GPIOData;           // SPI data pin.
  UINT8   GPIOReset;          // reset pin, active low.
  UINT8   GPIOBrightness;     // backlight pwm pin.
  spi_inst_t *SPIPort;        // SPI port used for communication with LCD display.
  UINT16  Color[MAX_COLORS];  // some specific color definitions.
  UCHAR Mirroring[4][15];     // mirror mode name.
};


typedef struct _tFont
{    
  UINT8 *Table;
  UINT16 Width;
  UINT16 Height;
} sFONT;



/* Scan direction when refreshing LCD display. */
#define HORIZONTAL 0
#define VERTICAL   1

/* Rotation definitions. */
#define ROTATE_0     0
#define ROTATE_90   90
#define ROTATE_180 180
#define ROTATE_270 270

#define IMAGE_BACKGROUND    WHITE
#define FONT_FOREGROUND     BLACK
#define FONT_BACKGROUND     WHITE

#define ARRAY_LEN 255



/* Default different display types and screen dimensions. */
#define LCD_114       1  // Waveshare's 1.14 inch LCD display.
#define LCD_200       2  // Waveshare's 2 inches LCD display (320 X 240)
#define LCD_EXPLORER  3  // Pimoroni's Pico Explorer (240 X 240).

#define MAX_BUTTONS   4
#define BUTTON0       0
#define BUTTON1       1
#define BUTTON2       2
#define BUTTON3       3

#define LONG_PRESS_MASK      0x80
#define VERY_LONG_PRESS_MASK 0xC0

#define MAX_SCREEN_WIDTH   120  // this is for log terminal screen (not LED display).


/* ================================================================================================================================================================= *\
                                                                            Global variables.
\* ================================================================================================================================================================= */




/* ================================================================================================================================================================= *\
                                                                        Function prototypes.
\* ================================================================================================================================================================= */
/* Clear the Framebuffer while setting default color. */
void LCD_clear(UINT16 Color);

/* Clear LCD display without modifying Framebuffer. */
void LCD_cls(UINT16 Color);

/* Return color name of the given color value. */
void LCD_color_name(UINT16 ColorValue, UCHAR *ColorName);

/* Display the Framebuffer current settings. */
void LCD_display_info(void);

/* Function to be tested. */
void LCD_display_point(UINT16 PixelLine, UINT16 PixelColumn, UINT16 Color);

/* Draw a rectangle in the Framebuffer. */
void LCD_draw_box(UINT16 Xstart, UINT16 Ystart, UINT16 Xend, UINT16 Yend, UINT16 Color, DOT_PIXEL Line_width, DRAW_FILL Draw_Fill);

/* Display English characters in Framebuffer. */
void LCD_draw_char(UINT16 PixelLine, UINT16 PixelColumn, UCHAR Character, sFONT* Font, UINT16 Color_Foreground, UINT16 Color_Background);

/* Use the 8-point method to draw a circle of the specified size and at the specified position in the framebuffer. */
void LCD_draw_circle(UINT16 X_Center, UINT16 Y_Center, UINT16 Radius, UINT16 Color, DOT_PIXEL Line_width, DRAW_FILL Draw_Fill);

/* Draw an image in Framebuffer. */
void LCD_draw_image(UINT16 LineStart, UINT16 ColumnStart, const UINT16 *Image, UINT16 ImageHeight, UINT16 ImageWidth);  // const unsigned char *image (before)

/* Draw a line with arbitrary slope in the Framebuffer. */
void LCD_draw_line(UINT16 Xstart, UINT16 Ystart, UINT16 Xend, UINT16 Yend, UINT16 Color, DOT_PIXEL Line_width, LINE_STYLE Line_Style);

/* Draw a point of the selected color in the Framebuffer. */
void LCD_draw_point(UINT16 PixelLineNumber, UINT16 PixelColumnNumber, UINT16 Color, DOT_PIXEL Dot_Pixel, DOT_STYLE Dot_Style);

/* Module exits, closes SPI and BCM2835 library. */
void LCD_exit(void);

/* Force a hardware reset on the LCD display reset pin (active low). */
void LCD_hard_reset(void);

/* Initialize LCD display. */
void LCD_init(UINT16 Rotation, UINT16 Color);

/* Initialize Framebuffer. */
void LCD_init_framebuffer(UINT16 Rotation, UINT16 Color);

/* Initialize GPIO's used for LCD module SPI communication. */
void LCD_init_gpio(void);

/* Initialize GPIO's used for LCD module SPI communication and initialize PWM for backlight display. */
void LCD_init_LCDcomm(void);

/* Initialize LCD display registers. */
void LCD_init_register(void);

/* Convert current content of framebuffer according to specified mirroring orientation. */
void LCD_mirror_framebuffer(UINT16 LineStart, UINT16 ColumnStart, UINT16 LineEnd, UINT16 ColumnEnd, UINT8 MirroringType);

/* Partially clear Framebuffer (RAM memory) while setting color as specified. */
void LCD_part_clear(UINT16 StartLine, UINT16 StartColumn, UINT16 EndLine, UINT16 EndColumn, UINT16 Color);

/* Partially clear LCD display while setting the specified background color. */
void LCD_part_cls(UINT16 StartLine, UINT16 StartColumn, UINT16 EndLine, UINT16 EndColumn, UINT16 Color);

/* Refresh part of LCD display (faster than a "plain" LCD_refresh()). */
void LCD_part_refresh(UINT16 StartLine, UINT16 StartColumn, UINT16 EndLine, UINT16 EndColumn);

/* "printf()" emulation function. */
void LCD_printf(UINT16 PixelLine, UINT16 PixelColumn, UINT16 ForegroundColor, UINT16 BackgroundColor, sFONT *Font, UCHAR *Format, ...);

/* Read data from LCD display. */
void LCD_read(UINT8 *Parameter, UINT8 Length);

/* Refresh LCD display with Framebuffer content. */
void LCD_refresh(void);

/* Scroll part of LCD display. */
void LCD_scroll(UINT8 Direction, UINT16 StartLine, UINT16 StartColumn, UINT16 EndLine, UINT16 EndColumn, UINT16 ScrollNumber, UINT16 Color);

/* Send a command to LCD display through SPI protocol. */
void LCD_send_command(UINT8 Command);

/* Send 8 bits of data to LCD data port through SPI protocol. */
void LCD_send_data_8bit(UINT8 Data);

/* Send 16 bits of data to LCD data port through SPI protocol. */
void LCD_send_data_16bit(UINT16 Data);

/* Set the LCD display brightness. */
void LCD_set_brightness(UINT8 Value);

/* Set pixels in Framebuffer. */
void LCD_set_pixel(UINT16 PixelLineNumber, UINT16 PixelColumnNumber, UINT16 Color);

/* Rotate image by specified number of degrees (0, 90, 180, 270). */
void LCD_set_rotation(UINT16 Rotation);

/* Set the start position and size of the display area. */
void LCD_set_window(UINT16 XStart, UINT16 YStart, UINT16 XEnd, UINT16 YEnd);

/* Perform a software reset on LCD display. */
void LCD_soft_reset(void);

/* Transfer an image file directly to LCD display without using framebuffer. */
void LCD_splash_image(UINT16 PixelLineStart, UINT16 PixelColumnStart, UINT16 PixelLineEnd, UINT16 PixelColumnEnd, UINT16 *Image);

/* Perform tests to speed-up real-time clock refresh on LCD sreen. */
void LCD_test(UINT8 DoW, UINT8 Day, UINT8 Month, UINT16 Year, UINT8 Hour, UINT8 Minute, UINT8 Second);

/* Translate specified coordinates in current rotation mode to equivalent coordinates in native mode (rotation 0-degree). */
void LCD_translate_coord(UINT16 *Line, UINT16 *Column);

/* Send a string to VT101 monitor through Pico UART. */
extern void log_printf(UINT LineNumber, const UCHAR *FunctionName, UCHAR *Format, ...);

/* Input a string from stdio. */
extern void input_string(UCHAR *String, UINT16 StringSize, UINT32 TimeOutMSec);

/* Reverse byte order in a given word. */
UINT16 util_reverse_word(UINT16 Word);
#endif  // _LCD_MODULE_