/* ============================================================================================================================================================= *\
   Pico-LCD-Module.c
   St-Louys Andre - June 2023
   astlouys@gmail.com
   https://github.com/astlouys/Pico-LCD-Module
   Revision 17-JUN-2026
   Langage: Linux gcc
   Version 3.02

   ========================================================================
   Pico-LCD-Module is compatible with the ASTL Smart Home ecosystem family.
   ========================================================================

   REVISION HISTORY:
   =================
   20-JAN-2021 1.00 - Original source code from Waveshare.
   09-JUN-2023 2.00 - Initial release modified by Andre St-Louys.
   13-FEB-2025 2.01 - Fix inconsistencies on LCD coordinates (sometimes (0,0) to (319,239), sometimes (1,1) to (320, 240)).
                      Fine tune, optimizations and add error messages.
   28-FEB-2025 3.00 - Add support for Pimoroni's Pico-Explorer.
   12-JUN-2025 3.01 - Modify / adjust code to make it compliant to other Pico-Xxx-Module library members.
                      Rename function log_info() to log_printf().
                      Use common functions get_pico_identifier(), input_string(), log_printf().
   27-MAY-2026 3.01 - Major update, debug and optimizing session to comply with last function standards.
\* ================================================================================================================================================================= */

/* ============================================================================================================================================================= *\
                                    C-Language module to integrate a Waveshare's Pico color LCD display to an existing program.

     Most operations are performed on Framebuffer (in RAM memory) and then, the content of this Framebuffer is transferred to the LCD display through SPI protocol.

                                                           Copyright notice from Waveshare's original V1.00:

       Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal
         in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
                     copies of the Software, and to permit persons to  whom the Software is furnished to do so, subject to the following conditions:

                      The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.

         THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
          FITNESS OR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
                        LIABILITY WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE
                                                                 OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
\* ============================================================================================================================================================= */



/* $PAGE */
/* $TITLE=Include files. */
/* ============================================================================================================================================================= *\
                                            Generic list of GPIOs usage and pin-out for most ASTL Smart Home ecosystem devices.
                                               (To be used as a guideline only. May sometimes be different for some devices)
\* ============================================================================================================================================================= */
/* ------------------------------------------------------------------------------------------------------------------------------------------------------------- *\
Pin 01 - GPIO  0  - Reserved for UART0 TX (Pico-ASTL-CallerId modem).
Pin 02 - GPIO  1  - Reserved for UART0 RX (Pico-ASTL-CallerId modem).
Pin 03 - GND      - Ground
Pin 04 - GPIO  2  - Button 2 (on Waveshare's 2-inch LCD display).
Pin 05 - GPIO  3  - Button 3 (on Waveshare's 2-inch LCD display).
Pin 06 - GPIO  4  - Reserved for UART1 TX (Pico-ASTL-SoundServer and Pico-ASTL-SPrinter).
Pin 07 - GPIO  5  - Reserved for UART1 RX (Pico-ASTL-SoundServer and Pico-ASTL-SPrinter).
Pin 08 - GND      - Ground
Pin 09 - GPIO  6  - Reserved for I2C1 SDA (data)  for BME280, LTR559 and other I2C devices.
Pin 10 - GPIO  7  - Reserved for I2C1 SCL (clock) for BME280, LTR559 and other I2C devices.
Pin 11 - GPIO  8  - LCD_DATA_COMMAND - data / command control pin  (on Waveshare's 2-inch LCD display)     Pimoroni Explorer Base: Motor1-.
Pin 12 - GPIO  9  - LCD_CHIP_SELECT  - chip select pin, active low (on Waveshare's 2-inch LCD display)     Pimoroni Explorer Base: Motor1+.
Pin 13 - GND      - Ground
Pin 14 - GPIO 10  - LCD_CLOCK        - SPI clock pin               (on Waveshare's 2-inch LCD display)     Pimoroni Explorer Base: Motor2-.
Pin 15 - GPIO 11  - LCD_DATA         - SPI  data pin               (on Waveshare's 2-inch LCD display)     Pimoroni Explorer Base: Motor2+.
Pin 16 - GPIO 12  - LCD_RESET        - reset pin, active low       (on Waveshare's 2-inch LCD display)     Pimoroni Explorer Base: ButtonA.
Pin 17 - GPIO 13  - LCD_BRIGHTNESS   - backlight pwm pin           (on Waveshare's 2-inch LCD display)     Pimoroni Explorer Base: ButtonB.
Pin 18 - GND      - Ground
Pin 19 - GPIO 14  - Reserved for Pico-ASTL-SoundServer status pin                                          Pimoroni Explorer Base: ButtonX.
Pin 20 - GPIO 15  - Button 0 (on Waveshare's 2-inch LCD display)                                           Pimoroni Explorer Base: ButtonY.
Pin 21 - GPIO 16  - Reserved for infrared sensor / receiver (VS1838)                                       Pimoroni Explorer Base: SPI MISO.
Pin 22 - GPIO 17  - Button 1 (on Waveshare's 2-inch LCD display)                                           Pimoroni Explorer Base: LCD Chip Select.
Pin 23 - GND      - Ground
Pin 24 - GPIO 18  - Reserved for indicator LED                                                             Pimoroni Explorer Base: SPI SCK.
Pin 25 - GPIO 19  - Reserved for tri-color LED (RED)                                                       Pimoroni Explorer Base: SPI MOSI.
Pin 26 - GPIO 20  - Reserved for tri-color LED (GREEN)                                                     Pimoroni Explorer Base: I2C SDA (data).
Pin 27 - GPIO 21  - Reserved for tri-color LED (BLUE)                                                      Pimoroni Explorer Base: I2C SCL (clock).
Pin 28 - GND      - Ground
Pin 29 - GPIO 22  - Reserved for Active or Passive buzzer                                                  Pimoroni Explorer Base: I2C INT (interrupt).
Pin 30 - RUN      - Enable pin for RP2040. May be used to reset the Pico.
Pin 31 - GPIO 26  - Reserved for analog device (ADC0).
Pin 32 - GPIO 27  - Reserved for analog device (ADC1).
Pin 33 - GND      - ADC ground
Pin 34 - GPIO 28  - Reserved for analog device (ADC2).
Pin 35 - ADC Vref - ADC voltage reference.
Pin 36 - 3V3 out  - Regulated 3V3 to feed 3V3 devices (max 300mA).
Pin 37 - 3V3 en   - Connected to the SMPS enable pin and pulled high by a 100K resistor. Put it to Low to turn Off 3V3 to the RP2040.
Pin 38 - GND      - Ground
Pin 39 - VSYS 5V  - 5V out if powered by USB or main system input voltage (1.8V to 5.5V) if powered externally. SMPS will generate 3V3 for the RP2040 and GPIOs.
Pin 40 - VBUS 5V  - 5V input voltage coming from Pico's USB connector.

GPIO 23 - Low for high efficiency (PFM) or High for improved ripple (PWM).
GPIO 24 - VBUS sense. Detect USB power or VBus pin (High if VBUS is present, Low otherwise).
GPIO 25 - Pico's LED (output)
GPIO 29 - ADC3 - VSYS sense. Read VSYS / 3 through resistor divider and Q1.
GPIO 30 - ADC4 - Read Pico's internal temperature sensor.

Test points under Pico PC board:
Test point 1 - Ground.
Test point 2 - USB DM  - USB data negative pin.
Test point 3 - USB DP  - USB data positive pin.
Test point 4 - GPIO 23 - Switched Mode Power Supply (SMPS) Pin. Controls the on-board SMPS power save pin.
Test point 5 - GPIO 25 - Pico's LED.
Test point 6 - Bootsel (upload mode).
\* ------------------------------------------------------------------------------------------------------------------------------------------------------------- */



/* $PAGE */
/* $TITLE=Included files. */
/* ============================================================================================================================================================= *\
                                                                          Include files.
\* ============================================================================================================================================================= */
#include "baseline.h"
#include "pico/stdlib.h"
#include "stdio.h"
#include "stdlib.h"
#include "string.h"

#include "Pico-LCD-Module.h"

#include "font8.c"
#include "font12.c"
#include "font16.c"
#include "font20.c"
#include "font24.c"





/* $PAGE */
/* $TITLE=Definitions. */
/* ============================================================================================================================================================= *\
                                                                             Definitions.
\* ============================================================================================================================================================= */
// #define RELEASE_VERSION  ///



/* $PAGE */
/* $TITLE=Global or static variables. */
/* ============================================================================================================================================================= *\
                                                                        Global or static variables.
\* ============================================================================================================================================================= */
UCHAR LCD_Separator[201];
UCHAR LCD_Separator1[201];

/* Complete day names. */
extern UCHAR DayName[7][13];

/* Short - 3-letters - day names. */
extern UCHAR ShortDay[7][4];

/* Complete month names. */
extern UCHAR MonthName[13][13];

/* Short - 3-letters - month names. */
extern UCHAR ShortMonth[13][4];

static UINT8 SliceNumber;

sFONT Font24;
sFONT Font20;
sFONT Font16;
sFONT Font12;
sFONT Font8;

extern struct struct_lcd StructLCD;

extern void util_display_data(UCHAR *Data, UINT32 DataSize);



/* $PAGE */
/* $TITLE=LCD_clear() */
/* ============================================================================================================================================================= *\
                                                   Clear Framebuffer (RAM memory) while setting color as specified.
                            NOTE: Function <LCD_refresh() must then be called to transfer the content of the framebuffer to the LCD display.
\* ============================================================================================================================================================= */
void LCD_clear(UINT16 Color)
{
#ifdef RELEASE_VERSION
  UINT8 FlagLocalDebug = FLAG_OFF;  // must be turned OFF at all time.
#else   // RELEASE_VERSION
  UINT8 FlagLocalDebug = FLAG_OFF;  // may be turned ON for debug purposes.
#endif  // RELEASE_VERSION

  UCHAR  String[32];

  UINT32 FrameBufferSize;
  UINT32 MemOffset;


  /* Calculate number of pixels in FrameBuffer (size depends of StructLCD.Depth). */
  FrameBufferSize = StructLCD.PhysicalHeight * StructLCD.PhysicalWidth;

  if (FlagLocalDebug)
  {
    LCD_color_name(Color, String);
    log_printf(__LINE__, __func__, "Entering LCD_clear(Color: 0x%4.4X - %s)\n", Color, String);
    log_printf(__LINE__, __func__, "Rotation currently set to %u degrees.\n", StructLCD.Rotation);
    log_printf(__LINE__, __func__, "StructLCD.PhysicalHeight:  %3u   StructLCD.PhysicalWidth: %3u   StructLCD.Depth: %u\n",  StructLCD.PhysicalHeight, StructLCD.PhysicalWidth, StructLCD.Depth);
    log_printf(__LINE__, __func__, "StructLCD.Height:          %3u   StructLCD.Width:         %3u\n",                         StructLCD.Height,         StructLCD.Width);
    log_printf(__LINE__, __func__, "Size of Framebuffer: %u X %u = %lu pixels (hex: 0x%8.8lX) or %lu bytes (hex: 0x%lX)\n",
               StructLCD.PhysicalHeight, StructLCD.PhysicalWidth, StructLCD.PhysicalHeight * StructLCD.PhysicalWidth, StructLCD.PhysicalHeight * StructLCD.PhysicalWidth,
               StructLCD.PhysicalHeight * StructLCD.PhysicalWidth * StructLCD.Depth, StructLCD.PhysicalHeight * StructLCD.PhysicalWidth * StructLCD.Depth);
  }

  for (MemOffset = 0l; MemOffset < FrameBufferSize; ++MemOffset)
    StructLCD.FrameBuffer[MemOffset] = Color;

  if (FlagLocalDebug) log_printf(__LINE__, __func__, "Exiting LCD_clear()\n\n");

  return;
}





/* $PAGE */
/* $TITLE=LCD_cls() */
/* ============================================================================================================================================================= *\
                                                    Clear LCD display while setting the specified background color.
              NOTE: The framebuffer remains unchanged and still contain its data. A call to LCD_refresh() would restore the current framebuffer content.
\* ============================================================================================================================================================= */
void LCD_cls(UINT16 Color)
{
#ifdef RELEASE_VERSION
  UINT8 FlagLocalDebug = FLAG_OFF;  // must be turned OFF at all time.
#else   // RELEASE_VERSION
  UINT8 FlagLocalDebug = FLAG_OFF;  // may be turned ON for debug purposes.
#endif  // RELEASE_VERSION

  UCHAR String[32];

  UINT32 FrameBufferSize;
  UINT32 MemOffset;
    
  
  FrameBufferSize = StructLCD.Height * StructLCD.Width;

  if (FlagLocalDebug)
  {
    LCD_color_name(Color, String);
    log_printf(__LINE__, __func__, "Entering LCD_cls(Color: 0x%4.4X - %s)\n", Color, String);
    log_printf(__LINE__, __func__, "Rotation currently set to %u degrees.\n", StructLCD.Rotation);
    log_printf(__LINE__, __func__, "StructLCD.PhysicalHeight:  %3u   StructLCD.PhysicalWidth:  %3u\n", StructLCD.PhysicalHeight, StructLCD.PhysicalWidth);
    log_printf(__LINE__, __func__, "StructLCD.Height:          %3u   StructLCD.Width:          %3u\n", StructLCD.Height,  StructLCD.Width);
    log_printf(__LINE__, __func__, "Size of Framebuffer: %u X %u = %lu pixels (hex: 0x%8.8lX) or %lu bytes   Depth: %u\n",   StructLCD.PhysicalHeight, StructLCD.PhysicalWidth, FrameBufferSize, FrameBufferSize * StructLCD.Depth, FrameBufferSize, StructLCD.Depth);
  }

  LCD_set_window(0, 0, StructLCD.Height - 1, StructLCD.Width - 1);

  gpio_put(StructLCD.GPIODataCommand, 1);  // set command mode.
  gpio_put(StructLCD.GPIOChipSelect,  0);  // enable chip select active low.
  
  if (FlagLocalDebug) log_printf(__LINE__, __func__, "Before spi_write_blocking()\n");

  for (MemOffset = 0l; MemOffset < FrameBufferSize; ++MemOffset)
    spi_write_blocking(StructLCD.SPIPort, (UINT8 *)&Color, 2);

  gpio_put(StructLCD.GPIOChipSelect, 1);  // disable chip select when done.

  if (FlagLocalDebug) log_printf(__LINE__, __func__, "Exiting  LCD_cls()\n\n");

  return;
}





/* $PAGE */
/* $TITLE=LCD_color_name() */
/* ============================================================================================================================================================= *\
                                                              Return color name of the given color value.
                                   NOTE: Only the main color names are covered. Color code will be returned for an unknown color value.
\* ============================================================================================================================================================= */
void LCD_color_name(UINT16 ColorValue, UCHAR *ColorName)
{
  switch (ColorValue)
  {
    case (WHITE):
      strcpy(ColorName, "white");
    break;

    case (BLACK):
      strcpy(ColorName, "black");
    break;

    case (RED):
      strcpy(ColorName, "red");
    break;

    case (GREEN):
      strcpy(ColorName, "green");
    break;

    case (BLUE):
      strcpy(ColorName, "blue");
    break;

    case (MAGENTA):
      strcpy(ColorName, "magenta");
    break;

    case (CYAN):
      strcpy(ColorName, "cyan");
    break;

    case (YELLOW):
      strcpy(ColorName, "yellow");
    break;

    case (BROWN):
      strcpy(ColorName, "brown");
    break;

    case (BROWN_RED):
      strcpy(ColorName, "brown-red");
    break;

    case (GREY):
      strcpy(ColorName, "grey");
    break;

    case (DARK_BLUE):
      strcpy(ColorName, "dark-blue");
    break;

    case (DARK_GREEN):
      strcpy(ColorName, "dark-green");
    break;

    case (GREEN_YELLOW):
      strcpy(ColorName, "green-yellow");
    break;

    case (TURQUOISE):
      strcpy(ColorName, "turquoise");
    break;

    case (JEANS_BLUE):
      strcpy(ColorName, "jeans-blue");
    break;

    case (LIGHT_GREEN):
      strcpy(ColorName, "light-green");
    break;

    default:
      sprintf(ColorName, "undefined color code (0x%4.4X)", ColorValue);
    break;
  }

  return;
}





/* $PAGE */
/* $TITLE=LCD_display_info() */
/* ============================================================================================================================================================= *\
                                                   Show the LCD display current settings. Also display main color values.
\* ============================================================================================================================================================= */
void LCD_display_info(void)
{
  UINT32 FrameBufferSize = StructLCD.PhysicalHeight * StructLCD.PhysicalWidth;


  log_printf(__LINE__, __func__, LCD_Separator);
  log_printf(__LINE__, __func__, "<120>Current LCD display settings / parameters:\n");

  switch (StructLCD.DeviceType)
  {
    case (LCD_114):
    {
      log_printf(__LINE__, __func__, "<120>LCD initialized for Waveshare's 1.14-inch device.\n");
      log_printf(__LINE__, __func__, "<120>Native mode is portrait mode, with USB connector at the bottom of the display.\n");
    }
    break;

    case (LCD_200):
    {
      log_printf(__LINE__, __func__, "<120>LCD initialized for Waveshare's 2-inch device.\n");
      log_printf(__LINE__, __func__, "<120>Native mode is portrait mode, with USB connector at the bottom of the display.\n");
    }
    break;

    case (LCD_EXPLORER):
    {
      log_printf(__LINE__, __func__, "<120>LCD initialized for Pimoroni's Pico Explorer device.\n");
      log_printf(__LINE__, __func__, "<120>Native mode is Pico located at top left so that user can read text on the Explorer.\n");
    }
    break;
  }

  log_printf(__LINE__, __func__, LCD_Separator);

  log_printf(__LINE__, __func__, "LCD display physical height (in native mode): %5u pixels.\n", StructLCD.PhysicalHeight);
  log_printf(__LINE__, __func__, "LCD display physical width  (in native mode): %5u pixels.\n", StructLCD.PhysicalWidth);

  log_printf(__LINE__, __func__, "Size of virtual framebuffer:              %u X %u = %5u pixels of %u bytes each (total: %u bytes)\n", StructLCD.PhysicalHeight, StructLCD.PhysicalWidth, FrameBufferSize, StructLCD.Depth, FrameBufferSize * StructLCD.Depth);
  log_printf(__LINE__, __func__, "Framebuffer address in RAM:              0x%8.8p (Pico RAM space begins at 0x20000000)\n",    &StructLCD.FrameBuffer[0]);
  log_printf(__LINE__, __func__, "End address of Framebuffer in RAM:       0x%8.8p (Pico RAM space ends   at 0x20040000)\n",    &StructLCD.FrameBuffer[320 * 240 - 1]);

  log_printf(__LINE__, __func__, LCD_Separator);
  log_printf(__LINE__, __func__, "LCD display current height:                   %5u\n",  StructLCD.Height);
  log_printf(__LINE__, __func__, "LCD display current  width:                   %5u\n",  StructLCD.Width);

  log_printf(__LINE__, __func__, "LCD display line    offset:                   %5u\n",  StructLCD.LineOffset);
  log_printf(__LINE__, __func__, "LCD display column  offset:                   %5u\n",  StructLCD.ColumnOffset);

  log_printf(__LINE__, __func__, LCD_Separator);
  log_printf(__LINE__, __func__, "LCD display current rotation setting:          %5u-degree\n", StructLCD.Rotation);
  log_printf(__LINE__, __func__, "LCD display current mirror setting:            %10s\n", StructLCD.Mirroring[StructLCD.MirrorMode]);
  log_printf(__LINE__, __func__, "LCD display depth:                            %5u\n",  StructLCD.Depth);
  log_printf(__LINE__, __func__, LCD_Separator);

  return;
}





/* $PAGE */
/* $TITLE=LCD_display_point() */
/* ============================================================================================================================================================= *\
                                                              Turn On a point with specified color on LCD display.
   NOTE: LCD_display_point() is similar to LCD_cls() in that it works directly on LCD display. The FrameBuffer remains unchanged and keeps its original content.
         This means an LCD_refresh() is not required with LCD_display_point(). This also means that an eventual LCD_refresh() will restore the original 
         FrameBuffer content, defeating the action of LCD_display_point().
\* ============================================================================================================================================================= */
void LCD_display_point(UINT16 Line, UINT16 Column, UINT16 Color)
{
#ifdef RELEASE_VERSION
  UINT8 FlagLocalDebug = FLAG_OFF;  // must be turned OFF at all time.
#else   // RELEASE_VERSION
  UINT8 FlagLocalDebug = FLAG_OFF;   // may be turned ON for debug purposes.
#endif  // RELEASE_VERSION


  if (FlagLocalDebug)
  {
    log_printf(__LINE__, __func__, "Entering LCD_display_point(%u,%u, 0x%4.4X)\n", Line, Column, Color);
    log_printf(__LINE__, __func__, "Height: %3u   Width: %3u   Physical height: %3u   Physical width: %3u\n", StructLCD.Height, StructLCD.Width, StructLCD.PhysicalHeight, StructLCD.PhysicalWidth);
    log_printf(__LINE__, __func__, "Current rotation setting: %u\n", StructLCD.Rotation);
  }

  /* Validate Pixel line coordinate specified. */
  if (Line > (StructLCD.Height - 1))
  {
    log_printf(__LINE__, __func__, "Line coordinate exceeds display boundaries (Line: %u VS %u).\n", Line, (StructLCD.Height - 1));
    return;
  }

  /* Validate PixelColumn coordinate specified. */
  if (Column > (StructLCD.Width - 1))
  {
    log_printf(__LINE__, __func__, "Column coordinate exceeds display boundaries (Column: %u VS %u).\n", Column, (StructLCD.Width - 1));
    return;
  }

  /* Define LCD display writable area. */
  LCD_set_window(Line, Column, Line + 1, Column + 1);
  LCD_send_data_16bit(Color);

  if (FlagLocalDebug) log_printf(__LINE__, __func__, "Exiting  LCD_display_point()\n\n");

  return;
}





/* $PAGE */
/* $TITLE=LCD_draw_box() */
/* ============================================================================================================================================================= *\
                                                                            Draw a box.
                                                               LineStart   ：Box starting pixel line.
                                                               ColumnStart ：Box starting pixel column.
                                                               LineEnd     ：Box ending pixel line.
                                                               ColumnEnd   ：Box ending pixel column.
                                                               Color       ：Color of the box border.
                                                               LineWidth   : Line thickness of the box border.
                                                               DrawFill    : Whether to fill the inside of the box or not with the specified color.
\* ============================================================================================================================================================= */
void LCD_draw_box(UINT16 LineStart, UINT16 ColumnStart, UINT16 LineEnd, UINT16 ColumnEnd, UINT16 Color, DOT_PIXEL LineWidth, DRAW_FILL DrawFill)
{
#ifdef RELEASE_VERSION
  UINT8 FlagLocalDebug = FLAG_OFF;  // must be turned OFF at all time.
#else   // RELEASE_VERSION
  UINT8 FlagLocalDebug = FLAG_OFF;   // may be turned ON for debug purposes.
#endif  // RELEASE_VERSION

  UINT16 TopLine;
  UINT16 BottomLine;
  UINT16 LeftColumn;
  UINT16 RightColumn;
  UINT16 Loop1UInt16;


  if (FlagLocalDebug) log_printf(__LINE__, __func__, "Entering LCD_draw_box()\n");


  /* Validate LineStart coordinate specified. Let the firmware continue so that the part of the box that is visible will be displayed.*/
  if (LineStart > (StructLCD.Height - 1))
    log_printf(__LINE__, __func__, "Line start coordinate exceeds display boundaries (Line: %u VS %u).\n", LineStart, (StructLCD.Height - 1));

  /* Validate ColumnStart coordinate specified. Let the firmware continue so that the part of the box that is visible will be displayed. */
  if (ColumnStart > (StructLCD.Width - 1))
    log_printf(__LINE__, __func__, "Column start coordinate exceeds display boundaries (Column: %u VS %u).\n", ColumnStart, (StructLCD.Width - 1));

  /* Validate LineEnd coordinate specified. Let the firmware continue so that the part of the box that is visible will be displayed. */
  if (LineEnd > (StructLCD.Height - 1))
    log_printf(__LINE__, __func__, "Line end coordinate exceeds display boundaries (Line: %u VS %u).\n", LineEnd, (StructLCD.Height - 1));

  /* Validate ColumnEnd coordinate specified. Let the firmware continue so that the part of the box that is visible will be displayed. */
  if (ColumnEnd > (StructLCD.Width - 1))
    log_printf(__LINE__, __func__, "Column end coordinate exceeds display boundaries (Column: %u VS %u).\n", ColumnEnd, StructLCD.Width);


  if (DrawFill)
  {
    /* Draw the box while painting it all with the specified filling color. */
    for (Loop1UInt16 = LineStart; Loop1UInt16 <= LineEnd; ++Loop1UInt16)
      LCD_draw_line(Loop1UInt16, ColumnStart, Loop1UInt16, ColumnEnd, Color, LineWidth, LINE_STYLE_SOLID);
  }
  else
  {
    TopLine     = LineStart;
    BottomLine  = LineEnd;
    LeftColumn  = ColumnStart;
    RightColumn = ColumnEnd;

    for (Loop1UInt16 = 0; Loop1UInt16 < LineWidth; ++Loop1UInt16)
    {
      LCD_draw_line(TopLine,    LeftColumn,  TopLine,    RightColumn, Color, DOT_PIXEL_1X1, LINE_STYLE_SOLID);  // draw top line of the box.
      LCD_draw_line(BottomLine, LeftColumn,  BottomLine, RightColumn, Color, DOT_PIXEL_1X1, LINE_STYLE_SOLID);  // draw bottom line of the box.
      LCD_draw_line(TopLine,    LeftColumn,  BottomLine, LeftColumn,  Color, DOT_PIXEL_1X1, LINE_STYLE_SOLID);  // draw left side of the box.
      LCD_draw_line(TopLine,    RightColumn, BottomLine, RightColumn, Color, DOT_PIXEL_1X1, LINE_STYLE_SOLID);  // draw right side of the box.

      /* Prepare coordinates to draw next smaller border of box thickness is more than one pixel. */
      ++TopLine;
      --BottomLine;
      ++LeftColumn;
      --RightColumn;
    }
  }

  if (FlagLocalDebug) log_printf(__LINE__, __func__, "Exiting LCD_draw_box()\n\n");

  return;
}





/* $PAGE */
/* $TITLE=LCD_draw_char() */
/* ============================================================================================================================================================= *\
                                                        *** IMPORTANT *** IMPORTANT *** IMPORTANT *** IMPORTANT ***
                                                   NOTE: Function LCD_draw_char() is called by LCD_printf().
                                                         Function LCD_printf() must be used by user instead or LCD_draw_char().

                                                         Splash specified ASCII character to Framebuffer.
 
                                                   NOTE: The PixelLineNumber and PixelColumnNumber coordinates correspond to
                                                         the upper left position of the character to be splashed (in pixels).
                                                         Be aware that the specified character set may have extra blank pixels
                                                         above / under / to the left / to the right of the character to be displayed.
                                                         Consequently, character position on LCD display may be a few pixels off
                                                         from the specified coordinates

                                                   PixelLineNumber   ：Pixel line number of the upper left position of the character.
                                                   PixelColumnNumber ：Pixel column number of the upper left position of the character.
                                                   Character         ：ASCII character to be displayed.
                                                   Font              ：A pointer to a structure containing the specifications of the character set to be used.
                                                   ForegroundColor   :Foreground color to use for character.
                                                   BackgroundColor   :Background color to use behind the character.
\* ============================================================================================================================================================= */
void LCD_draw_char(UINT16 PixelLineNumber, UINT16 PixelColumnNumber, UCHAR Character, sFONT* Font, UINT16 ForegroundColor, UINT16 BackgroundColor)
{
#ifdef RELEASE_VERSION
  UINT8 FlagLocalDebug = FLAG_OFF;  // must be turned OFF at all time.
#else   // RELEASE_VERSION
  UINT8 FlagLocalDebug = FLAG_OFF;  // may be turned ON for debug purposes.
#endif  // RELEASE_VERSION

  UCHAR *CharPointer;
  
  UINT16 Column;
  UINT16 Line;

  UINT32 CharOffset;


  if (FlagLocalDebug)
  {
    log_printf(__LINE__, __func__, "Entering LCD_draw_char(Line: %u   Column: %u   <%c>)\n", PixelLineNumber, PixelColumnNumber, Character);
    log_printf(__LINE__, __func__, "Font height: %u     Font width: %u\n", Font->Height, Font->Width);
  }

  /* Validate PixelLineNumber specified as arguments. Let the firmware continue so that the part of the character that is visible will be displayed.*/
  if ((PixelLineNumber + Font->Height) > (StructLCD.Height - 1))
    if (FlagLocalDebug)
     log_printf(__LINE__, __func__, "WARNING - Line coordinates exceed display boundaries (%u VS %u and font height: %u).\n", PixelLineNumber, (StructLCD.Height - 1), Font->Height);

  /* Validate PixelColumnNumber specified as arguments. Let the firmware continue so that the part of the character that is visible will be displayed.*/
  if ((PixelColumnNumber + Font->Width) > (StructLCD.Width - 1))
    if (FlagLocalDebug)
      log_printf(__LINE__, __func__, "WARNING - Column coordinates exceed display boundaries (%u VS %u and font width: %u).\n", PixelColumnNumber, (StructLCD.Width - 1), Font->Width);

  
  
  /* Determine character offset in the specific character set table. */
  CharOffset = (Character - ' ') * Font->Height * (Font->Width / 8 + (Font->Width % 8 ? 1 : 0));
  CharPointer = &Font->Table[CharOffset];
  if (FlagLocalDebug) log_printf(__LINE__, __func__, "Displaying character <%c> (%u) on Line %u   and Column %u\n", Character, Character, PixelLineNumber, PixelColumnNumber);


  /* Build all pixel lines of the specified character in framebuffer. */
  for (Line = 0; Line < Font->Height; ++Line)
  {
    /* Display all pixel columns of the specified character in framebuffer. */
    for (Column = 0; Column < Font->Width; ++Column)
    {
      if (*CharPointer & (0x80 >> (Column % 8)))
      {
        /* Set foreground pixel. */
        LCD_set_pixel(PixelLineNumber + Line, PixelColumnNumber + Column, ForegroundColor);
        if (FlagLocalDebug) log_printf(__LINE__, __func__, "Set pixel with foreground <X> at %3u, %3u\n", PixelLineNumber + Line, PixelColumnNumber + Column);
      }
      else
      {
        /* Set background pixel. */
        LCD_set_pixel(PixelLineNumber + Line, PixelColumnNumber + Column, BackgroundColor);
        if (FlagLocalDebug) log_printf(__LINE__, __func__, "Set pixel with background < > at %3u, %3u\n", PixelLineNumber + Line, PixelColumnNumber + Column);
      }
      
      /* One pixel corresponds to 8 bits. */
      if (Column % 8 == 7) ++CharPointer;
    }
    
    if (Font->Width % 8 != 0) ++CharPointer;
  }

  if (FlagLocalDebug) log_printf(__LINE__, __func__, "Exiting  LCD_draw_char()\n\n");

  return;
}





/* $PAGE */
/* $TITLE=LCD_draw_circle() */
/* ============================================================================================================================================================= *\
                                          Use the 8-point method to draw a circle of the specified size and at the specified position.
                                                         LineCenter   ：Pixel line of circle center.
                                                         ColumnCenter ：Pixel column of circle center.
                                                         Radius       ：Circle radius.
                                                         Color        ：Color of the circle to be drawn.
                                                         LineWidth     : Line width of the circle to be drawn.
                                                         DrawFill      : Whether to fill the inside of the circle or not.
\* ============================================================================================================================================================= */
void LCD_draw_circle(UINT16 LineCenter, UINT16 ColumnCenter, UINT16 Radius, UINT16 Color, DOT_PIXEL LineWidth, DRAW_FILL DrawFill)
{
#ifdef RELEASE_VERSION
  UINT8 FlagLocalDebug = FLAG_OFF;  // must be turned OFF at all time.
#else   // RELEASE_VERSION
  UINT8 FlagLocalDebug = FLAG_OFF;  // may be turned ON for debug purposes.
#endif  // RELEASE_VERSION

  INT16 Esp;
  INT16 sCountY;
  INT16 XCurrent;
  INT16 YCurrent;


  if (FlagLocalDebug) log_printf(__LINE__, __func__, "Entering LCD_draw_circle()\n");

  /* Validate LineCenter coordinate and radius specified. 
     If there is an override, let the Firmware continue so that the part of the circle that is visible be displayed.*/
  if (((LineCenter + Radius) > (StructLCD.Height - 1)) || (Radius > (LineCenter - 1)))
    log_printf(__LINE__, __func__, "WARNING - Circle line coordinates exceed display height (%u VS %u and radius: %u).\n", LineCenter, StructLCD.Height, Radius);

  /* Validate ColumnCenter coordinate and radius specified. 
     If there is an override, let the Firmware continue so that the part of the circle that is visible be displayed.*/
  if (((ColumnCenter + Radius) > (StructLCD.Width - 1)) || (Radius > (ColumnCenter - 1)))
    log_printf(__LINE__, __func__, "WARNING - Circle column coordinates exceed display width (%u VS %u and radius: %u).\n", ColumnCenter, StructLCD.Width, Radius);


  /* Draw a circle from (0, Radius) as a starting point. */
  XCurrent = 0;
  YCurrent = Radius;

  /* Cumulative error, evaluate next point of the circle. */
  Esp = 3 - (Radius << 1);

  if (DrawFill == DRAW_FILL_FULL)
  {
    /* Realistic circle. */
    while (XCurrent <= YCurrent)
    {
      for (sCountY = XCurrent; sCountY <= YCurrent; ++sCountY)
      {
        if (FlagLocalDebug) log_printf(__LINE__, __func__, "LineCenter: %3u   ColumnCenter: %3u   XCurrent: %3u   YCurrent: %3u   sCountY: %3u\n", LineCenter, ColumnCenter, XCurrent, YCurrent, sCountY);

        if (FlagLocalDebug) log_printf(__LINE__, __func__, "LCD_draw_point(%3u, %3u)\n", LineCenter + sCountY,  ColumnCenter + XCurrent);
        LCD_draw_point(LineCenter + sCountY,  ColumnCenter + XCurrent, Color, DOT_PIXEL_DFT, DOT_STYLE_DFT);  // 0

        if (XCurrent <= ColumnCenter)
        {
          if (FlagLocalDebug) log_printf(__LINE__, __func__, "LCD_draw_point(%3u, %3u)\n", LineCenter + sCountY,  ColumnCenter - XCurrent);
          LCD_draw_point(LineCenter + sCountY,  ColumnCenter - XCurrent, Color, DOT_PIXEL_DFT, DOT_STYLE_DFT);  // 7
        }

        if (FlagLocalDebug) log_printf(__LINE__, __func__, "LCD_draw_point(%3u, %3u)\n", LineCenter + XCurrent, ColumnCenter + sCountY);
        LCD_draw_point(LineCenter + XCurrent, ColumnCenter + sCountY,  Color, DOT_PIXEL_DFT, DOT_STYLE_DFT);  // 1
        
        if (sCountY <= ColumnCenter)
        {
          if (FlagLocalDebug) log_printf(__LINE__, __func__, "LCD_draw_point(%3u, %3u)\n", LineCenter + XCurrent, ColumnCenter - sCountY);
          LCD_draw_point(LineCenter + XCurrent, ColumnCenter - sCountY,  Color, DOT_PIXEL_DFT, DOT_STYLE_DFT);  // 6
        }

        if (XCurrent <= LineCenter)
        {
          if (FlagLocalDebug) log_printf(__LINE__, __func__, "LCD_draw_point(%3u, %3u)\n", LineCenter - XCurrent, ColumnCenter + sCountY);
          LCD_draw_point(LineCenter - XCurrent, ColumnCenter + sCountY,  Color, DOT_PIXEL_DFT, DOT_STYLE_DFT);  // 2
          
          if (sCountY <= ColumnCenter)
          {
            if (FlagLocalDebug) log_printf(__LINE__, __func__, "LCD_draw_point(%3u, %3u)\n", LineCenter - XCurrent, ColumnCenter - sCountY);
            LCD_draw_point(LineCenter - XCurrent, ColumnCenter - sCountY,  Color, DOT_PIXEL_DFT, DOT_STYLE_DFT);  // 5
          }
        }

        if (sCountY  <= LineCenter)
        {
          if (FlagLocalDebug) log_printf(__LINE__, __func__, "LCD_draw_point(%3u, %3u)\n", LineCenter - sCountY,  ColumnCenter + XCurrent);
          LCD_draw_point(LineCenter - sCountY,  ColumnCenter + XCurrent, Color, DOT_PIXEL_DFT, DOT_STYLE_DFT);  // 3

          if (XCurrent <= ColumnCenter)
          {
            if (FlagLocalDebug) log_printf(__LINE__, __func__, "LCD_draw_point(%3u, %3u)\n", LineCenter - sCountY,  ColumnCenter - XCurrent);
            LCD_draw_point(LineCenter - sCountY,  ColumnCenter - XCurrent, Color, DOT_PIXEL_DFT, DOT_STYLE_DFT);  // 4
          }
        }
      }
      
      if (Esp < 0)
        Esp += 4 * XCurrent + 6;
      else
      {
        Esp += 10 + 4 * (XCurrent - YCurrent);
        --YCurrent;
      }
      ++XCurrent;
    }
  }
  else
  {
    /* Hollow circle. */
    while (XCurrent <= YCurrent)
    {
      LCD_draw_point(LineCenter + YCurrent, ColumnCenter + XCurrent, Color, LineWidth, DOT_STYLE_DFT);                                  // 0
      if (XCurrent <= ColumnCenter) LCD_draw_point(LineCenter + YCurrent, ColumnCenter - XCurrent, Color, LineWidth, DOT_STYLE_DFT);    // 7


      LCD_draw_point(LineCenter + XCurrent, ColumnCenter + YCurrent, Color, LineWidth, DOT_STYLE_DFT);                                  // 1
      if (YCurrent <= ColumnCenter) LCD_draw_point(LineCenter + XCurrent, ColumnCenter - YCurrent, Color, LineWidth, DOT_STYLE_DFT);    // 6


      if (XCurrent <= LineCenter)
      {
        LCD_draw_point(LineCenter - XCurrent, ColumnCenter + YCurrent, Color, LineWidth, DOT_STYLE_DFT);                                // 2
        if (YCurrent <= ColumnCenter) LCD_draw_point(LineCenter - XCurrent, ColumnCenter - YCurrent, Color, LineWidth, DOT_STYLE_DFT);  // 5
      }


      if (YCurrent <= LineCenter)
      {
        LCD_draw_point(LineCenter - YCurrent, ColumnCenter + XCurrent, Color, LineWidth, DOT_STYLE_DFT);                                // 3
        if (XCurrent <= ColumnCenter) LCD_draw_point(LineCenter - YCurrent, ColumnCenter - XCurrent, Color, LineWidth, DOT_STYLE_DFT);  // 4
      }


      if (Esp < 0)
        Esp += 4 * XCurrent + 6;
      else
      {
        Esp += 10 + 4 * (XCurrent - YCurrent);
        --YCurrent;
      }
      ++XCurrent;
    }
  }

  if (FlagLocalDebug) log_printf(__LINE__, __func__, "Exiting  LCD_draw_circle()\n\n");

  return;
}





/* $PAGE */
/* $TITLE=LCD_draw_image() */
/* ============================================================================================================================================================= *\
                                                                      Splash an image to framebuffer.
                                                  LineStart    : Pixel line coordinate where to start image splash.
                                                  ColumnStart  : Pixel column coordinate where to start image splash.
                                                  Image        ： Pointer to image start address.
                                                  ImageHeight  ： Number of lines in the image (image height).
                                                  ImageWidth    : Number of columns in the image (image width).
\* ============================================================================================================================================================= */
void LCD_draw_image(UINT16 LineStart, UINT16 ColumnStart, const UINT16 *Image, UINT16 ImageHeight, UINT16 ImageWidth) 
{
#ifdef RELEASE_VERSION
  UINT8 FlagLocalDebug = FLAG_OFF;  // must be turned OFF at all time.
#else   // RELEASE_VERSION
  UINT8 FlagLocalDebug = FLAG_OFF;  // may be turned ON for debug purposes.
#endif  // RELEASE_VERSION

  UINT Loop1UInt;
  UINT Loop2UInt;

  UINT16 PixelWord;


  if (FlagLocalDebug) log_printf(__LINE__, __func__, "Entering LCD_draw_image()\n");

  for (Loop1UInt = 0; Loop1UInt < ImageHeight; ++Loop1UInt)
  {
    for (Loop2UInt = 0; Loop2UInt < ImageWidth; ++Loop2UInt)
    {
      if ((ColumnStart + Loop2UInt < StructLCD.Width) && (LineStart + Loop1UInt < StructLCD.Height))  // exceeding part will not show up.
      {
        PixelWord = util_reverse_word(Image[(Loop1UInt * ImageWidth) + Loop2UInt + 1]);
        LCD_set_pixel(LineStart + Loop1UInt, ColumnStart + Loop2UInt, PixelWord);
      }
		}
	}

  if (FlagLocalDebug) log_printf(__LINE__, __func__, "Exiting  LCD_draw_image()\n\n");

  return;
}





/* $PAGE */
/* $TITLE=LCD_draw_line() */
/* ============================================================================================================================================================= *\
                                                                  Draw a line with arbitrary slope.
                                                             LineStart   ：Starting pixel line coordinate.
                                                             ColumnStart ：Starting pixel column coordinate.
                                                             LineEnd     ：Ending pixel line coordinate.
                                                             ColumnEnd   ：Ending pixel column coordinate.
                                                             Color       ：The color of the line to be drawn.
                                                             LineWidth    : Width of the line to be drawn.
                                                             LineStyle    : Solid or dotted lines
\* ============================================================================================================================================================= */
void LCD_draw_line(UINT16 LineStart, UINT16 ColumnStart, UINT16 LineEnd, UINT16 ColumnEnd, UINT16 Color, DOT_PIXEL LineWidth, LINE_STYLE LineStyle)
{
#ifdef RELEASE_VERSION
  UINT8 FlagLocalDebug = FLAG_OFF;  // must be turned OFF at all time.
#else   // RELEASE_VERSION
  UINT8 FlagLocalDebug = FLAG_OFF;  // may be turned ON for debug purposes.
#endif  // RELEASE_VERSION

  char DottedLength;

  UCHAR String[32];

  INT Esp;
  INT LineDirection;
  INT ColumnDirection;

  INT16 DeltaX;
  INT16 DeltaY;

  UINT16 XCoordinate;
  UINT16 YCoordinate;


  if (FlagLocalDebug)
  {
    switch (LineStyle)
    {
      case (LINE_STYLE_SOLID):
        strcpy(String, "LINE_STYLE_SOLID");
      break;
  
      case (LINE_STYLE_DOTTED):
        strcpy(String, "LINE_STYLE_DOTTED");
      break;
  
      default:
        strcpy(String, "Undefined");
      break;
    }
  
    log_printf(__LINE__, __func__, "Entering LCD_draw_line from (%u,%u) to (%u,%u)   Pixel thickness: %u   Line style: %s\n", LineStart, ColumnStart, LineEnd, ColumnEnd, LineWidth, String);
  }

  /* Validate LineStart coordinate. Let the firmware continue so that the part of the line that is visible will be displayed. */
  if (LineStart > (StructLCD.Height - 1))
    log_printf(__LINE__, __func__, "LineStart coordinate exceeds display boundaries (%u VS %u).\n", LineStart, (StructLCD.Height - 1));

  /* Validate LineEnd coordinate. Let the firmware continue so that the part of the line that is visible will be displayed. */
  if (LineEnd > (StructLCD.Height - 1))
    log_printf(__LINE__, __func__, "LineEnd coordinate exceeds display boundaries (%u VS %u).\n", LineEnd, (StructLCD.Height - 1));

  /* Validate ColumnStart coordinate. Let the firmware continue so that the part of the line that is visible will be displayed. */
  if (ColumnStart > (StructLCD.Width - 1))
    log_printf(__LINE__, __func__, "ColumnStart coordinate exceeds display boundaries (%u VS %u).\n", ColumnStart, (StructLCD.Width - 1));

  /* Validate ColumnEnd coordinate. Let the firmware continue so that the part of the line that is visible will be displayed. */
  if (ColumnEnd > (StructLCD.Width - 1))
    log_printf(__LINE__, __func__, "ColumnEnd coordinate exceeds display boundaries (%u VS %u).\n", ColumnEnd, (StructLCD.Width - 1));


  /* Set line and column start point coordinates. */
  XCoordinate = LineStart;
  YCoordinate = ColumnStart;

  /* Set the delta between start and end coordinates for line and column. */
  DeltaX = (int)LineEnd   - (int)LineStart   <= 0 ? LineEnd   - LineStart   : LineStart   - LineEnd;
  DeltaY = (int)ColumnEnd - (int)ColumnStart >= 0 ? ColumnEnd - ColumnStart : ColumnStart - ColumnEnd;

  /* Set increment direction, 1 is positive, -1 is negative. */
  LineDirection   = LineStart   < LineEnd   ? 1 : -1;
  ColumnDirection = ColumnStart < ColumnEnd ? 1 : -1;

  /* Cumulative error. */
  Esp = DeltaY + DeltaX;
  DottedLength = 0;

  if (FlagLocalDebug)
  {
    log_printf(__LINE__, __func__, "XCoordinate:     %u\n", XCoordinate);
    log_printf(__LINE__, __func__, "YCoordinate:     %u\n", YCoordinate);
    log_printf(__LINE__, __func__, "DeltaX:          %d\n", DeltaX);
    log_printf(__LINE__, __func__, "DeltaY:          %d\n", DeltaY);
    log_printf(__LINE__, __func__, "LineDirection:   %d\n", LineDirection);
    log_printf(__LINE__, __func__, "ColumnDirection: %d\n", ColumnDirection);
    log_printf(__LINE__, __func__, "Esp:             %d\n", Esp);
  }

  while(1)
  {
    ++DottedLength;
    /* Painted dotted line, 2 points is really virtual. */
    if ((LineStyle == LINE_STYLE_DOTTED) && ((DottedLength % 3) == 0))
    {
      LCD_draw_point(XCoordinate, YCoordinate, IMAGE_BACKGROUND, LineWidth, DOT_STYLE_DFT);  // default = DOT_FILLED_AROUND
      DottedLength = 0;
    }
    else
    {
      LCD_draw_point(XCoordinate, YCoordinate, Color, LineWidth, DOT_FILL_RIGHTUP);
    }
    
    if ((2 * Esp) >= DeltaX)
    {
      if (YCoordinate == ColumnEnd) break;
      Esp += DeltaX;
      YCoordinate += ColumnDirection;
    }
    
    if ((2 * Esp) <= DeltaY)
    {
      if (XCoordinate == LineEnd) break;
      Esp += DeltaY;
      XCoordinate += LineDirection;
    }
  }

  if (FlagLocalDebug) log_printf(__LINE__, __func__, "Exiting  LCD_draw_line()\n\n");

  return;
}





/* $PAGE */
/* $TITLE=LCD_draw_point() */
/* ============================================================================================================================================================= *\
                                                                      Draw a point of the selected color.
                                                           LineNumber   : Pixel line where to draw the point.
                                                           ColumnNumber : Pixel column where to draw the point.
                                                           Color        : Color of the point to be drawn.
                                                           DotPixel     : Size of the point to be drawn.
                                                           DotStyle     : Style of the point to be drawn.
   NOTE: DotStyle may be DOT_FILL_RIGHTUP or DOT_FILL_AROUND.
         DOT_FILL_RIGHTUP is intuitive and corresponds wo what we do expect. A DOT_PIXEL_3X3, for example, will draw a point of 3 pixels by 3 pixels dot on the display.
         DOT_FILL_AROUND  is less intuitive. A DOT_PIXEL_3X3, for example, will draw a small empty "box" 5 X 5, surrounding the specified 3 X 3 point specified.
\* ============================================================================================================================================================= */
void LCD_draw_point(UINT16 LineNumber, UINT16 ColumnNumber, UINT16 Color, DOT_PIXEL DotPixel, DOT_STYLE DotStyle)
{
#ifdef RELEASE_VERSION
  UINT8 FlagLocalDebug = FLAG_OFF;  // must be turned OFF at all time.
#else   // RELEASE_VERSION
  UINT8 FlagLocalDebug = FLAG_OFF;  // may be turned ON for debug purposes.
#endif  // RELEASE_VERSION

  UCHAR String[32];

  INT16 LinePixel;
  INT16 ColumnPixel;

 
  if (FlagLocalDebug)
  {
    switch (DotStyle)
    {
      case (DOT_FILL_AROUND):
        strcpy(String, "DOT_FILL_AROUND");
      break;
  
      case (DOT_FILL_RIGHTUP):
        strcpy(String, "DOT_FILL_RIGHTUP");
      break;
  
      default:
        strcpy(String, "Undefined");
      break;
    }
  
    log_printf(__LINE__, __func__, "Entering LCD_draw_point()\n");
    log_printf(__LINE__, __func__, "LineNumber: %u   ColumnNumber: %u   Color: 0x%8.8X   DotPixel: %u   DotStyle: %s\n", LineNumber, ColumnNumber, Color, DotPixel, String);
    log_printf(__LINE__, __func__, "DOT_FILL_AROUND: %u   DOT_FILL_RIGHTUP: %u\n", DOT_FILL_AROUND, DOT_FILL_RIGHTUP);
    log_printf(__LINE__, __func__, "2 * DotPixel - 1 = %d\n", 2 * DotPixel - 1);
  }

  /* Validate line coordinate. */
  if (LineNumber > (StructLCD.Height - 1))
  {
    if (FlagLocalDebug) log_printf(__LINE__, __func__, "Line coordinate exceeds display boundaries (%u VS %u).\n", LineNumber, (StructLCD.Height - 1));
    return;
  }

  /* Validate column coordinate. */
  if (ColumnNumber > (StructLCD.Width - 1))
  {
    if (FlagLocalDebug) log_printf(__LINE__, __func__, "Column coordinate exceeds display boundaries (%u VS %u).\n", ColumnNumber, (StructLCD.Width - 1));
    return;
  }

  if (DotStyle == DOT_FILL_AROUND)
  {
    /* Drawing edges of the specified point. */
    for (ColumnPixel = 0; ColumnPixel < 2 * DotPixel - 1; ++ColumnPixel)
    {
      for (LinePixel = 0; LinePixel < 2 * DotPixel - 1; ++LinePixel)
      {
        if ((LineNumber + LinePixel - DotPixel < 0) || (ColumnNumber + ColumnPixel - DotPixel < 0))
        {
          if (FlagLocalDebug) log_printf(__LINE__, __func__, "Do not print point: (LineNumber + LinePixel - DotPixel): %d   (ColumnNumber + ColumnPixel - DotPixel): %d\n", (LineNumber + LinePixel - DotPixel), (ColumnNumber + ColumnPixel - DotPixel));
          break;
        }
        else
        {
          if (FlagLocalDebug) log_printf(__LINE__, __func__, "Drawing edges: Setting pixel at position: LineNumber: %3d   ColumnNumber: %3d   with Color: 0x%4.4X\n", LineNumber + LinePixel - DotPixel, ColumnNumber + ColumnPixel - DotPixel, Color);
        }

        LCD_set_pixel(LineNumber + LinePixel - DotPixel, ColumnNumber + ColumnPixel - DotPixel, Color);
      }
    }
  }
  else
  {
    /* Drawing point while filling the inside. */
    for (ColumnPixel = 0; ColumnPixel < DotPixel; ++ColumnPixel)
    {
      for (LinePixel = 0; LinePixel < DotPixel; ++LinePixel)
      {
        if (FlagLocalDebug)
        {
          log_printf(__LINE__, __func__, "LineNumber: %u   LinePixel: %u   ColumnNumber: %u   ColumnPixel: %u   DotPixel: %u\n", LineNumber, LinePixel, ColumnNumber, ColumnPixel, DotPixel);
          log_printf(__LINE__, __func__, "Drawing filled: Setting pixel at position: LineNumber: %3d   ColumnNumber: %3d   with Color: 0x%8.8X\n", LineNumber + LinePixel - 1, ColumnNumber + ColumnPixel - 1, Color);
        }
        LCD_set_pixel(LineNumber + LinePixel, ColumnNumber + ColumnPixel, Color);
      }
    }
  }

  if (FlagLocalDebug) log_printf(__LINE__, __func__, "Exiting  LCD_draw_point()\n\n");

  return;
}





/* $PAGE */
/* $TITLE=LCD_exit() */
/* ============================================================================================================================================================= *\
                                                                       Module exits and closes SPI.
                                                                  Not really required, but good practice.
\* ============================================================================================================================================================= */
void LCD_exit(void)
{
#ifdef RELEASE_VERSION
  UINT8 FlagLocalDebug = FLAG_OFF;  // must be turned OFF at all time.
#else   // RELEASE_VERSION
  UINT8 FlagLocalDebug = FLAG_OFF;  // may be turned ON for debug purposes.
#endif  // RELEASE_VERSION

  if (FlagLocalDebug) log_printf(__LINE__, __func__, "Entering LCD_exit()\n");

  free(StructLCD.FrameBuffer);
  StructLCD.FrameBuffer = NULL;

  if (FlagLocalDebug) log_printf(__LINE__, __func__, "Exiting  LCD_exit()\n\n");

  return;
}





/* $PAGE */
/* $TITLE=LCD_hard_reset() */
/* ============================================================================================================================================================= *\
                                                    Force a hardware reset on the LCD display reset pin (active low).
\* ============================================================================================================================================================= */
void LCD_hard_reset(void)
{
#ifdef RELEASE_VERSION
  UINT8 FlagLocalDebug = FLAG_OFF;  // must be turned OFF at all time.
#else   // RELEASE_VERSION
  UINT8 FlagLocalDebug = FLAG_OFF;  // may be turned ON for debug purposes.
#endif  // RELEASE_VERSION

  if (FlagLocalDebug) log_printf(__LINE__, __func__, "Entering LCD_hard_reset()\n");
    
  /* In the case of Pico Explorer Base, LCD reset is not connected to Pico. */
  if (StructLCD.DeviceType == LCD_EXPLORER) return;

  gpio_put(StructLCD.GPIOReset, 1);
  sleep_ms(100);

  gpio_put(StructLCD.GPIOReset, 0);
  sleep_ms(100);

  gpio_put(StructLCD.GPIOReset, 1);
  sleep_ms(100);

  if (FlagLocalDebug) log_printf(__LINE__, __func__, "Exiting  LCD_hard_reset()\n\n");

  return;
}





/* $PAGE */
/* $TITLE=LCD_init() */
/* ============================================================================================================================================================= *\
                                                                           Initialize LCD device.
\* ============================================================================================================================================================= */
void LCD_init(UINT16 Rotation, UINT16 Color)
{
#ifdef RELEASE_VERSION
  UINT8 FlagLocalDebug = FLAG_OFF;  // must be turned OFF at all time.
#else   // RELEASE_VERSION
  UINT8 FlagLocalDebug = FLAG_OFF;  // may be turned ON for debug purposes.
#endif  // RELEASE_VERSION

  UCHAR String[32];


  memset(LCD_Separator, 0x3D, MAX_SCREEN_WIDTH);   // <=> for screen formatting.
  LCD_Separator[MAX_SCREEN_WIDTH - 2] = '\n';      // add line-feed terminator.
  LCD_Separator[MAX_SCREEN_WIDTH - 1] = '\0';      // add liend-of-string.

  memset(LCD_Separator1, 0x2D, MAX_SCREEN_WIDTH);  // <-> for screen formatting.
  LCD_Separator1[MAX_SCREEN_WIDTH - 2] = '\n';     // add line-feed terminator.
  LCD_Separator1[MAX_SCREEN_WIDTH - 1] = '\0';     // add end-of-string.


  if (FlagLocalDebug) log_printf(__LINE__, __func__, "Entering LCD_init()\n");

  /* ----------------------------------------------------------------------------------------------------------------------------------------------------------- *\
                                                          Assign device parameters for the selected device type.
  \* ----------------------------------------------------------------------------------------------------------------------------------------------------------- */
  switch (StructLCD.DeviceType)
  {
    case (LCD_114):
      if (FlagLocalDebug) log_printf(__LINE__, __func__, "Initializing LCD device as a Waveshare's 1.14 inch LCD display.\n");
      /* Line and column physical parameters are given for native mode (display in portrait mode). */
      StructLCD.PhysicalHeight = 240;
      StructLCD.PhysicalWidth  = 135;
      StructLCD.LineOffset     = 40;
      StructLCD.ColumnOffset   = 52;
      StructLCD.Rotation       = Rotation;
      StructLCD.MirrorMode     = MIRROR_NONE;
      StructLCD.Depth          = LCD_RGB_565;  // 2 bytes per pixel, 65k colors

      /* GPIO used by buttons integrated on LCD display. */
      StructLCD.GPIOButton[0] =  3;  // to be verified.
      StructLCD.GPIOButton[1] = 15;  // to be verified.
      StructLCD.GPIOButton[2] =  2;  // to be verified.
      StructLCD.GPIOButton[3] = 17;  // to be verified.

      /* GPIOs used for LCD display. */
      StructLCD.GPIODataCommand =  8;  // to be verified.   // data / command control pin.
      StructLCD.GPIOChipSelect  =  9;  // to be verified.   // chip select pin, active low.
      StructLCD.GPIOClock       = 10;  // to be verified.   // SPI clock pin.
      StructLCD.GPIOData        = 11;  // to be verified.   // SPI data pin.
      StructLCD.GPIOReset       = 12;  // to be verified.   // reset pin, active low.
      StructLCD.GPIOBrightness  = 13;  // to be verified.   // backlight pwm pin.

      /* SPI port used for communication with LCD display. */
      StructLCD.SPIPort = spi1;  // to be verified.
    break;

    case (LCD_200):
      if (FlagLocalDebug) log_printf(__LINE__, __func__, "Initializing LCD device as a Waveshare's 2.00 inch LCD display.\n");
      /* Line and column physical parameters are given for native mode (display in portrait mode). */
      StructLCD.PhysicalHeight = 320;
      StructLCD.PhysicalWidth  = 240;
      StructLCD.LineOffset     = 0;
      StructLCD.ColumnOffset   = 0;
      StructLCD.Rotation       = Rotation;
      StructLCD.MirrorMode     = MIRROR_NONE;
      StructLCD.Depth          = LCD_RGB_565;  // 2 bytes per pixel, 65k colors

      /* GPIO used by buttons integrated on LCD display. */
      StructLCD.GPIOButton[0] =  3;
      StructLCD.GPIOButton[1] = 15;
      StructLCD.GPIOButton[2] =  2;
      StructLCD.GPIOButton[3] = 17;

      /* GPIOs used for LCD display. */
      StructLCD.GPIODataCommand =  8;  // data / command control pin.
      StructLCD.GPIOChipSelect  =  9;  // chip select pin, active low.
      StructLCD.GPIOClock       = 10;  // SPI clock pin.
      StructLCD.GPIOData        = 11;  // SPI data pin.
      StructLCD.GPIOReset       = 12;  // reset pin, active low.
      StructLCD.GPIOBrightness  = 13;  // backlight pwm pin.

      /* SPI port used for communication with LCD display. */
      StructLCD.SPIPort = spi1;
    break;

    case (LCD_EXPLORER):
      if (FlagLocalDebug) log_printf(__LINE__, __func__, "Initializing LCD device as a Pimoroni's Pico Explorer Base module with LCD display.\n");
      /* Line and column physical parameters are given for native mode (LCD display to the right of the Pimoroni module). */
      StructLCD.PhysicalHeight = 240;
      StructLCD.PhysicalWidth  = 240;
      StructLCD.LineOffset     = 0;
      StructLCD.ColumnOffset   = 0;
      StructLCD.Rotation       = Rotation;
      StructLCD.MirrorMode     = MIRROR_NONE;
      StructLCD.Depth          = LCD_RGB_565;  // 2 bytes per pixel, 65k colors

      /* GPIO used by buttons integrated to the Pico-Explorer module. */
      StructLCD.GPIOButton[0] = 12;
      StructLCD.GPIOButton[1] = 13;
      StructLCD.GPIOButton[2] = 14;
      StructLCD.GPIOButton[3] = 15;

      /* GPIOs used for LCD display. */
      StructLCD.GPIODataCommand = 16;  // data / command control pin.
      StructLCD.GPIOChipSelect  = 17;  // chip select pin, active low.
      StructLCD.GPIOClock       = 18;  // SPI clock pin.
      StructLCD.GPIOData        = 19;  // SPI data pin.
      // StructLCD.GPIOReset       = 12;  // LCD reset is not connected to a Pico GPIO.
      // StructLCD.GPIOBrightness  = 13;  // backlight pwm pin - NOTE: There is no backlight control on the Pico Explorer Base.

      /* SPI port used for communication with LCD display. */
      StructLCD.SPIPort = spi0;
    break;

    default:
      log_printf(__LINE__, __func__, "Unrecognized LCD device specified: %u... unable to initialize\n", StructLCD.DeviceType);
      return;
    break;
  }


  /* ----------------------------------------------------------------------------------------------------------------------------------------------------------- *\
                                                                          Initialize GPIOs
  \* ----------------------------------------------------------------------------------------------------------------------------------------------------------- */
  if (FlagLocalDebug)
  {
    log_printf(__LINE__, __func__, "Before executing LCD_init_gpio()\n");
    log_printf(__LINE__, __func__, "Press <Enter> to proceed: ");
    input_string(String, sizeof(String), 0l);
  }
  LCD_init_gpio();   // initialize all GPIOs used by LCD display.


  /* ----------------------------------------------------------------------------------------------------------------------------------------------------------- *\
                                                             Perform a hardware reset on LCD display.
                                                 NOTE: There is no hardware reset available on Pico-Explorer-Base.
  \* ----------------------------------------------------------------------------------------------------------------------------------------------------------- */
  if (FlagLocalDebug)
  {
    log_printf(__LINE__, __func__, "Before executing LCD_hard_reset()\n");
    log_printf(__LINE__, __func__, "Press <Enter> to proceed: ");
    input_string(String, sizeof(String), 0l);
  }
  LCD_hard_reset();  // force hardware reset after GPIO have been initialized.


  /* ----------------------------------------------------------------------------------------------------------------------------------------------------------- *\
                                                                       Initialize framebuffer.
  \* ----------------------------------------------------------------------------------------------------------------------------------------------------------- */
  if (FlagLocalDebug)
  {
    log_printf(__LINE__, __func__, "Before executing LCD_init_framebuffer()\n");
    log_printf(__LINE__, __func__, "Press <Enter> to proceed: ");
    input_string(String, sizeof(String), 0l);
  }
  LCD_init_framebuffer(Rotation, Color);  // initialize virtual framebuffer according to specified rotation view.


  /* ----------------------------------------------------------------------------------------------------------------------------------------------------------- *\
                                            Initialize SPI communication with LCD display and PWM signal for display brightness.
                                                 NOTE: There is no PWM control on the brightness for Pico-Explorer-Base.
  \* ----------------------------------------------------------------------------------------------------------------------------------------------------------- */
  if (FlagLocalDebug)
  {
    log_printf(__LINE__, __func__, "Before executing LCD_init_LCDcomm()\n");
    log_printf(__LINE__, __func__, "Press <Enter> to proceed: ");
    input_string(String, sizeof(String), 0l);
  }
  LCD_init_LCDcomm();


  /* ----------------------------------------------------------------------------------------------------------------------------------------------------------- *\
                        Turn Off display (set brightness to 0) to prevent a quick glitch in LCD display until LCD_clear() has been done below.
                                                NOTE: There is no brightness control on the Pico-Explorer-base.
  \* ----------------------------------------------------------------------------------------------------------------------------------------------------------- */
  if (FlagLocalDebug)
  {
    log_printf(__LINE__, __func__, "Before executing LCD_set_brightness(0)\n");
    log_printf(__LINE__, __func__, "Press <Enter> to proceed: ");
    input_string(String, sizeof(String), 0l);
  }
  LCD_set_brightness(0);


  /* ----------------------------------------------------------------------------------------------------------------------------------------------------------- *\
                                                                    Initialize LCD display registers.
  \* ----------------------------------------------------------------------------------------------------------------------------------------------------------- */
  if (FlagLocalDebug)
  {
    log_printf(__LINE__, __func__, "Before executing LCD_init_register()\n");
    log_printf(__LINE__, __func__, "Press <Enter> to proceed: ");
    input_string(String, sizeof(String), 0l);
  }
  LCD_init_register();


  /* ----------------------------------------------------------------------------------------------------------------------------------------------------------- *\
                                                              Set LCD display rotation as requested by caller.
  \* ----------------------------------------------------------------------------------------------------------------------------------------------------------- */
  if (FlagLocalDebug)
  {
    log_printf(__LINE__, __func__, "Before executing LCD_set_rotation(%u)\n", Rotation);
    log_printf(__LINE__, __func__, "Press <Enter> to proceed: ");
    input_string(String, sizeof(String), 0l);
  }
  LCD_set_rotation(Rotation);


  /* ----------------------------------------------------------------------------------------------------------------------------------------------------------- *\
                                                Initialize LCD display dimensions according to specified rotation mode.
  \* ----------------------------------------------------------------------------------------------------------------------------------------------------------- */
  if (FlagLocalDebug)
  {
    log_printf(__LINE__, __func__, "Before executing LCD_set_window()\n");
    log_printf(__LINE__, __func__, "Press <Enter> to proceed: ");
    input_string(String, sizeof(String), 0l);
  }
  /* Define LCD display writable area. */
  LCD_set_window(0, 0, StructLCD.Height - 1, StructLCD.Width - 1);


  /* ----------------------------------------------------------------------------------------------------------------------------------------------------------- *\
                                                       Erase framebuffer and set specified color as background color.
  \* ----------------------------------------------------------------------------------------------------------------------------------------------------------- */
  if (FlagLocalDebug)
  {
    LCD_color_name(Color, String);
    log_printf(__LINE__, __func__, "Before executing LCD_clear(0x%4.4X - %s)\n", Color, String);
    log_printf(__LINE__, __func__, "Press <Enter> to proceed: ");
    input_string(String, sizeof(String), 0l);
  }
  LCD_clear(Color);


  /* ----------------------------------------------------------------------------------------------------------------------------------------------------------- *\
                                                                 Transfer framebuffer to LCD display.
  \* ----------------------------------------------------------------------------------------------------------------------------------------------------------- */
  if (FlagLocalDebug)
  {
    log_printf(__LINE__, __func__, "Before executing LCD_refresh()\n");
    log_printf(__LINE__, __func__, "Press <Enter> to proceed: ");
    input_string(String, sizeof(String), 0l);
  }
  LCD_refresh();


  /* ----------------------------------------------------------------------------------------------------------------------------------------------------------- *\
                                                                     Set maximum brightness (100%)
  \* ----------------------------------------------------------------------------------------------------------------------------------------------------------- */
  if (FlagLocalDebug)
  {
    log_printf(__LINE__, __func__, "Before executing LCD_set_brightness(100)\n");
    log_printf(__LINE__, __func__, "Press <Enter> to proceed: ");
    input_string(String, sizeof(String), 0l);
  }
  LCD_set_brightness(100);


  /* ----------------------------------------------------------------------------------------------------------------------------------------------------------- *\
                                                                     Stuffing default color list.
  \* ----------------------------------------------------------------------------------------------------------------------------------------------------------- */
  if (FlagLocalDebug)
  {
    log_printf(__LINE__, __func__, "Before stuffing color list.\n");
    log_printf(__LINE__, __func__, "Press <Enter> to proceed: ");
    input_string(String, sizeof(String), 0l);
  }
  StructLCD.Color[0]  = BLACK;
  StructLCD.Color[1]  = RED;
  StructLCD.Color[2]  = GREEN;
  StructLCD.Color[3]  = BLUE;
  StructLCD.Color[4]  = CYAN;
  StructLCD.Color[5]  = MAGENTA;
  StructLCD.Color[6]  = YELLOW;
  StructLCD.Color[7]  = BROWN;
  StructLCD.Color[8]  = BROWN_RED;
  StructLCD.Color[9]  = GREY;
  StructLCD.Color[10] = DARK_BLUE;
  StructLCD.Color[11] = DARK_GREEN;
  StructLCD.Color[12] = GREEN_YELLOW;
  StructLCD.Color[13] = TURQUOISE;
  StructLCD.Color[14] = JEANS_BLUE;
  StructLCD.Color[15] = LIGHT_GREEN;
  StructLCD.Color[16] = WHITE;

  strcpy(StructLCD.Mirroring[MIRROR_NONE],       "No mirroring");
  strcpy(StructLCD.Mirroring[MIRROR_HORIZONTAL], "Horizontal");
  strcpy(StructLCD.Mirroring[MIRROR_VERTICAL],   "Vertical");
  strcpy(StructLCD.Mirroring[MIRROR_ORIGIN],     "Origin");

  if (FlagLocalDebug) log_printf(__LINE__, __func__, "Exiting  LCD_init()\n\n");

  return;
}





/* $PAGE */
/* $TITLE=LCD_init_framebuffer() */
/* ============================================================================================================================================================= *\
                                     Initialize framebuffer where bitmap activity occurs before being transfered to the LCD display.
                                                       This function is usually called by LCD_init().
                                                       Height         : The height of the virtual image.
                                                       Width          : The width of the virtual image.
                                                       Rotation       : Direction in which the Framebuffer will be restored on LCD display.
                                                       Color          : Background color with which the Framebuffer will be initialized.
                                                       Depth          : Color - RGB - definition (1= 444 / 4k colors   2= 565 / 65k colors   3= 666 / 262k colors).
\* ============================================================================================================================================================= */
void LCD_init_framebuffer(UINT16 Rotation, UINT16)
{
#ifdef RELEASE_VERSION
  UINT8 FlagLocalDebug = FLAG_OFF;  // must be turned OFF at all time.
#else   // RELEASE_VERSION
  UINT8 FlagLocalDebug = FLAG_OFF;  // may be turned ON for debug purposes.
#endif  // RELEASE_VERSION

  UINT8 *Dum0Ptr;

  UINT32 TotalPixels;
  UINT32 Loop1UInt32;


  TotalPixels = (UINT32)(StructLCD.PhysicalHeight * StructLCD.PhysicalWidth);  // two bytes per pixel for color definition in RGB_565 mode.

  if (FlagLocalDebug)
  {
    log_printf(__LINE__, __func__, "Entering LCD_init_framebuffer()\n");
    log_printf(__LINE__, __func__, "StructLCD.FrameBuffer: 0x%p\n", StructLCD.FrameBuffer);
    log_printf(__LINE__, __func__, "StructLCD.PhysicalHeight: %4u     StructLCD.PhysicalWidth: %4u     StructLCD.Depth: %u\n", StructLCD.PhysicalHeight, StructLCD.PhysicalWidth, StructLCD.Depth);
    log_printf(__LINE__, __func__, "FrameBuffer size (total number of pixels): %lu (0x%lX)\n", TotalPixels, TotalPixels);
    log_printf(__LINE__, __func__, "Display RAM before framebuffer calloc:\n");
    Dum0Ptr = (UINT8 *)0x20006380;
    util_display_data(Dum0Ptr, StructLCD.PhysicalHeight * StructLCD.PhysicalWidth * StructLCD.Depth);
  }

  /* Create RAM FrameBuffer to manage virtual screen operations. */
  if (StructLCD.FrameBuffer == NULL)  // make sure we allocate memory only once.
  {
    if ((StructLCD.FrameBuffer = calloc(StructLCD.PhysicalHeight * StructLCD.PhysicalWidth, StructLCD.Depth)) != NULL)
    {
      if (FlagLocalDebug)
      {
        log_printf(__LINE__, __func__, "malloc() returned successfully 0x%p.\n", StructLCD.FrameBuffer);
        log_printf(__LINE__, __func__, "Framebuffer starts: 0x%p     Framebuffer ends: 0x%p\n", StructLCD.FrameBuffer, (UINT8 *)StructLCD.FrameBuffer + (TotalPixels * StructLCD.Depth));

        /* NOTE: Header points to the first byte of the block available for next calloc(). */
        log_printf(__LINE__, __func__, "Display FrameBuffer header:\n");
        Dum0Ptr = ((UINT8 *)StructLCD.FrameBuffer) - 8;
        util_display_data(Dum0Ptr, 8);

        log_printf(__LINE__, __func__, "Display RAM area after framebuffer calloc:\n");
        util_display_data((UINT8 *)StructLCD.FrameBuffer, StructLCD.PhysicalHeight * StructLCD.PhysicalWidth * StructLCD.Depth);
      }
    }
    else
    {
      log_printf(__LINE__, __func__, "Failed to allocate virtual FrameBuffer.\n");
      exit(0);
    }
  }
  else
  {
    log_printf(__LINE__, __func__, "StructLCD.FrameBuffer has already been allocated before and remains to: 0x%p\n", StructLCD.FrameBuffer);
    log_printf(__LINE__, __func__, "Re-initialize StructLCD.FrameBuffer\n");
    for (Loop1UInt32 = 0; Loop1UInt32 < (StructLCD.PhysicalHeight * StructLCD.PhysicalWidth); ++Loop1UInt32)
      StructLCD.FrameBuffer[Loop1UInt32] = 0x0000;
  }


  StructLCD.Rotation = Rotation;

  if ((StructLCD.Rotation == ROTATE_0) || (StructLCD.Rotation == ROTATE_180))
  {
    StructLCD.Height  = StructLCD.PhysicalHeight;
    StructLCD.Width   = StructLCD.PhysicalWidth;
  }
  else
  {
    StructLCD.Height  = StructLCD.PhysicalWidth;
    StructLCD.Width   = StructLCD.PhysicalHeight;
  }

  if (FlagLocalDebug) log_printf(__LINE__, __func__, "Exiting  LCD_init_framebuffer()\n\n");

  return;
}





/* $PAGE */
/* $TITLE=LCD_init_gpio() */
/* ============================================================================================================================================================= *\
                                 Initialize GPIO's used for LCD module SPI communication and also the 4 buttons integrated to the device.
                                                                 This function is usually called by LCD_init().

                                               Note: Function LCD_init_button() must be used to initialize four buttons' GPIOs.
\* ============================================================================================================================================================= */
void LCD_init_gpio(void)
{
#ifdef RELEASE_VERSION
  UINT8 FlagLocalDebug = FLAG_OFF;  // must be turned OFF at all time.
#else   // RELEASE_VERSION
  UINT8 FlagLocalDebug = FLAG_OFF;  // may be turned ON for debug purposes.
#endif  // RELEASE_VERSION

  UINT16 Loop1UInt16;


  if (FlagLocalDebug) log_printf(__LINE__, __func__, "Entering LCD_init_gpio()\n");

  /* LCD reset doesn't seem to be connected to Pico in the case of the Pico Explorer Base. */
  if (StructLCD.DeviceType != LCD_EXPLORER)
  {
    gpio_init(StructLCD.GPIOReset);
    gpio_set_dir(StructLCD.GPIOReset, GPIO_OUT);
  }

  gpio_init(StructLCD.GPIODataCommand);
  gpio_set_dir(StructLCD.GPIODataCommand, GPIO_OUT);

  gpio_init(StructLCD.GPIOChipSelect);
  gpio_set_dir(StructLCD.GPIOChipSelect, GPIO_OUT);
  gpio_put(StructLCD.GPIOChipSelect, 1);  // disable chip select.

  /* Initialize the 4 buttons integrated to the device. */
  for (Loop1UInt16 = 0; Loop1UInt16 < 4; ++Loop1UInt16)
  {
    gpio_init(StructLCD.GPIOButton[Loop1UInt16]);
    gpio_set_dir(StructLCD.GPIOButton[Loop1UInt16], GPIO_IN);
    gpio_pull_up(StructLCD.GPIOButton[Loop1UInt16]);
  }

  if (FlagLocalDebug) log_printf(__LINE__, __func__, "Exiting  LCD_init_gpio()\n\n");

  return;
}





/* $PAGE */
/* $TITLE=LCD_init_LCDcomm() */
/* ============================================================================================================================================================= *\
                                    Initialize GPIO's used for LCD module SPI communication and initialize PWM for backlight display.
                                                          This function is usually called by LCD_init()
\* ============================================================================================================================================================= */
void LCD_init_LCDcomm(void)
{
#ifdef RELEASE_VERSION
  UINT8 FlagLocalDebug = FLAG_OFF;  // must be turned OFF at all time.
#else   // RELEASE_VERSION
  UINT8 FlagLocalDebug = FLAG_OFF;  // may be turned ON for debug purposes.
#endif  // RELEASE_VERSION


  if (FlagLocalDebug) log_printf(__LINE__, __func__, "Entering LCD_init_LCDcomm()\n");

  spi_init(StructLCD.SPIPort, 4000000);
    
  gpio_set_function(StructLCD.GPIOClock, GPIO_FUNC_SPI);
  gpio_set_function(StructLCD.GPIOData,  GPIO_FUNC_SPI);
    
  /* Initialize PWM signal used to control LCD display brightness.
     NOTE: Pico Explorer Base has no LCD display brightness control (at least in the hardware version that I have). */
  if (StructLCD.DeviceType != LCD_EXPLORER)
  {
    gpio_set_function(StructLCD.GPIOBrightness, GPIO_FUNC_PWM);
    SliceNumber = pwm_gpio_to_slice_num(StructLCD.GPIOBrightness);
    pwm_set_wrap(SliceNumber, 100);
  
    pwm_set_chan_level(SliceNumber, PWM_CHAN_B, 10);
  
    pwm_set_clkdiv(SliceNumber, 50);
    pwm_set_enabled(SliceNumber, true);
  }
    
  if (FlagLocalDebug) log_printf(__LINE__, __func__, "Exiting  LCD_init_LCDcomm()\n\n");

  return;
}





/* $PAGE */
/* $TITLE=LCD_init_register() */
/* ============================================================================================================================================================= *\
                                                                        Initialize LCD114 display registers.
\* ============================================================================================================================================================= */
void LCD_init_register(void)
{
#ifdef RELEASE_VERSION 
  UINT8 FlagLocalDebug = FLAG_OFF;  // must be turned OFF at all time.
#else   // RELEASE_VERSION
  UINT8 FlagLocalDebug = FLAG_OFF;  // may be turned ON for debug purposes.
#endif  // RELEASE_VERSION


  if (FlagLocalDebug) log_printf(__LINE__, __func__, "Entering LCD_init_register()\n");

  /* Define Control Interface Color Format: 16 bits per pixel ("Mode: 565") 65k colors. */
  LCD_send_command(0x3A);
  LCD_send_data_8bit(0x05);

	/* Back and front porch settings. */
  LCD_send_command(0xB2);
  LCD_send_data_8bit(0x0C);
  LCD_send_data_8bit(0x0C);
  LCD_send_data_8bit(0x00);
  LCD_send_data_8bit(0x33);
  LCD_send_data_8bit(0x33);

  /* Gate control. */
  LCD_send_command(0xB7);
  LCD_send_data_8bit(0x35);

  /* VCOM setting. */
  LCD_send_command(0xBB);
  LCD_send_data_8bit(0x19);

  /* LCM control setting. */
  LCD_send_command(0xC0);
  LCD_send_data_8bit(0x2C);

  /* VDV and VRH command enable. */
  LCD_send_command(0xC2);
  LCD_send_data_8bit(0x01);
  
  /* VRH setting. */
  LCD_send_command(0xC3);
  LCD_send_data_8bit(0x12);
  
  /* VDV setting. */
  LCD_send_command(0xC4);
  LCD_send_data_8bit(0x20);

  /* Frame rate control in normal mode. */
  LCD_send_command(0xC6);
  LCD_send_data_8bit(0x0F);
    
  /* Power control 1. */
  LCD_send_command(0xD0);
  LCD_send_data_8bit(0xA4);
  LCD_send_data_8bit(0xA1);

  /* Positive voltage gamma control. */
  LCD_send_command(0xE0);
  LCD_send_data_8bit(0xD0);
  LCD_send_data_8bit(0x04);
  LCD_send_data_8bit(0x0D);
  LCD_send_data_8bit(0x11);
  LCD_send_data_8bit(0x13);
  LCD_send_data_8bit(0x2B);
  LCD_send_data_8bit(0x3F);
  LCD_send_data_8bit(0x54);
  LCD_send_data_8bit(0x4C);
  LCD_send_data_8bit(0x18);
  LCD_send_data_8bit(0x0D);
  LCD_send_data_8bit(0x0B);
  LCD_send_data_8bit(0x1F);
  LCD_send_data_8bit(0x23);

  /* Negative voltage gamma control. */
  LCD_send_command(0xE1);
  LCD_send_data_8bit(0xD0);
  LCD_send_data_8bit(0x04);
  LCD_send_data_8bit(0x0C);
  LCD_send_data_8bit(0x11);
  LCD_send_data_8bit(0x13);
  LCD_send_data_8bit(0x2C);
  LCD_send_data_8bit(0x3F);
  LCD_send_data_8bit(0x44);
  LCD_send_data_8bit(0x51);
  LCD_send_data_8bit(0x2F);
  LCD_send_data_8bit(0x1F);
  LCD_send_data_8bit(0x1F);
  LCD_send_data_8bit(0x20);
  LCD_send_data_8bit(0x23);

	/* Set display inversion On. */
  LCD_send_command(0x21);

  /* Sleep out (turn Off sleep mode) and start LCD scan. */
  LCD_send_command(0x11);

	/* Display On (recover from Display Off mode). */
  LCD_send_command(0x29);

  if (FlagLocalDebug) log_printf(__LINE__, __func__, "Exiting  LCD_init_register()\n\n");

  return;
}





/* $PAGE */
/* $TITLE=LCD_mirror_framebuffer() */
/* ============================================================================================================================================================= *\
                                Convert specified area of current framebuffer content according to specified mirroring orientation.
      Keep in mind that MirrorMode is kept in the main LCD structure but if only a part of LCD display has been converted, the coordinates of this LCD area
                     are not saved. The application will have to keep track of this section of the display if required by the program's logic.
\* ============================================================================================================================================================= */
void LCD_mirror_framebuffer(UINT16 LineStart, UINT16 ColumnStart, UINT16 LineEnd, UINT16 ColumnEnd, UINT8 MirroringType)
{
#ifdef RELEASE_VERSION 
  UINT8 FlagLocalDebug = FLAG_OFF;  // must be turned OFF at all time.
#else   // RELEASE_VERSION
  UINT8 FlagLocalDebug = FLAG_OFF;  // may be turned ON for debug purposes.
#endif  // RELEASE_VERSION

  UINT16 Dum1UInt16;
  UINT16 LineNumber;
  UINT16 ColumnNumber;
  UINT16 Loop1UInt16;


  if (FlagLocalDebug) log_printf(__LINE__, __func__, "Current rotation setting is: %u degrees\n", StructLCD.Rotation);

  /* Make sure coordinates ordering is correct. */
  if (LineEnd < LineStart)
  {
    Dum1UInt16 = LineStart;
    LineStart  = LineEnd;
    LineEnd    = Dum1UInt16;
  }

  if (ColumnEnd < ColumnStart)
  {
    Dum1UInt16  = ColumnStart;
    ColumnStart = ColumnEnd;
    ColumnEnd   = Dum1UInt16;
  }


  if (MirroringType == MIRROR_NONE)
  {
    /* If a request is done to restore FrameBuffer to MIRROR_NONE, it means that we must redo the same process that has been done already... */
    /* If display is currentely in MIRROR_HORIZONTAL, making another "MIRROR_HORIZONTAL" will, in fact, restore the display in MIRROR_NONE mode. */
    LCD_mirror_framebuffer(LineStart, ColumnStart, LineEnd, ColumnEnd, StructLCD.MirrorMode);
    StructLCD.MirrorMode = MIRROR_NONE;
  }



  if (MirroringType == MIRROR_HORIZONTAL)
  {
    /* Process FrameBuffer horizontal mirroring for specified area. */
    for (LineNumber = LineStart; LineNumber < LineEnd; ++LineNumber)
    {
      for (Loop1UInt16 = 0; Loop1UInt16 < ((ColumnEnd - ColumnStart) / 2) + 1; ++Loop1UInt16)
      {
        // log_printf(__LINE__, __func__, "Exchanging framebuffer[%u] with framebuffer[%u]\n", (LineNumber * StructLCD.Width) + ColumnStart + Loop1UInt16, (LineNumber * StructLCD.Width) + (ColumnEnd - Loop1UInt16));
        Dum1UInt16 = StructLCD.FrameBuffer[(LineNumber * StructLCD.Width) + ColumnStart + Loop1UInt16];
        StructLCD.FrameBuffer[(LineNumber * StructLCD.Width) + (ColumnStart + Loop1UInt16)] = StructLCD.FrameBuffer[(LineNumber * StructLCD.Width) + (ColumnEnd - Loop1UInt16)];
        StructLCD.FrameBuffer[(LineNumber * StructLCD.Width) + (ColumnEnd - Loop1UInt16)]   = Dum1UInt16;
      }
    }
    StructLCD.MirrorMode = MIRROR_HORIZONTAL;
  }



  if (MirroringType == MIRROR_VERTICAL)
  {
    /* Process FrameBuffer vertical mirroring for specified area. */
    for (ColumnNumber = ColumnStart; ColumnNumber < ColumnEnd; ++ColumnNumber)
    {
      for (Loop1UInt16 = 0; Loop1UInt16 < ((LineEnd - LineStart) / 2) + 1; ++Loop1UInt16)
      {
        // log_printf(__LINE__, __func__, "Exchanging framebuffer[%u] with framebuffer[%u]\n", ((LineStart + Loop1UInt16) * StructLCD.Width) + ColumnNumber, ((LineEnd - Loop1UInt16) * StructLCD.Width + ColumnNumber));
        Dum1UInt16 = StructLCD.FrameBuffer[((LineStart + Loop1UInt16) * StructLCD.Width) + ColumnNumber];
        StructLCD.FrameBuffer[((LineStart + Loop1UInt16) * StructLCD.Width) + ColumnNumber] = StructLCD.FrameBuffer[(LineEnd - Loop1UInt16) * StructLCD.Width + ColumnNumber];
        StructLCD.FrameBuffer[(LineEnd - Loop1UInt16) * StructLCD.Width + ColumnNumber] = Dum1UInt16;
      }
    }
    StructLCD.MirrorMode = MIRROR_VERTICAL;
  }



  if (MirroringType == MIRROR_ORIGIN)
  {
    /* Process FrameBuffer origin mirroring for specified area. */
    /* NOTE: ORIGIN mirroring is equivalent of changing rotation by 180 degrees relative to current rotation setting. */
    for (ColumnNumber = ColumnStart; ColumnNumber < (ColumnEnd) / 2; ++ColumnNumber)
    {
      for (Loop1UInt16 = 0; Loop1UInt16 < (LineEnd - LineStart) + 1; ++Loop1UInt16)
      {
        // log_printf(__LINE__, __func__, "Exchanging framebuffer[%u] with framebuffer[%u]\n", ((LineStart + Loop1UInt16) * StructLCD.Width) + ColumnNumber, ((LineEnd - Loop1UInt16 + 1) * StructLCD.Width - (ColumnNumber + 1)));
        Dum1UInt16 = StructLCD.FrameBuffer[((LineStart + Loop1UInt16) * StructLCD.Width) + ColumnNumber];
        StructLCD.FrameBuffer[((LineStart + Loop1UInt16) * StructLCD.Width) + ColumnNumber] = StructLCD.FrameBuffer[(LineEnd - Loop1UInt16 + 1) * StructLCD.Width - (ColumnNumber + 1)];
        StructLCD.FrameBuffer[(LineEnd - Loop1UInt16 + 1) * StructLCD.Width - (ColumnNumber + 1)] = Dum1UInt16;
      }
    }
    StructLCD.MirrorMode = MIRROR_ORIGIN;
  }

  LCD_refresh();

  return;
 }





/* $PAGE */
/* $TITLE=LCD_part_clear() */
/* ============================================================================================================================================================= *\
                                              Partially clear Framebuffer (RAM memory) while setting color as specified.
                            NOTE: Function <LCD_refresh()> must then be called to transfer the content of the framebuffer to the LCD display.
\* ============================================================================================================================================================= */
void LCD_part_clear(UINT16 LineStart, UINT16 ColumnStart, UINT16 LineEnd, UINT16 ColumnEnd, UINT16 Color)
{
#ifdef RELEASE_VERSION
  UINT8 FlagLocalDebug = FLAG_OFF;  // must be turned OFF at all time.
#else   // RELEASE_VERSION
  UINT8 FlagLocalDebug = FLAG_OFF;  // may be turned ON for debug purposes.
#endif  // RELEASE_VERSION

  UCHAR  String[32];

  UINT16 LineNumber;
  UINT16 ColumnNumber;

  if (FlagLocalDebug)
  {
    LCD_color_name(Color, String);
    log_printf(__LINE__, __func__, "Entering LCD_part_clear(Color: 0x%4.4X - %s)\n", Color, String);
    log_printf(__LINE__, __func__, "Rotation currently set to %u degrees.\n", StructLCD.Rotation);
    log_printf(__LINE__, __func__, "StructLCD.PhysicalHeight:  %3u   StructLCD.PhysicalWidth: %3u   StructLCD.Depth: %u\n",  StructLCD.PhysicalHeight, StructLCD.PhysicalWidth, StructLCD.Depth);
    log_printf(__LINE__, __func__, "StructLCD.Height:          %3u   StructLCD.Width:         %3u\n",                         StructLCD.Height,         StructLCD.Width);
    log_printf(__LINE__, __func__, "Size of Framebuffer: %u X %u = %lu pixels (hex: 0x%8.8lX) or %lu bytes (hex: 0x%lX)\n",
               StructLCD.PhysicalHeight, StructLCD.PhysicalWidth, StructLCD.PhysicalHeight * StructLCD.PhysicalWidth, StructLCD.PhysicalHeight * StructLCD.PhysicalWidth,
               StructLCD.PhysicalHeight * StructLCD.PhysicalWidth * StructLCD.Depth, StructLCD.PhysicalHeight * StructLCD.PhysicalWidth * StructLCD.Depth);
    sleep_ms(2000);
  }

  for (LineNumber = LineStart; LineNumber <= LineEnd; ++LineNumber)
  {
    for (ColumnNumber = ColumnStart; ColumnNumber <= ColumnEnd; ++ColumnNumber)
    {
      StructLCD.FrameBuffer[(LineNumber * StructLCD.Width) + ColumnNumber] = Color;
    }
  }

  if (FlagLocalDebug) log_printf(__LINE__, __func__, "Exiting  LCD_part_clear()\n\n");

  return;
}





/* $PAGE */
/* $TITLE=LCD_part_cls() */
/* ============================================================================================================================================================= *\
                                               Partially erase LCD display while setting the specified background color.
              NOTES: The framebuffer remains unchanged and still contains its data. A call to LCD_refresh() will restore the current framebuffer content.
                     Since the LCD_part_cls() function doesn't work with the framebuffer, we need to emulate rotation and / or mirroring by Firmware.
\* ============================================================================================================================================================= */
void LCD_part_cls(UINT16 LineStart, UINT16 ColumnStart, UINT16 LineEnd, UINT16 ColumnEnd, UINT16 Color)
{
#ifdef RELEASE_VERSION
  UINT8 FlagLocalDebug = FLAG_OFF;  // must be turned OFF at all time.
#else   // RELEASE_VERSION
  UINT8 FlagLocalDebug = FLAG_OFF;  // may be turned ON for debug purposes.
#endif  // RELEASE_VERSION

  UCHAR String[32];

  UINT16 LineNumber;
  UINT16 ColumnNumber;
  UINT16 Width;
  UINT16 Height;


  if (FlagLocalDebug)
  {
    Height = LineEnd   - LineStart;
    Width  = ColumnEnd - ColumnStart;
    LCD_color_name(Color, String);
    log_printf(__LINE__, __func__, "Entering LCD_part_cls(Color: 0x%4.4X - %s)\n", Color, String);
    log_printf(__LINE__, __func__, "Rotation currently set to %u-degree.\n", StructLCD.Rotation);
    log_printf(__LINE__, __func__, "StructLCD.PhysicalHeight:  %3u   StructLCD.PhysicalWidth:  %3u\n", StructLCD.PhysicalHeight, StructLCD.PhysicalWidth);
    log_printf(__LINE__, __func__, "StructLCD.Height:          %3u   StructLCD.Width:          %3u\n", StructLCD.Height,  StructLCD.Width);
    log_printf(__LINE__, __func__, "Size of area to erase: %u X %u = %lu pixels (hex: 0x%8.8lX) of depth %u or %lu bytes.\n", Height, Width, Width * Height, Width * Height, StructLCD.Depth, Width * Height * StructLCD.Depth);
    log_printf(__LINE__, __func__, "Original   coordinates of area to erase: (%u,%u) to (%u,%u)\n", LineStart, ColumnStart, LineEnd, ColumnEnd);
  }

  /* Define LCD display writable area. */
  LCD_set_window(LineStart, ColumnStart, LineEnd, ColumnEnd);

  gpio_put(StructLCD.GPIODataCommand, 1);  // set command mode.
  gpio_put(StructLCD.GPIOChipSelect,  0);  // enable chip select active low.
  
  if (FlagLocalDebug) log_printf(__LINE__, __func__, "Before spi_write_blocking()\n");

  for (LineNumber = LineStart; LineNumber <= LineEnd; ++LineNumber)
    for (ColumnNumber = ColumnStart; ColumnNumber <= ColumnEnd; ++ColumnNumber)
      spi_write_blocking(StructLCD.SPIPort, (UINT8 *)&Color, StructLCD.Depth);

  gpio_put(StructLCD.GPIOChipSelect, 1);  // disable chip select when done.

  /* Reset LCD display writable area for full LCD display. */
  LCD_set_window(0, 0, StructLCD.Height - 1, StructLCD.Width - 1);

  if (FlagLocalDebug) log_printf(__LINE__, __func__, "Exiting LCD_part_cls()\n\n");

  return;
}





/* $PAGE */
/* $TITLE=LCD_part_refresh() */
/* ============================================================================================================================================================= *\
                                                Refresh part of LCD display (faster than a "plain" LCD_refresh()).
\* ============================================================================================================================================================= */
void LCD_part_refresh(UINT16 LineStart, UINT16 ColumnStart, UINT16 LineEnd, UINT16 ColumnEnd)
{
  UINT16 LineNumber;
  UINT16 ColumnNumber;


  /* Define LCD display writable area. */
  LCD_set_window(LineStart, ColumnStart, LineEnd, ColumnEnd);

  gpio_put(StructLCD.GPIODataCommand, 1);  // high for data, low for command.
  gpio_put(StructLCD.GPIOChipSelect,  0);

  for (LineNumber = LineStart; LineNumber <= LineEnd; ++LineNumber)
    for (ColumnNumber = ColumnStart; ColumnNumber <= ColumnEnd; ++ColumnNumber)
      spi_write_blocking(StructLCD.SPIPort, (UINT8 *)&StructLCD.FrameBuffer[(LineNumber * StructLCD.Width) + ColumnNumber], StructLCD.Depth);

  /* Display On (recover from Display Off mode). */
  gpio_put(StructLCD.GPIOChipSelect, 1);
  LCD_send_command(0x29);

  /* Reset LCD display writable area to full LCD display. */
  LCD_set_window(0, 0, StructLCD.Height - 1, StructLCD.Width - 1);

  return;
}





/* $PAGE */
/* $TITLE=LCD_printf() */
/* ============================================================================================================================================================= *\
                                                                          Printf emulation function.

                                                   NOTE: PixelLineNumber and PixelColumnNumber correspond to 
                                                         the upper left corner where text will begin to be displayed.
                                                         Be aware that the specified character set may have extra blank pixels
                                                         above / under / to the left / to the right of the character to be displayed.
                                                         Consequently, character position on LCD display may seem to be a few pixels off
                                                         from the specified coordinates

                                                   LineNumber      : Pixel line number where printing will begin.
                                                   ColumnNumber    : Pixel column number where printing will begin.
                                                   ForegroundColor : Foreground color to use for the text to be displayed.
                                                   BackgroundColor : Background color to use behind the text to be displayed.
                                                   Font            : A structure pointer to the character set to be used.
                                                   Format          : Parameters of the printf statement (same as usual printf).
    
                                                   NOTE: If ColumnNumber = 999, text will be horizontally centered on the line.
\* ============================================================================================================================================================= */
void LCD_printf(UINT16 LineNumber, UINT16 ColumnNumber, UINT16 ForegroundColor, UINT16 BackgroundColor, sFONT *Font, UCHAR *Format, ...)
{
#ifdef RELEASE_VERSION
  UINT8 FlagLocalDebug = FLAG_OFF;  // must be turned OFF at all time.
#else   // RELEASE_VERSION
  UINT8 FlagLocalDebug = FLAG_OFF;  // may be turned ON for debug purposes.
#endif  // RELEASE_VERSION

  UCHAR Dum1Str[256];

  UINT8 CharCounter;

  va_list argp;


  /* Unfold text to be printed to variable <Dum1Str> */
  va_start(argp, Format);
  vsnprintf(Dum1Str, sizeof(Dum1Str), Format, argp);
  va_end(argp);
 

  if (FlagLocalDebug)
  {
    log_printf(__LINE__, __func__, "Entering LCD_printf(%s)\n", Dum1Str);
    log_printf(__LINE__, __func__, "Line number:              %3u   Column number:           %3u\n", LineNumber,       ColumnNumber);
    log_printf(__LINE__, __func__, "Font height:              %3u   Font width:              %3u\n", Font->Height,     Font->Width);
    log_printf(__LINE__, __func__, "StructLCD.DisplayHeigth: %3u   StructLCD.Width: %3u\n",          StructLCD.Height, StructLCD.Width);
  }


  /* Validation of the pixel line number specified. */
  if (LineNumber > (StructLCD.Height - 1))
  {
    log_printf(__LINE__, __func__, "LineNumber exceeds the LCD display boundaries (%3u VS %3u).\n", LineNumber, (StructLCD.Height - 1));
    return;
  }


  /* If PixelColumn = 999, text will be horizontally centered on the line. */
  if (ColumnNumber == 999)
  {
    if ((strlen(Dum1Str) * Font->Width) > StructLCD.Width)
    {
      /* If the text is too long to fit on a single line, print whatever fits (beginning at column 0). 
         The remaining will be discarded by LCD_set_pixel() function when reaching the rightmost position on the LCD display. */
      ColumnNumber = 0;
    }
    else
    {
      /* The text fits on a single line, center it. */
      ColumnNumber = (UINT16)((StructLCD.Width - (strlen(Dum1Str) * Font->Width)) / 2);
    }
  }

  /* Validation of the pixel column number. */
  if (ColumnNumber > (StructLCD.Width - 1))
  {
    log_printf(__LINE__, __func__, "ColumnNumber exceeds the maximum number of pixel columns (%3u VS %3u).\n", ColumnNumber, (StructLCD.Width - 1));
    return;
  }

  
  /* Print the text specified. */
  CharCounter = 0;
  while (Dum1Str[CharCounter])
  {
    if (FlagLocalDebug)
    {
      log_printf(__LINE__, __func__, "Printing character <%c>.\n", Dum1Str[CharCounter]);
      log_printf(__LINE__, __func__, "PixelLineNumber: %3u   PixelColumnNumber: %3u\n", LineNumber, ColumnNumber);
    }

    LCD_draw_char(LineNumber, ColumnNumber, Dum1Str[CharCounter], Font, ForegroundColor, BackgroundColor);

    /* Point to next character in the string. */
    ++CharCounter;

    /* Determine column start for next character of the string. */
    if (FlagLocalDebug) log_printf(__LINE__, __func__, "Next ColumnNumber set to %3u (font width = %3u).\n", ColumnNumber + Font->Width, Font->Width);

    ColumnNumber += Font->Width;
  }

  if (FlagLocalDebug) log_printf(__LINE__, __func__, "Exiting LCD_printf()\n\n");

  return;
}





/* $PAGE */
/* $TITLE=LCD_read() */
/* ============================================================================================================================================================= *\
                                                                    Read data from LCD display.
\* ============================================================================================================================================================= */
void LCD_read(UINT8 *Parameter, UINT8 Length)
{
  UINT8 Loop1UInt8;

  LCD_send_command(0x04);  // read display ID.
  spi_read_blocking(StructLCD.SPIPort, 0, Parameter, Length);

  for (Loop1UInt8 = 0; Loop1UInt8 < Length; ++Loop1UInt8)  
    log_printf(__LINE__, __func__, "Parameter %u: %u\n", Loop1UInt8, Parameter[Loop1UInt8]);

  return;
}





/* $PAGE */
/* $TITLE=LCD_refresh()) */
/* ============================================================================================================================================================= *\
                                                           Refresh LCD display with current Framebuffer content.
\* ============================================================================================================================================================= */
void LCD_refresh(void)
{
#ifdef RELEASE_VERSION
  UINT8 FlagLocalDebug = FLAG_OFF;  // must be turned OFF at all time.
#else   // RELEASE_VERSION
  UINT8 FlagLocalDebug = FLAG_OFF;  // may be turned ON for debug purposes.
#endif  // RELEASE_VERSION

  UINT32 Loop1UInt32;


  if (FlagLocalDebug) log_printf(__LINE__, __func__, "Entering LCD_refresh(Height: %3u   Width: %3u)\n", StructLCD.PhysicalHeight, StructLCD.PhysicalWidth);

  /* Define LCD display writable area. */
  LCD_set_window(0, 0, StructLCD.Height - 1, StructLCD.Width - 1);

  gpio_put(StructLCD.GPIODataCommand, 1);  // high for data, low for command.
  gpio_put(StructLCD.GPIOChipSelect,  0);

  for (Loop1UInt32 = 0l; Loop1UInt32 < (StructLCD.PhysicalHeight * StructLCD.PhysicalWidth); ++Loop1UInt32)
    spi_write_blocking(StructLCD.SPIPort, (UINT8 *)&StructLCD.FrameBuffer[Loop1UInt32], StructLCD.Depth);

	/* Display On (recover from Display Off mode). */
  gpio_put(StructLCD.GPIOChipSelect, 1);
  LCD_send_command(0x29);

  if (FlagLocalDebug) log_printf(__LINE__, __func__, "Exiting  LCD_refresh()\n\n");

  return;
}





/* $PAGE */
/* $TITLE=LCD_scroll() */
/* ============================================================================================================================================================= *\
                                                                   Scroll part of LCD display.
                                                                   WARNING - WARNING - WARNING
            The function LCD_scroll() works as it should. However, given the serial protocol (I2C) used to transfer the framebuffer to the LCD display,
              the process is *really* slow. Consequently, the function must be used to scroll only small parts of the LCD display (only a few lines),
                                                  or be used where / when a very slow scrolling does not matter.
                                                     NOTE: Only "scroll-up" direction is supported for now.
\* ============================================================================================================================================================= */
void LCD_scroll(UINT8 Direction, UINT16 LineStart, UINT16 ColumnStart, UINT16 LineEnd, UINT16 ColumnEnd, UINT16 ScrollNumber, UINT16 Color)
{
#ifdef RELEASE_VERSION
  UINT8 FlagLocalDebug = FLAG_OFF;  // must be turned OFF at all time.
#else   // RELEASE_VERSION
  UINT8 FlagLocalDebug = FLAG_OFF;  // may be turned ON for debug purposes.
#endif  // RELEASE_VERSION

  UINT16 AddressSource = 0;
  UINT16 AddressTarget = 0;
  UINT16 CurrentLine;
  UINT16 CurrentColumn;
  UINT16 Loop1UInt16;


  if (FlagLocalDebug)
  {
    log_printf(__LINE__, __func__, "Direction: %3u         Depth: %3u   ScrollNumber: %u\n",              Direction, StructLCD.Depth, ScrollNumber);
    log_printf(__LINE__, __func__, "LineStart: %3u   ColumnStart: %3u   LineEnd: %3u   ColumnEnd: %3u\n", LineStart, ColumnStart, LineEnd, ColumnEnd);
  }

  /* Validate line number passed as argument. */
  if ((LineStart > (StructLCD.Height - 1)) || (LineEnd > (StructLCD.Height - 1)))
  {
    log_printf(__LINE__, __func__, "Line coordinate exceeds display boundaries (LineStart: %3u or LineEnd: %3u VS StructLCD.Height - 1: %3u).\n", LineStart, LineEnd, (StructLCD.Height - 1));
    return;
  }      

  /* Validate column number passed as argument. */
  if ((ColumnStart > (StructLCD.Width - 1)) || (ColumnEnd > (StructLCD.Width - 1)))
  {
    log_printf(__LINE__, __func__, "Column coordinate exceeds display boundaries (ColumnStart: %3u or ColumnEnd: %3u VS StructLCD.Width - 1: %3u).\n", ColumnStart, ColumnEnd, (StructLCD.Width - 1));
    return;
  }      


  /* Number of times to scroll may be different than number of lines to scroll. Last line to be scrolled will be filled by specified color. */
  for (Loop1UInt16 = 0; Loop1UInt16 < ScrollNumber; ++Loop1UInt16)
  {
    for (CurrentLine = LineStart - Loop1UInt16; (CurrentLine < LineEnd) & (CurrentLine > 0); ++CurrentLine)
    {
      for (CurrentColumn = ColumnStart; CurrentColumn < ColumnEnd; ++CurrentColumn)
      {
        AddressSource = (CurrentLine * StructLCD.Width) + CurrentColumn;
        AddressTarget = ((CurrentLine - 1) * StructLCD.Width) + CurrentColumn;
        StructLCD.FrameBuffer[AddressTarget] = StructLCD.FrameBuffer[AddressSource];
        StructLCD.FrameBuffer[AddressSource] = Color;  // fill empty line that has been scrolled up with specified color.
      }
    }
    LCD_refresh();  // should be replace with a <LCD_part_refresh()> to speed process up.
  }

  return;
}





/* $PAGE */
/* $TITLE=LCD_send_command() */
/* ============================================================================================================================================================= *\
                                                         Send a command to LCD display through SPI protocol.
\* ============================================================================================================================================================= */
void LCD_send_command(UINT8 Command)
{
#ifdef RELEASE_VERSION
  UINT8 FlagLocalDebug = FLAG_OFF;  // must be turned OFF at all time.
#else   // RELEASE_VERSION
  UINT8 FlagLocalDebug = FLAG_OFF;  // may be turned ON for debug purposes.
#endif  // RELEASE_VERSION

  if (FlagLocalDebug) log_printf(__LINE__, __func__, "Entering LCD_send_command(%u)\n", Command);

  gpio_put(StructLCD.GPIODataCommand, 0);  // high for data and low for command.
  gpio_put(StructLCD.GPIOChipSelect, 0);   // chip select active low.
  spi_write_blocking(StructLCD.SPIPort, &Command, 1);
  gpio_put(StructLCD.GPIOChipSelect, 1);   // disable chip select when done.

  if (FlagLocalDebug) log_printf(__LINE__, __func__, "Exiting  LCD_send_command()\n\n");

  return;
}





/* $PAGE */
/* $TITLE=LCD_send_data_8bit() */
/* ============================================================================================================================================================= *\
                                                     Send 8 bits of data to LCD data port through SPI protocol.
\* ============================================================================================================================================================= */
void LCD_send_data_8bit(UINT8 Data)
{
#ifdef RELEASE_VERSION
  UINT8 FlagLocalDebug = FLAG_OFF;  // must be turned OFF at all time.
#else   // RELEASE_VERSION
  UINT8 FlagLocalDebug = FLAG_OFF;  // may be turned ON for debug purposes.
#endif  // RELEASE_VERSION

  if (FlagLocalDebug) log_printf(__LINE__, __func__, "Entering LCD_send_data_8bit(%u - 0x%2.2X)\n", Data, Data);

  gpio_put(StructLCD.GPIODataCommand, 1);           // high for data and low for command.
  gpio_put(StructLCD.GPIOChipSelect, 0);            // chip select active low.
  spi_write_blocking(StructLCD.SPIPort, &Data, 1);  // send data.
  gpio_put(StructLCD.GPIOChipSelect, 1);            // disable chip select when done.

  if (FlagLocalDebug) log_printf(__LINE__, __func__, "Exiting  LCD_send_data_8bit()\n\n");

  return;
}





/* $PAGE */
/* $TITLE=LCD_send_data_16bit() */
/* ============================================================================================================================================================ *\
                                                       Send 16 bits of data to LCD data port through SPI protocol.
\* ============================================================================================================================================================= */
void LCD_send_data_16bit(UINT16 Data)
{
#ifdef RELEASE_VERSION
  UINT8 FlagLocalDebug = FLAG_OFF;  // must be turned OFF at all time.
#else   // RELEASE_VERSION
  UINT8 FlagLocalDebug = FLAG_OFF;  // may be turned ON for debug purposes.
#endif  // RELEASE_VERSION

  UINT8 Dum1UInt8;


  if (FlagLocalDebug) log_printf(__LINE__, __func__, "Entering LCD_send_data_16bit(%u - 0x%4.4X)\n", Data, Data);

  gpio_put(StructLCD.GPIODataCommand, 1);  // 1 = Data
  gpio_put(StructLCD.GPIOChipSelect, 0);   // Chip Select: active low.
  
  Dum1UInt8 = (UINT8)((Data >> 8) & 0xFF);  // send highest 8 bits.
  spi_write_blocking(StructLCD.SPIPort, &Dum1UInt8, 1);

  Dum1UInt8 = (UINT8)(Data & 0xFF);  // send lowest 8 bits.
  spi_write_blocking(StructLCD.SPIPort, &Dum1UInt8, 1);

  gpio_put(StructLCD.GPIOChipSelect, 1);

  if (FlagLocalDebug) log_printf(__LINE__, __func__, "Exiting  LCD_send_data_16bit()\n\n");

  return;
}





/* $PAGE */
/* $TITLE=LCD_set_brightness() */
/* ============================================================================================================================================================= *\
                                                                        Set the LCD display brightness.
\* ============================================================================================================================================================= */
void LCD_set_brightness(UINT8 PercentValue)
{
#ifdef RELEASE_VERSION
  UINT8 FlagLocalDebug = FLAG_OFF;  // must be turned OFF at all time.
#else   // RELEASE_VERSION
  UINT8 FlagLocalDebug = FLAG_OFF;  // may be turned ON for debug purposes.
#endif  // RELEASE_VERSION


  if (FlagLocalDebug) log_printf(__LINE__, __func__, "Entering LCD_set_brightness()\n");

  if (StructLCD.DeviceType == LCD_EXPLORER) return;  // Pico Explorer Base has no LCD display broghtness control.

  if (PercentValue > 100)
  {
    log_printf(__LINE__, __func__, "Error - Brightness parameter must represent percentage for brightness (from 0 to 100).\n");
    PercentValue = 100;  // default to maximum brightness.
  }
  
  pwm_set_chan_level(SliceNumber, PWM_CHAN_B, PercentValue);
  StructLCD.Brightness = PercentValue;

  if (FlagLocalDebug) log_printf(__LINE__, __func__, "Exiting  LCD_set_brightness()\n\n");

  return;
}





/* $PAGE */
/* $TITLE=LCD_set_pixel() */
/* ============================================================================================================================================================= *\
                                                                                  Draw pixels
                                                              PixelLineNumber   : Pixel line where to set pixel.
                                                              PixelColumnNumber : Pixel column where to set pixel.
                                                              Color             : Color to set pixel.
\* ============================================================================================================================================================= */
void LCD_set_pixel(UINT16 LineNumber, UINT16 ColumnNumber, UINT16 Color)
{
#ifdef RELEASE_VERSION
  UINT8 FlagLocalDebug = FLAG_OFF;  // must be turned OFF at all time.
#else   // RELEASE_VERSION
  UINT8 FlagLocalDebug = FLAG_OFF;  // may be turned ON for debug purposes.
#endif  // RELEASE_VERSION

  UINT32 Address;


  /* Validate line number passed as argument. */
  if (FlagLocalDebug && (LineNumber > (StructLCD.Height - 1)))
  {
    log_printf(__LINE__, __func__, "Line coordinates exceed display boundaries (LineNumber: %3u VS StructLCD.Height - 1: %3u).\n", LineNumber, (StructLCD.Height - 1));
    return;
  }      

  /* Validate column number passed as argument. */
  if (FlagLocalDebug && (ColumnNumber > (StructLCD.Width - 1)))
  {
    log_printf(__LINE__, __func__, "Column coordinates exceed display boundaries (ColumnNumber: %3u VS StructLCD.Width - 1: %3u).\n", ColumnNumber, (StructLCD.Width - 1));
    return;
  }      


  /* RGB-565 = 65k colors. */
  Address = (LineNumber * StructLCD.Width) + ColumnNumber;
  if (FlagLocalDebug) log_printf(__LINE__, __func__, "Depth: %2u     Address: %5u     Color: 0x%4.4X\n", StructLCD.Depth, Address, Color);
  StructLCD.FrameBuffer[Address] = Color;

  return;
}





/* $PAGE */
/* $TITLE=LCD_set_rotation()) */
/* ============================================================================================================================================================= *\
                                                    Rotate image by specified number of degrees (0, 90, 180, 270).
                               This allows the LCD display to be used in any orientation to easily adapt to physical constraints.
   NOTES: 0-degree correspond to "native mode" (USB connector to the bottom for Waveshare's 2-inches Pico LCD display) or LCD display to the right for Pimoroni's
          Changing from 0-degree to 90-degrees means one quarter of a turn in clockwise direction.
          Changing from 90-degrees to 180 degrees is the same (one more quarter turn), and so on and so forth.
          Mirroring is reset to NONE when rotation is set.
\* ============================================================================================================================================================= */
void LCD_set_rotation(UINT16 Rotation)
{
#ifdef RELEASE_VERSION
  UINT8 FlagLocalDebug = FLAG_OFF;  // must be turned OFF at all time.
#else   // RELEASE_VERSION
  UINT8 FlagLocalDebug = FLAG_OFF;  // may be turned ON for debug purposes.
#endif  // RELEASE_VERSION

  UINT8 MemoryAccessType;


  if (FlagLocalDebug) log_printf(__LINE__, __func__, "Entering LCD_set_rotation(%u)\n", Rotation);

  /* Set LCD rotation angle as requested. */
  switch (Rotation)
  {
    case (ROTATE_0):
      StructLCD.Rotation = ROTATE_0;
      StructLCD.Height   = StructLCD.PhysicalHeight;
      StructLCD.Width    = StructLCD.PhysicalWidth;
      MemoryAccessType = 0x00;  // Set Memory Data Access control mode. Mirroring setting is reset to NONE.
    break;

    case (ROTATE_90):
      StructLCD.Rotation = ROTATE_90;
      StructLCD.Height   = StructLCD.PhysicalWidth;
      StructLCD.Width    = StructLCD.PhysicalHeight;
      MemoryAccessType = 0xA0;  // Set Memory Data Access control mode. Mirroring setting is reset to NONE.
    break;

    case (ROTATE_180):
      StructLCD.Rotation = ROTATE_180;
      StructLCD.Height   = StructLCD.PhysicalHeight;
      StructLCD.Width    = StructLCD.PhysicalWidth;
      MemoryAccessType = 0xC0;  // Set Memory Data Access control mode. Mirroring setting is reset to NONE.
    break;

    case (ROTATE_270):
      StructLCD.Rotation = ROTATE_270;
      StructLCD.Height   = StructLCD.PhysicalWidth;
      StructLCD.Width    = StructLCD.PhysicalHeight;
      MemoryAccessType = 0x60;  // Set Memory Data Access control mode. Mirroring setting is reset to NONE.
    break;

    default:
      log_printf(__LINE__, __func__, "Invalid rotation mode specified: %u\n", Rotation);
      log_printf(__LINE__, __func__, "Must be 0, 90, 180 or 270.\n");
      return;
    break;
  }

  /* Transfer command. */
  LCD_send_command(0x2C);

  /* Initialize Memory Data Access control mode. */
  LCD_send_command(0x36);
  LCD_send_data_8bit(MemoryAccessType);

  /* Define LCD display writable area. */
  LCD_set_window(0, 0, StructLCD.Height - 1, StructLCD.Width - 1);

  if (FlagLocalDebug)
  {
    log_printf(__LINE__, __func__, "Image rotation has been set to %d-degree (MemoryAccessType: 0x%2.2X).\n", Rotation, MemoryAccessType);
    log_printf(__LINE__, __func__, "Exiting LCD_set_rotation()\n");
  }

  return;
}





/* $PAGE */
/* $TITLE=LCD_set_window() */
/* ============================================================================================================================================================= *\
                                                                Define the writable area of LCD display.
   NOTE: LCD_set_window() will define the writable area while accounting for current rotation mode. So, for example, defining a writable area
         from (0,0) up to (50,50) will target a different area of LCD display, depending in which rotation mode we are.
\* ============================================================================================================================================================= */
void LCD_set_window(UINT16 LineStart, UINT16 ColumnStart, UINT16 LineEnd, UINT16 ColumnEnd)
{
#ifdef RELEASE_VERSION
  UINT8 FlagLocalDebug = FLAG_OFF;  // must be turned OFF at all time.
#else   // RELEASE_VERSION
  UINT8 FlagLocalDebug = FLAG_OFF;  // may be turned ON for debug purposes.
#endif  // RELEASE_VERSION


  if (FlagLocalDebug)
  {
    log_printf(__LINE__, __func__, "Entering LCD_set_window() from (%u,%u) up to (%u,%u)\n", LineStart, ColumnStart, LineEnd, ColumnEnd);
    log_printf(__LINE__, __func__, "Current rotation mode set to: %u-degree\n", StructLCD.Rotation);
  }

#if 0  //
  /* Re-order LineStart and LineEnd if required. */
  /* Should not be required since the caller should make sure coordinates are adequate. */
  if (LineEnd < LineStart)
  {
    Dum1UInt16 = LineStart;
    LineStart  = LineEnd;
    LineEnd    = Dum1UInt16;
  }

  /* Re-order ColumnStart and ColumnEnd if required. */
  if (ColumnEnd < ColumnStart)
  {
    Dum1UInt16  = ColumnStart;
    ColumnStart = ColumnEnd;
    ColumnEnd   = Dum1UInt16;
  }
#endif  //

  /* Set the Column coordinates. */
  LCD_send_command(0x2A);
  LCD_send_data_16bit(ColumnStart);
  LCD_send_data_16bit(ColumnEnd);
    
  /* Set the Line coordinates. */
  LCD_send_command(0x2B);
  LCD_send_data_16bit(LineStart);
  LCD_send_data_16bit(LineEnd);

  /* Transfer data to FrameBuffer. */
  LCD_send_command(0X2C);

  if (FlagLocalDebug) log_printf(__LINE__, __func__, "Exiting LCD_set_window()\n\n");

  return;
}





/* $PAGE */
/* $TITLE=LCD_soft_reset() */
/* ============================================================================================================================================================= *\
                                                                   Perform a software reset on LCD display.
\* ============================================================================================================================================================= */
void LCD_soft_reset(void)
{
  /* Send software reset command. */
  LCD_send_command(0x01);

  /* Let the time for LCD display to reload reset default values (must be minimum 5 msec). */
  sleep_ms(10);

  return;
}





/* $PAGE */
/* $TITLE=LCD_splash_image() */
/* ============================================================================================================================================================= *\
                      Transfer part of the Framebuffer (or another memory location whose pointer is passed as a parameter) to the LCD display.

     NOTES: For 65k colors, the color depth must be 2 bytes per pixel (RGB-565) for a total of 240 X 320 X 2 = 154K for a single full-LCD display image.
            Given the size of RAM in the Pico (4 banks of 64kBytes usable - plus 2 stack banks for both cores), even a single (full LCD display) image takes
            a considerable amount of available RAM space. To overcome this limitation, an SD card peripheral could be attached to the Pico with a basic
            filesystem to allow transfering image data directly from an SD card to the integrated LCD display framebuffer, completely by-passing the
            Pico's RAM framebuffer. Doing this could potentially allow using the full color depth of the LCD display (18 bits per pixel) for a complete
            262k color definition. At the time of this writing (NOV-2024), I haven't implemented a SD card peripheral yet. However, an image pointer from
            flash may also be passed to the function. In this case, image is transferred directly from flash to LCD display. Since flash is 2MBytes, it
            gives some chance for larger images if we don't have an extra flash to read from.
\* ============================================================================================================================================================= */
void LCD_splash_image(UINT16 LineStart, UINT16 ColumnStart, UINT16 LineEnd, UINT16 ColumnEnd, UINT16 *Image)
{
#ifdef RELEASE_VERSION
  UINT8 FlagLocalDebug = FLAG_OFF;  // must be turned OFF at all time.
#else   // RELEASE_VERSION
  UINT8 FlagLocalDebug = FLAG_OFF;  // may be turned ON for debug purposes.
#endif  // RELEASE_VERSION

  UINT16 PixelWord;

  UINT32 Loop1UInt32;
  UINT32 NbPixels;


  NbPixels = (LineEnd - LineStart + 1) * (ColumnEnd - ColumnStart + 1);

  if (FlagLocalDebug)
  {
    log_printf(__LINE__, __func__, "Entering LCD_splash_image(%u, %u, %u, %u, 0x%8.8X)\n", LineStart, ColumnStart, LineEnd, ColumnEnd, Image);
    log_printf(__LINE__, __func__, "LineStart: %u   LineEnd: %u   ColumnStart: %u   ColumnEnd: %u\n", LineStart, LineEnd, ColumnStart, ColumnEnd);
    log_printf(__LINE__, __func__, "LineEnd - LineStart: %u     ColumnEnd - ColumnStart: %u\n", LineEnd - LineStart, ColumnEnd - ColumnStart);
    log_printf(__LINE__, __func__, "NbPixels: %lu\n", NbPixels);
  }

  /* Define LCD display writable area. */
  LCD_set_window(LineStart, ColumnStart, LineEnd, ColumnEnd);

  gpio_put(StructLCD.GPIODataCommand, 1);  // set command mode.
  gpio_put(StructLCD.GPIOChipSelect,  0);  // enable chip select active low.

  for (Loop1UInt32 = 0l; Loop1UInt32 < NbPixels; ++Loop1UInt32)
  {
    PixelWord = util_reverse_word(Image[Loop1UInt32]);
    spi_write_blocking(StructLCD.SPIPort, (UINT8 *)&PixelWord, StructLCD.Depth);
  }

  gpio_put(StructLCD.GPIOChipSelect, 1);  // disable chip select when done.

  /* Reset LCD display writable area to full LCD display. */
  LCD_set_window(0, 0, StructLCD.Height - 1, StructLCD.Width - 1);

  if (FlagLocalDebug) log_printf(__LINE__, __func__, "Exiting  LCD_splash_image()\n");

  return;
 }





/* $PAGE */
/* $TITLE=LCD_test() */
/* ============================================================================================================================================================= *\
                                                                   Perform a software reset on LCD display.
                                                        IMPORTANT: You're on your own when playing with the test area !!
\* ============================================================================================================================================================= */
void LCD_test(UINT8 DoW, UINT8 Day, UINT8 Month, UINT16 Year, UINT8 Hour, UINT8 Minute, UINT8 Second)
{
  UCHAR String[32];

  UINT16 Loop1UInt16;
  UINT16 LineNumber;
  UINT16 ColumnNumber;
  UINT16 LineStart;
  UINT16 LineEnd;
  UINT16 ColumnStart;
  UINT16 ColumnEnd;


  LineStart   = 0;
  LineEnd     = 100;
  ColumnStart = 1;
  ColumnEnd   = 238;
  log_printf(__LINE__, __func__, "Number of bytes: %u X %u = %u\n", (LineEnd - LineStart), (ColumnEnd - ColumnStart), ((LineEnd - LineStart) * (ColumnEnd - ColumnStart)));
  log_printf(__LINE__, __func__, "Press <Enter> to LCD_clear(BLACK) (%u, %u) to (%u, %u): ", LineStart, ColumnStart, LineEnd, ColumnEnd);
  input_string(String, sizeof(String), 0l);
  printf("\n\n");

  /* Define LCD display writable area. */
  LCD_set_window(0, 0, StructLCD.Height - 1, StructLCD.Width - 1);
  LCD_clear(BLACK);

  /* Define LCD display writable area. */
  LCD_set_window(LineStart, ColumnStart, LineEnd, ColumnEnd);

  gpio_put(StructLCD.GPIODataCommand, 1);  // high for data, low for command.
  gpio_put(StructLCD.GPIOChipSelect,  0);

  for (LineNumber = LineStart; LineNumber < LineEnd; ++LineNumber)
  {
    for (ColumnNumber = ColumnStart; ColumnNumber < ColumnEnd; ++ColumnNumber)
    {
      spi_write_blocking(StructLCD.SPIPort, (UINT8 *)&StructLCD.FrameBuffer[ColumnNumber + (LineNumber * StructLCD.Width) - 1], StructLCD.Depth);
    }
  }

	/* Display On (recover from Display Off mode). */
  gpio_put(StructLCD.GPIOChipSelect, 1);
  LCD_send_command(0x29);

  



  LineStart   = 20;
  LineEnd     = 50;
  ColumnStart = 0;
  ColumnEnd   = 239;
  log_printf(__LINE__, __func__, "Number of bytes: %u\n", ((LineEnd - LineStart) * (ColumnEnd - ColumnStart)));
  log_printf(__LINE__, __func__, "Press <Enter> to LCD_clear(YELLOW) (%u, %u) to (%u, %u): ", LineStart, ColumnStart, LineEnd, ColumnEnd);
  input_string(String, sizeof(String), 0l);
  printf("\n\n");

  /* Define LCD display writable area. */
  LCD_set_window(0, 0, StructLCD.Height - 1, StructLCD.Width - 1);
  LCD_clear(YELLOW);

  /* Define LCD display writable area. */
  LCD_set_window(LineStart, ColumnStart, LineEnd, ColumnEnd);

  gpio_put(StructLCD.GPIODataCommand, 1);  // high for data, low for command.
  gpio_put(StructLCD.GPIOChipSelect,  0);

  for (LineNumber = LineStart; LineNumber < LineEnd; ++LineNumber)
  {
    for (ColumnNumber = ColumnStart; ColumnNumber < ColumnEnd; ++ColumnNumber)
    {
      spi_write_blocking(StructLCD.SPIPort, (UINT8 *)&StructLCD.FrameBuffer[ColumnNumber + (LineNumber * StructLCD.Width) - 1], StructLCD.Depth);
    }
  }

	/* Display On (recover from Display Off mode). */
  gpio_put(StructLCD.GPIOChipSelect, 1);
  LCD_send_command(0x29);





  LineStart   = 80;
  LineEnd     = 110;
  ColumnStart = 10;
  ColumnEnd   = 229;
  log_printf(__LINE__, __func__, "Number of bytes: %u\n", ((LineEnd - LineStart) * (ColumnEnd - ColumnStart)));
  log_printf(__LINE__, __func__, "Press <Enter> to LCD_clear(BLUE) (%u, %u) to (%u, %u): ", LineStart, ColumnStart, LineEnd, ColumnEnd);
  input_string(String, sizeof(String), 0l);
  printf("\n\n");

  /* Define LCD display writable area. */
  LCD_set_window(0, 0, StructLCD.Height - 1, StructLCD.Width - 1);
  LCD_clear(BLUE);

  /* Define LCD display writable area. */
  LCD_set_window(LineStart, ColumnStart, LineEnd, ColumnEnd);

  gpio_put(StructLCD.GPIODataCommand, 1);  // high for data, low for command.
  gpio_put(StructLCD.GPIOChipSelect,  0);

  for (LineNumber = LineStart; LineNumber < LineEnd; ++LineNumber)
  {
    for (ColumnNumber = ColumnStart; ColumnNumber < ColumnEnd; ++ColumnNumber)
    {
      spi_write_blocking(StructLCD.SPIPort, (UINT8 *)&StructLCD.FrameBuffer[ColumnNumber + (LineNumber * StructLCD.Width) - 1], StructLCD.Depth);
    }
  }

	/* Display On (recover from Display Off mode). */
  gpio_put(StructLCD.GPIOChipSelect, 1);
  LCD_send_command(0x29);





  LineStart   = 150;
  LineEnd     = 200;
  ColumnStart = 100;
  ColumnEnd   = 149;
  log_printf(__LINE__, __func__, "Number of bytes: %u\n", ((LineEnd - LineStart) * (ColumnEnd - ColumnStart)));
  log_printf(__LINE__, __func__, "Press <Enter> to LCD_clear(RED) (%u, %u) to (%u, %u): ", LineStart, ColumnStart, LineEnd, ColumnEnd);
  input_string(String, sizeof(String), 0l);
  printf("\n\n");

  /* Define LCD display writable area. */
  LCD_set_window(0, 0, StructLCD.Height - 1, StructLCD.Width - 1);
  LCD_clear(RED);

  /* Define LCD display writable area. */
  LCD_set_window(LineStart, ColumnStart, LineEnd, ColumnEnd);

  gpio_put(StructLCD.GPIODataCommand, 1);  // high for data, low for command.
  gpio_put(StructLCD.GPIOChipSelect,  0);

  for (LineNumber = LineStart; LineNumber < LineEnd; ++LineNumber)
  {
    for (ColumnNumber = ColumnStart; ColumnNumber < ColumnEnd; ++ColumnNumber)
    {
      spi_write_blocking(StructLCD.SPIPort, (UINT8 *)&StructLCD.FrameBuffer[ColumnNumber + (LineNumber * StructLCD.Width) - 1], StructLCD.Depth);
    }
  }

	/* Display On (recover from Display Off mode). */
  gpio_put(StructLCD.GPIOChipSelect, 1);
  LCD_send_command(0x29);





  LineStart   =  20;
  LineEnd     = 100;
  ColumnStart =  12;
  ColumnEnd   = 227;

  for (Loop1UInt16 = 0; Loop1UInt16 < 10; ++Loop1UInt16)
  {
    switch (Loop1UInt16)
    {
      case (0):
      case (2):
      case (4):
      case (6):
      case (8):
        /* Define LCD display writable area. */
        LCD_set_window(0, 0, StructLCD.Height - 1, StructLCD.Width - 1);
        LCD_clear(WHITE);
        LCD_printf(20, 999, BLACK, WHITE, &Font16, "%s", DayName[DoW]);
        LCD_printf(40, 999, BLACK, WHITE, &Font16, "%2u-%s-%4.4u", Day, ShortMonth[Month], Year);
        LCD_printf(60, 999, BLACK, WHITE, &Font16, "%2u:%2.2u:%2.2u", Hour, Minute, Second);
        /* Define LCD display writable area. */
        LCD_set_window(LineStart, ColumnStart, LineEnd, ColumnEnd);
      break;

      case (1):
      case (3):
      case (5):
      case (7):
      case (9):
      default:
        /* Define LCD display writable area. */
        LCD_set_window(0, 0, StructLCD.Height - 1, StructLCD.Width - 1);
        LCD_clear(WHITE);
        LCD_printf(20, 999, BLACK, WHITE, &Font16, "ABCDEFGHIJKLMN");
        LCD_printf(40, 999, BLACK, WHITE, &Font16, "1234567890");
        LCD_printf(60, 999, BLACK, WHITE, &Font16, "11223344556677889900");
        /* Define LCD display writable area. */
        LCD_set_window(LineStart, ColumnStart, LineEnd, ColumnEnd);
      break;
    }

    log_printf(__LINE__, __func__, "Number of bytes: %u   (pass: %u)\n", ((LineEnd - LineStart) * (ColumnEnd - ColumnStart)), Loop1UInt16 + 1);
    log_printf(__LINE__, __func__, "Press <Enter> to refresh date and time: ");
    input_string(String, sizeof(String), 0l);
    printf("\n\n");
  }

  /* Define LCD display writable area. */
  LCD_set_window(0, 0, StructLCD.Height - 1, StructLCD.Width - 1);

  return;
}





/* $PAGE */
/* $TITLE=LCD_translate_coord() */
/* ============================================================================================================================================================= *\
                      Translate specified coordinates in current rotation mode to equivalent coordinates in native mode (rotation 0-degree).
\* ============================================================================================================================================================= */
void LCD_translate_coord(UINT16 *Line, UINT16 *Column)
{
  UINT16 TempLine;


  switch (StructLCD.Rotation)
  {
    case (0):
      /* Already in native mode, nothing to do. */
    break;

    case (90):
      TempLine = *Line;  // keep track of specified line number.
      *Line   = StructLCD.PhysicalHeight - 1 - *Column;
      *Column = TempLine;
    break;

    case (180):
      *Line   = StructLCD.Height - 1 - *Line;
      *Column = StructLCD.Width  - 1 - *Column;
    break;

    case (270):
      TempLine = *Line;  // keep track of specified line number.
      *Line   = StructLCD.PhysicalHeight - 1 - *Column;
      *Column = StructLCD.PhysicalWidth  - 1 - TempLine;
    break;

    return;
  }
}





/* $PAGE */
/* $TITLE=util_reverse_word() */
/* ============================================================================================================================================================= *\
                                                               Reverse byte order in a given word.
\* ============================================================================================================================================================= */
UINT16 util_reverse_word(UINT16 Word)
{
  UINT16 PixelWord;

  PixelWord = ((Word << 8) & 0xFF00) | (Word >> 8);

  return PixelWord;
}
