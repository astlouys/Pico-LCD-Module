/* ============================================================================================================================================================= *\
   Pico-LCD-Example.c
   St-Louys Andre - Juin 2025
   astlouys@gmail.com
   https://github.com/astlouys/Pico-LCD-Module
   Revision 17-JUN-2026
   Langage: C-Language
   Version 3.02

   ==========================================================================
   Pico-LCD-Example is compatible with the SLASH Smart Home ecosystem family.
   ==========================================================================

   REVISION HISTORY:
   =================
   20-JAN-2021 1.00 - Original source code from Waveshare.
   03-JUN-2023 2.00 - Initial release modified by Andre St-Louys.
   28-FEB-2025 3.00 - Add support for Pimoroni's Pico Explorer.
   12-JUN-2025 3.01 - Modify / adjust code to make it compliant with other Pico-Xxx-Module library members.
                      Rename function log_info() to log_printf().
                      Use common functions get_pico_identifier(), input_string(), log_printf().
   14-MAY-2026 3.02 - Major update, debug and optimizing session to comply with last function standards.
\* ============================================================================================================================================================= */


/* ============================================================================================================================================================= *\
                                Demo program for Waveshare's 2-inch and Pimoroni's Pico Explorer Base RGB color Pico's LCD display.
                                         (could probably be adapted easily for Waveshare's 1.14-inch LCD display as well).
                                            Copyright notice reproduced from Waveshare's original Firmware Version 1.00:

   Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documnetation files (the "Software"), to deal
     in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
                  copies of the Software, and to permit persons to  whom the Software is furished to do so, subject to the following conditions:

                  The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.

      THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
      FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
                    LIABILITY WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE
                                                           OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.

\* ============================================================================================================================================================= */



/* $PAGE */
/* $TITLE=List of GPIO usage. */
/* ============================================================================================================================================================= *\
                                                                     List of GPIO usage and pin-out.
\* ============================================================================================================================================================= */
/* ------------------------------------------------------------------------------------------------------------------------------------------------------------- *\
    Using Waveshare's 2-inch LCD display and Waveshare's BME280 module.
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
    Pin 11 - GPIO  8  - LCD_DATA_COMMAND - data / command control pin  (on Waveshare's 2-inch LCD display).
    Pin 12 - GPIO  9  - LCD_CHIP_SELECT  - chip select pin, active low (on Waveshare's 2-inch LCD display).
    Pin 13 - GND      - Ground
    Pin 14 - GPIO 10  - LCD_CLOCK        - SPI clock pin               (on Waveshare's 2-inch LCD display).
    Pin 15 - GPIO 11  - LCD_DATA         - SPI  data pin               (on Waveshare's 2-inch LCD display).
    Pin 16 - GPIO 12  - LCD_RESET        - reset pin, active low       (on Waveshare's 2-inch LCD display).
    Pin 17 - GPIO 13  - LCD_BRIGHTNESS   - backlight pwm pin           (on Waveshare's 2-inch LCD display).
    Pin 18 - GND      - Ground
    Pin 19 - GPIO 14  - Reserved for Pico-ASTL-SoundServer status pin.
    Pin 20 - GPIO 15  - Button 0 (on Waveshare's 2-inch LCD display).
    Pin 21 - GPIO 16  - Reserved for infrared sensor / receiver (VS1838).
    Pin 22 - GPIO 17  - Button 1 (on Waveshare's 2-inch LCD display).
    Pin 23 - GND      - Ground
    Pin 24 - GPIO 18  - Reserved for indicator LED.
    Pin 25 - GPIO 19  - Reserved for tri-color LED (RED).
    Pin 26 - GPIO 20  - Reserved for tri-color LED (GREEN).
    Pin 27 - GPIO 21  - Reserved for tri-color LED (BLUE).
    Pin 28 - GND      - Ground
    Pin 29 - GPIO 22  - Reserved for Active or Passive buzzer.
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

    Test point 1 - Ground.
    Test point 2 - USB DM  - USB data negative pin.
    Test point 3 - USB DP  - USB data positive pin.
    Test point 4 - GPIO 23 - Switched Mode Power Supply (SMPS) Pin. Controls the on-board SMPS power save pin.
    Test point 5 - GPIO 25 - Pico's LED.
    Test point 6 - Bootsel (upload mode).
\* ------------------------------------------------------------------------------------------------------------------------------------------------------------- */


/* ------------------------------------------------------------------------------------------------------------------------------------------------------------- *\
    For Pimoroni's Pico Explorer
    GPIO  0 - Unused.
    GPIO  1 - Unused.
    GPIO  2 - Unused.
    GPIO  3 - Unused.
    GPIO  4 - Unused.
    GPIO  5 - Unused.
    GPIO  6 - Unused.
    GPIO  7 - Unused.
    GPIO  8 - Motor #1 negative.
    GPIO  9 - Motor #1 positive.
    GPIO 10 - Motor #2 negative.
    GPIO 11 - Motor #2 positive.
    GPIO 12 - Key <A>
    GPIO 13 - Key <B>
    GPIO 14 - Key <X>
    GPIO 15 - Key <Y>
    GPIO 16 - SPI MISO
    GPIO 17 - LCD CS
    GPIO 18 - SPI SCK
    GPIO 19 - SPI MOSI
    GPIO 20 - I2C SDA
    GPIO 21 - I2C SCL
    GPIO 22 - I2C INT
    GPIO 23 - Unused.
    GPIO 24 - Unused.
    GPIO 25 - Pico's LED.
    GPIO 26 - ADC0
    GPIO 27 - ADC1
    GPIO 28 - ADC2
\* ------------------------------------------------------------------------------------------------------------------------------------------------------------- */





/* $PAGE */
/* $TITLE=Include files. */
/* ============================================================================================================================================================= *\
                                                                            Include files.
\* ============================================================================================================================================================= */
#include "baseline.h"
#include "hardware/rtc.h"
#include "hardware/watchdog.h"
#include "math.h"
#include "pico/bootrom.h"
#include "pico/unique_id.h"
#include "stdio.h"
#include "stdlib.h"
#include "stdarg.h"
#include <string.h>

#include "Pico-LCD-Module.h"

#include "MixedTubes.img"
#include "Nature1.img"
#include "Nature2.img"
#include "PicoLogo.img"
#include "SunFall.img"
#include "WaterFall.img"
#include "Waveshare1.img"
#include "Waveshare2.img"
#include "Waveshare3.img"




/* $PAGE */
/* $TITLE=Definitions. */
/* ============================================================================================================================================================= *\
                                                                               Definitions.
\* ============================================================================================================================================================= */
/* Set type of device used. NOTE: ONLY ONE MUST BE SELECTED. */
// Valid choices for now are:
// LCD_114: Waveshare's 1.14 inch LCD display (may partly work for some functions, but not supported for now).
// LCD_200: Waveshare's 2.00 inches LCD display.
// LCD_EXPLORER: Pimoroni's Pico-Explorer Base module (see User Guide front page for a picture).
// #define LCD_TYPE   LCD_114       // Waveshare's 1.14 inch LCD display (not supported for now).
#define LCD_TYPE   LCD_200          // Waveshare's 2-inch LCD display.
// #define LCD_TYPE   LCD_EXPLORER  // Pimoroni's Pico-Explorer (see front page of User Guide).

#define FIRMWARE_VERSION  "3.02"

#define TYPE_NONE             0
#define TYPE_CONTINUE         1
#define TYPE_CONTINUE_OR_MENU 2
#define TYPE_PROCEED          3

#define MAX_SCREEN_WIDTH  120  // terminal log screen default width.

/* Debug definitions. */
#define DEBUG_MASK_NONE 0x0000000000000000
#define DEBUG_MASK_DEMO 0x0000000000000001



/* $PAGE */
/* $TITLE=Global variables. */
/* ============================================================================================================================================================= *\
                                                                      Global variable
\* ============================================================================================================================================================= */
UCHAR PicoIdentifier[40];                     // device identifier for SLASH ecosystem.
UCHAR PicoUniqueId[25];                       // Pico Unique ID read from flash IC.
UCHAR Separator[201];
UCHAR Separator1[201];

UINT8 FlagAllTests = FLAG_OFF;  // indicates that we are currently in menu option "all tests in sequence".
UINT8 PicoType;

UINT64 DebugBitMask;

extern sFONT Font24;
extern sFONT Font20;
extern sFONT Font16;
extern sFONT Font12;
extern sFONT Font8;

struct struct_lcd StructLCD;

/* Day names. */
UCHAR DayName[7][13] =
{
  {"Sunday"}, {"Monday"}, {"Tuesday"}, {"Wednesday"}, {"Thursday"}, {"Friday"}, {"Saturday"}
};

/* Short month names (3 letters). */
UCHAR ShortMonth[13][4] =
{
  {" "}, {"JAN"}, {"FEB"}, {"MAR"}, {"APR"}, {"MAY"}, {"JUN"}, {"JUL"}, {"AUG"}, {"SEP"}, {"OCT"}, {"NOV"}, {"DEC"}
};



/* $PAGE */
/* $TITLE=Function prototypes. */
/* ============================================================================================================================================================= *\
                                                                        Function prototypes.
\* ============================================================================================================================================================= */
/* Blink Pico's LED according to provided parameters. */
void blink(UINT8 NbOfTimes, UINT16 TimeOn, UINT16 TimeOff);

/* Display footer on LCD display. */
void display_LCD_footer(void);

/* Display header on LCD display. */
void display_LCD_header(void);

/* Display presentation screen on LCD display. */
void display_LCD_presentation(void);

/* Fill LCD display with text. */
void fill_text(UINT8 FontSize);

/* Find the number of pixels in the image buffer, assuming RGB-565 (2 bytes per pixel). */
UINT32 find_number_of_pixels(UCHAR *ImagePtr);

/* Find the specific Pico Identifier string from its Unique ID. */
void get_pico_identifier(UCHAR *PicoUniqueId, UCHAR *PicoIdentifier, UINT8 *PicoType);

/* Function to analyze RGB-565 image size and format. */
void image_analysis(void);

/* Flush stdin buffer. */
void input_flush(void);

/* Read a string from stdin. */
void input_string(UCHAR *String, UINT16 StringSize, UINT32 TimeOutMSec);

/* Print Firmware header to log file. */
void log_header(void);

/* Send string to log file. */
void log_printf(UINT LineNumber, const UCHAR *FunctionName, UCHAR *Format, ...);

/* Time delay function to use when sleep_ms() can't be used (during interrupts or callbacks). */
void no_op(UINT32 MSec);

/* Check if user wants to stop current process flow and return to menu. */
UINT8 stop_flow(UINT8 Type);

/* Terminal menu when a CDC USB connection is detected during power up sequence. */
void term_menu(void);

/* Test function LCD_clear() with different colors. */
void test_lcd_clear(void);

/* Test function LCD_cls() with different colors. */
void test_lcd_cls(void);

/* Test function LCD_display_point(). */
void test_lcd_display_point(void);

/* Test function LCD_draw_box(). */
void test_lcd_draw_box(void);

/* Test function LCD_draw_char(). */
void test_lcd_draw_char(void);

/* Test function LCD_draw_circle(). */
void test_lcd_draw_circle(void);

/* Test function LCD_draw_image(). */
void test_lcd_draw_image(void);

/* Test function LCD_draw_line(). */
void test_lcd_draw_line(void);

/* Test function LCD_draw_point(). */
void test_lcd_draw_point(void);

/* Test function LCD_mirror_framebuffer(). */
void test_lcd_mirror_framebuffer(void);

/* Test function LCD_part_clear(). */
void test_lcd_part_clear(void);

/* Test function LCD_part_cls(). */
void test_lcd_part_cls(void);

/* Test function LCD_part_refresh(). */
void test_lcd_part_refresh(void);

/* Test function LCD_printf(). */
void test_lcd_printf(void);

/* Test function LCD_scroll. */
void test_lcd_scroll(void);

/* Test function LCD_set_brightness(). */
void test_lcd_set_brightness(void);

/* Test function LCD_set_pixel() */
void test_lcd_set_pixel(void);

/* Test function LCD_set_rotation(). */
void test_lcd_set_rotation(void);

/* Test function LCD_splash_image(). */
void test_lcd_splash_image(void);

/* Test to draw lines on LCD display. */
void test_line_draw(void);

/* Perform miscellaneous combined tests. */
void test_miscellaneous(void);

void test_rotation(void);

/* Display data whose pointer is sent in argument to an external monitor. */
void util_display_data(UCHAR *Data, UINT32 DataSize);

/* Test display colors. */
void zone_test1(void);





/* $PAGE */
/* $TITLE=Main program entry point. */
/* ============================================================================================================================================================= *\
                                                                     Main program entry point.
          NOTE: In a "real" (non demo) working environment, LCD_init() should be called as soon as possible to blank the LCD display when power is applied.
\* ============================================================================================================================================================= */
int main(void)
{
  UCHAR  String[256];

  UINT8 Delay;

  UINT16 Loop1UInt16;


  /* ----------------------------------------------------------------------------------------------------------------------------------------------------------- *\
                                                                           Initializations.
  \* ----------------------------------------------------------------------------------------------------------------------------------------------------------- */
  stdio_init_all();

  /* Initialize Pico's LED (this init is for the plain Pico, not compatible with the PicoW). */
  gpio_init(GPIO_PICO_LED);
  gpio_set_dir(GPIO_PICO_LED, GPIO_OUT);

  get_pico_identifier(PicoUniqueId, PicoIdentifier, &PicoType);

  memset(Separator, 0x3D, MAX_SCREEN_WIDTH);   // <=> for screen formatting.
  Separator[MAX_SCREEN_WIDTH - 2] = '\n';      // add line-feed terminator.
  Separator[MAX_SCREEN_WIDTH - 1] = '\0';      // add liend-of-string.

  memset(Separator1, 0x2D, MAX_SCREEN_WIDTH);  // <-> for screen formatting.
  Separator1[MAX_SCREEN_WIDTH - 2] = '\n';     // add line-feed terminator.
  Separator1[MAX_SCREEN_WIDTH - 1] = '\0';     // add end-of-string.

  sleep_ms(500);  // give some time for USB CDC to become responsive.



  /* Give some time to start a terminal emulator program. */
  Delay = 0;
  while (stdio_usb_connected() == 0)
  {
    ++Delay;
    sleep_ms(50);  // 50 msec added to current wait time for a USB CDC connection.

    /* If we waited for more than this number of "50 msec" for a USB CDC connection, get out of the loop and continue. */
    if (Delay > 2400) break;  // 2400 = 2 minutes.
  }

  /* Clear screen. */
  log_printf(__LINE__, __func__, "cls");

  /* "Home" cursor. */
  log_printf(__LINE__, __func__, "home");

  log_header();
  log_printf(__LINE__, __func__, "<120>Main program entry point (Delay: %u msec waiting for USB CDC connection).\n", (Delay * 50));
  log_printf(__LINE__, __func__, "<120>Initializing LCD display in native portrait mode (Rotation: 0-degree / Mirroring: none).\n");
  log_printf(__LINE__, __func__, Separator);



  /* ----------------------------------------------------------------------------------------------------------------------------------------------------------- *\
                                                                       LCD setup
  \ ------------------------------------------------------------------------------------------------------------------------------------------------------------ */
  /* Wipe StructLCD on entry. */
  memset(&StructLCD, 0x00, sizeof(StructLCD));
  StructLCD.DeviceType = LCD_TYPE;

  /* Initialize LCD display. */
  LCD_init(ROTATE_0, WHITE);  // rotation 0-degree, white background color.



  /* ----------------------------------------------------------------------------------------------------------------------------------------------------------- *\
                                                                     Display active debug flags.
  \* ----------------------------------------------------------------------------------------------------------------------------------------------------------- */
  /* If user requested any section to be debugged through USB CDC, indicate it on log screen. */
  if (DebugBitMask)
  {
    log_printf(__LINE__, __func__, Separator);
    log_printf(__LINE__, __func__, "<120>Pico-LCD-Example LOG INFO\n");
    log_printf(__LINE__, __func__, Separator);

    /* Indicate all sections that we are debugging. */
    log_printf(__LINE__, __func__, "<120>Sections below will be logged for debug:\n");
    log_printf(__LINE__, __func__, Separator1);
    for (Loop1UInt16 = 0; Loop1UInt16 < 64; ++Loop1UInt16)
    {
      if (DebugBitMask & (1 << Loop1UInt16))
      {
        switch (1 << Loop1UInt16)
        {
          case DEBUG_MASK_DEMO:
            log_printf(__LINE__, __func__, "-> Main demo debugging\n");
          break;

         default:
            snprintf(String, sizeof(String), __func__, "-> Section #%u\n", Loop1UInt16);
            log_printf(__LINE__, __func__, String);
          break;
        }
      }
    }
    log_printf(__LINE__, __func__, Separator);
  }



  /* ----------------------------------------------------------------------------------------------------------------------------------------------------------- *\
                                                                       Loop on terminal menu.
  \* ----------------------------------------------------------------------------------------------------------------------------------------------------------- */
  while (1)
  {
    term_menu();
  }

  return 0;
}





/* $PAGE */
/* $TITLE=blink() */
/* ============================================================================================================================================================= *\
                                                         Blink Pico's LED according to provided parameters.
                                                 (Valid only on "plain" Pico - does not work the same way on PicoW)
\* ============================================================================================================================================================= */
void blink(UINT8 NbOfTimes, UINT16 TimeOn, UINT16 TimeOff)
{
  UINT8 Loop1UInt8;

  for (Loop1UInt8 = 0; Loop1UInt8 < NbOfTimes; ++Loop1UInt8)
  {
    gpio_put(GPIO_PICO_LED, 1);
    sleep_ms(TimeOn);

    gpio_put(GPIO_PICO_LED, 0);
    sleep_ms(TimeOff);
  }

  return;
}





/* $PAGE */
/* $TITLE=display_LCD_footer() */
/* ============================================================================================================================================================= *\
                                                                   Display footer on LCD display.
\* ============================================================================================================================================================= */
void display_LCD_footer(void)
{
  LCD_part_clear((StructLCD.Height - 25), 0, (StructLCD.Height - 1), (StructLCD.Width - 1), WHITE);
  LCD_draw_line((StructLCD.Height - 25), 0, (StructLCD.Height - 25), (StructLCD.Width - 1), BLACK, DOT_PIXEL_2X2, LINE_STYLE_SOLID);
  LCD_printf((StructLCD.Height - 20), 999, BLACK, WHITE, &Font16, "astlouys@gmail.com");
  LCD_refresh();

  return;
}





/* $PAGE */
/* $TITLE=display_LCD_header() */
/* ============================================================================================================================================================= *\
                                                                   Display header on LCD display.
\* ============================================================================================================================================================= */
void display_LCD_header(void)
{
  UINT16 Spacing;
  UINT16 StrLen;

  StrLen  = (strlen(FIRMWARE_VERSION) * Font16.Width);  // pixel width of Firmware Version
  Spacing = ((StructLCD.Width - (140 + StrLen)) / 3);   // 140 = image width, 3 = number of spaces on the line.

  LCD_part_clear(0, 0, 64, (StructLCD.Width - 1), WHITE);
  LCD_draw_image(10, Spacing, (UINT16 *)PicoLogo, 23, 140);
  LCD_printf(16, (StructLCD.Width - (Spacing + StrLen)), BLACK, WHITE, &Font16, "%s", FIRMWARE_VERSION);
  LCD_draw_line(40, 0, 40, (StructLCD.Width - 1), RED, DOT_PIXEL_2X2, LINE_STYLE_SOLID);
  LCD_printf(45, 999, RED, WHITE, &Font16, "Pico-LCD-Example");
  LCD_draw_line(62, 0, 62, (StructLCD.Width - 1), RED, DOT_PIXEL_2X2, LINE_STYLE_SOLID);

  return;
}





/* $PAGE */
/* $TITLE=display_LCD_presentation() */
/* ============================================================================================================================================================= *\
                                                           Display presentation screen on LCD display.
\* ============================================================================================================================================================= */
void display_LCD_presentation(void)
{
  LCD_clear(WHITE);
  switch (StructLCD.DeviceType)
  {
    case (LCD_114):
    case (LCD_200):
      if ((StructLCD.Rotation == ROTATE_90) || (StructLCD.Rotation == 270))
      {
        LCD_draw_image(0, 0, (UINT16 *)SunFall, 240, 320);
        LCD_refresh();   // update LCD display.
      }
      else
      {
        LCD_draw_image(80, (StructLCD.Width - 240) / 2, (UINT16 *)WaterFall, 240, 240);
        LCD_draw_image(30, (StructLCD.Width - 140) / 2, (UINT16 *)PicoLogo,   23, 140);
        LCD_refresh();   // update LCD display.
      }
      sleep_ms(2000);  // leave display above for a few seconds.

      LCD_clear(WHITE);
      LCD_draw_image(10, (StructLCD.Width - 140) / 2, (UINT16 *)PicoLogo,   23, 140);
      LCD_draw_image(70, (StructLCD.Width - 240) / 2, (UINT16 *)Waveshare1, 88, 240);
    break;

    case (LCD_EXPLORER):
      LCD_draw_image(30, (StructLCD.Width - 140) / 2, (UINT16 *)PicoLogo, 23, 140);
      LCD_refresh();   // update LCD display.
      sleep_ms(3000);  // let Pico logo picture for a few seconds.

      LCD_clear(WHITE);
      LCD_draw_image(0, (StructLCD.Width - 240) / 2, (UINT16 *)WaterFall, 240, 240);
      LCD_refresh();   // update LCD display.
      sleep_ms(3000);  // let waterfall picture for a few seconds.

      LCD_clear(WHITE);
      LCD_printf(50, 999, MAGENTA, WHITE, &Font24, "Pimoroni");
    break;

    default:
      LCD_printf(50, 999, MAGENTA, WHITE, &Font24, "Uninitialized");
    break;
  }

  display_LCD_header();
  display_LCD_footer();
  LCD_refresh();

  return;
}





/* $PAGE */
/* $TITLE=fill_text() */
/* ============================================================================================================================================================= *\
                                                                    Fill LCD display with text. 
\* ============================================================================================================================================================= */
void fill_text(UINT8 FontSize)
{
  UCHAR Character;
  UCHAR Text[256];

  UINT16 LineNumber;
  UINT16 Loop1UInt16;

  sFONT FontType;


  switch (FontSize)
  {
    case (8):
      FontType = Font8;
    break;

    case (12):
      FontType = Font12;
    break;

    case (16):
      FontType = Font16;
    break;

    case (20):
      FontType = Font20;
    break;

    default:
    case (24):
      FontType = Font24;
    break;
  }

  /* Build string to be displayed on a single line. */
  Character = 'A';
  memset(Text, '\0', sizeof(Text));

  for (Loop1UInt16 = 0; (((strlen(Text) + 1) * FontType.Width) < StructLCD.Width + 4); ++Loop1UInt16)
  {
    Text[Loop1UInt16] = Character++;
    if (Character == '[') Character = 'a';  // revert to lowercase letters.
  }

  LCD_clear(WHITE);  // clear LCD display on entry.
  for (LineNumber = 0; LineNumber < (StructLCD.Height - FontType.Height + 1); LineNumber += FontType.Height)
    LCD_printf(LineNumber, 0, BLUE, WHITE, &FontType, Text);
  LCD_refresh();

  return;
}





/* $PAGE */
/* $TITLE=find_number_of_pixels() */
/* ============================================================================================================================================================= *\
                                                          Find the number of elements in image buffer.
            ******* IMPORTANT ******* Four (4) bytes must be added at the end of the image buffer as "end-of-buffer delimiters": 0xAA, 0x55, 0xAA, 0x55.
                              Three (3) optional "end-of-buffer" messages may appear to positively identity the end of image buffer. 
                               The size of the buffer is obtained by substracting 1 from the first "end-of buffer" message address.
\* ============================================================================================================================================================= */
UINT32 find_number_of_pixels(UCHAR *ImagePtr)
{
  UINT8 Counter;

  UINT32 LastAddress;
  UINT32 Loop1UInt32;

  Counter     = 0;
  LastAddress = 0l;

  /* Stop at 4 MByte memory limit since Pico 2 flash is 4 MBytes. Start at position 2 to prevent problems while checking previous image memory locations. */
  for (Loop1UInt32 = 2; Loop1UInt32 < 4000000; ++Loop1UInt32)
  {
    if (Counter == 0)
    {
      LastAddress = Loop1UInt32;  // assume this is the last valid byte of image buffer.

      /* Counter is still 0, we didn't find an image delimiter so far. */
      if (ImagePtr[Loop1UInt32] == 0xAA)
      {
        /* We found a 0xAA, it could be the first footprint of the image buffer delimiter. */
        // log_printf(__LINE__, __func__, "End-of-buffer 0xAA found at %7lu   (%u)\n", Loop1UInt32, Counter);  // optional message to user.
        ++Counter;
      }
    }
    else
    {
      /* Counter is not zero, we already found at least one image delimiter. Check if it is the end of the image. */
      if (((ImagePtr[Loop1UInt32] == 0x55) && (ImagePtr[Loop1UInt32 - 1] == 0xAA)) || ((ImagePtr[Loop1UInt32] == 0xAA) && (ImagePtr[Loop1UInt32 - 1] == 0x55)))
      {
        // log_printf(__LINE__, __func__, "End-of-buffer 0x%2.2X found at %7lu   (%u)\n", ImagePtr[Loop1UInt32], Loop1UInt32, Counter);  // optional message to user.
        ++Counter;
        if (Counter >= 4)
        {
          log_printf(__LINE__, __func__, "End of image found at byte location: %lu\n", LastAddress);
          return ((Loop1UInt32 - 3) / 2);
        }
      }
      else
      {
        /* The delimiter found was not the ultimate image delimiter. Reset it. */
        // log_printf(__LINE__, __func__, "Delimiter found was not the ultimate image delimiter. Reset it.\n");  // optional message to user.
        Counter     = 0;
        LastAddress = 0l;
      }
    }
  }

  return 0l;
}


#include "get_pico_identifier.c"


/* $PAGE */
/* $TITLE=image_analysis() */
/* ============================================================================================================================================================= *\
                                                  This function may be used to find the number of pixels in an image file.
                                  and also to find the correct image definition using "brute force method" for an unknown RGB-565 image file.
                                                  Please note that <image files> used with this Firmware are text files with a special format. Use a text editor to take a look at their structure.
                          IMPORTANT: A special 4-byte footprint sequence (0xAA, 0x55, 0XAA, 0x55) must be added at the end of the image bitmap,
                                                                prior to calling "find_number_of_pixels()""
\* ============================================================================================================================================================= */
void image_analysis(void)
{
  UINT16 Loop1UInt16;

  UINT32 NbPixels;


  /* =========================================================================================================================================================== *\
                                                            Example of "find_number_of_pixels()" function.
  \* =========================================================================================================================================================== */
  /* Examples of using function "find_number_of_pixels()" to find the number of pixels in a RGB-565 image file. */
  log_printf(__LINE__, __func__, Separator);
  log_printf(__LINE__, __func__, "Example of using function: find_number_of_pixels()\n");
  log_printf(__LINE__, __func__, "Press <Enter> to find the number of pixels in image <MixedTubes.img>: ");
  if (stop_flow(TYPE_NONE)) return;
  NbPixels = find_number_of_pixels((UCHAR *)MixedTubes);
  log_printf(__LINE__, __func__, "Assuming the image depth is %u bytes per pixel (RGB-565)\n", StructLCD.Depth);
  log_printf(__LINE__, __func__, "Number of pixels found in image <MixedTubes.img>: %lu pixels (%lu bytes)\n", NbPixels, NbPixels * StructLCD.Depth);
  log_printf(__LINE__, __func__, Separator);
  printf(" \n\n");

  log_printf(__LINE__, __func__, Separator);
  log_printf(__LINE__, __func__, "Example of using function: find_number_of_pixels()\n");
  log_printf(__LINE__, __func__, "Press <Enter> to find the number of pixels in image <Nature1.img>: ");
  if (stop_flow(TYPE_NONE)) return;
  NbPixels = find_number_of_pixels((UCHAR *)Nature1);
  log_printf(__LINE__, __func__, "Assuming the image depth is %u bytes per pixel (RGB-565)\n", StructLCD.Depth);
  log_printf(__LINE__, __func__, "Number of pixels found in image <Nature1.img>: %lu pixels (%lu bytes)\n", NbPixels, NbPixels * StructLCD.Depth);
  log_printf(__LINE__, __func__, Separator);
  printf(" \n\n");

  log_printf(__LINE__, __func__, Separator);
  log_printf(__LINE__, __func__, "Example of using function: find_number_of_pixels()\n");
  log_printf(__LINE__, __func__, "Press <Enter> to find the number of pixels in image <Nature2.img>: ");
  if (stop_flow(TYPE_NONE)) return;
  NbPixels = find_number_of_pixels((UCHAR *)Nature2);
  log_printf(__LINE__, __func__, "Assuming the image depth is %u bytes per pixel (RGB-565)\n", StructLCD.Depth);
  log_printf(__LINE__, __func__, "Number of pixels found in image <Nature2.img>: %lu pixels (%lu bytes)\n", NbPixels, NbPixels * StructLCD.Depth);
  log_printf(__LINE__, __func__, Separator);
  printf(" \n\n");

  log_printf(__LINE__, __func__, Separator);
  log_printf(__LINE__, __func__, "Example of using function: find_number_of_pixels()\n");
  log_printf(__LINE__, __func__, "Press <Enter> to find the number of pixels in image <PicoLogo.img>: ");
  if (stop_flow(TYPE_NONE)) return;
  NbPixels = find_number_of_pixels((UCHAR *)PicoLogo);
  log_printf(__LINE__, __func__, "Assuming the image depth is %u bytes per pixel (RGB 565)\n", StructLCD.Depth);
  log_printf(__LINE__, __func__, "Number of pixels found in image <PicoLogo.img>: %lu pixels (%lu bytes)\n", NbPixels, NbPixels * StructLCD.Depth);
  log_printf(__LINE__, __func__, Separator);
  printf(" \n\n");

  log_printf(__LINE__, __func__, Separator);
  log_printf(__LINE__, __func__, "Example of using function: find_number_of_pixels()\n");
  log_printf(__LINE__, __func__, "Press <Enter> to find the number of pixels in image <SunFall.img>: ");
  if (stop_flow(TYPE_NONE)) return;
  NbPixels = find_number_of_pixels((UCHAR *)SunFall);
  log_printf(__LINE__, __func__, "Assuming the image depth is %u bytes per pixel (RGB-565)\n", StructLCD.Depth);
  log_printf(__LINE__, __func__, "Number of pixels found in image <SunFall.img>: %lu pixels (%lu bytes)\n", NbPixels, NbPixels * StructLCD.Depth);
  log_printf(__LINE__, __func__, Separator);
  printf(" \n\n");

  log_printf(__LINE__, __func__, Separator);
  log_printf(__LINE__, __func__, "Example of using function: find_number_of_pixels()\n");
  log_printf(__LINE__, __func__, "Press <Enter> to find the number of pixels in image <WaterFall.img>: ");
  if (stop_flow(TYPE_NONE)) return;
  NbPixels = find_number_of_pixels((UCHAR *)WaterFall);
  log_printf(__LINE__, __func__, "Assuming the image depth is %u bytes per pixel (RGB-565)\n", StructLCD.Depth);
  log_printf(__LINE__, __func__, "Number of pixels found in image WaterFall.img: %lu pixels (%lu bytes)\n", NbPixels, NbPixels * StructLCD.Depth);
  log_printf(__LINE__, __func__, Separator);
  printf(" \n\n");

  log_printf(__LINE__, __func__, Separator);
  log_printf(__LINE__, __func__, "Example of using function: find_number_of_pixels()\n");
  log_printf(__LINE__, __func__, "Press <Enter> to find the number of pixels in image <Waveshare1.img>: ");
  if (stop_flow(TYPE_NONE)) return;
  NbPixels = find_number_of_pixels((UCHAR *)Waveshare1);
  log_printf(__LINE__, __func__, "Assuming the image depth is %u bytes per pixel (RGB-565)\n", StructLCD.Depth);
  log_printf(__LINE__, __func__, "Number of pixels found in image <Waveshare1.img>: %lu pixels (%lu bytes)\n", NbPixels, NbPixels * StructLCD.Depth);
  log_printf(__LINE__, __func__, Separator);
  printf(" \n\n");

  log_printf(__LINE__, __func__, Separator);
  log_printf(__LINE__, __func__, "Example of using function: find_number_of_pixels()\n");
  log_printf(__LINE__, __func__, "Press <Enter> to find the number of pixels in image <Waveshare2.img>: ");
  if (stop_flow(TYPE_NONE)) return;
  NbPixels = find_number_of_pixels((UCHAR *)Waveshare2);
  log_printf(__LINE__, __func__, "Assuming the image depth is %u bytes per pixel (RGB-565)\n", StructLCD.Depth);
  log_printf(__LINE__, __func__, "Number of pixels found in image <Waveshare2.img>: %lu pixels (%lu bytes)\n", NbPixels, NbPixels * StructLCD.Depth);
  log_printf(__LINE__, __func__, Separator);
  printf(" \n\n");

  log_printf(__LINE__, __func__, Separator);
  log_printf(__LINE__, __func__, "Example of using function: find_number_of_pixels()\n");
  log_printf(__LINE__, __func__, "Press <Enter> to find the number of pixels in image <Waveshare3.img>: ");
  if (stop_flow(TYPE_NONE)) return;
  NbPixels = find_number_of_pixels((UCHAR *)Waveshare3);
  log_printf(__LINE__, __func__, "Assuming the image depth is %u bytes per pixel (RGB-565)\n", StructLCD.Depth);
  log_printf(__LINE__, __func__, "Number of pixels found in image <Waveshare3.img>: %lu pixels (%lu bytes)\n", NbPixels, NbPixels * StructLCD.Depth);
  log_printf(__LINE__, __func__, Separator);
  printf(" \n\n");



  /* ----------------------------------------------------------------------------------------------------------------------------------------------------------- *\
                                                            Finding the image definition of a RGB-565 image file.
  \* ----------------------------------------------------------------------------------------------------------------------------------------------------------- */
  /* Example of code used to find the right image definition... */
  /* NOTE: The image may not appear on LCD display if it is too large to be splashed on it. */
  printf(" \n\n\n");
  log_printf(__LINE__, __func__, Separator);
  log_printf(__LINE__, __func__, "<120>This section of code will try to find the right correct definition of a given RGB-565 formatted picture.\n");
  log_printf(__LINE__, __func__, "<120>First, use a text editor to take a look at the RGB-565 image files provided with this Firmware.\n");
  log_printf(__LINE__, __func__, "<120>Make sure the file you want to analyze is built the same way (do not forget the special 'footprint' bytes at the end).\n");
  log_printf(__LINE__, __func__, "<120>Then, #include your file and replace one of the filename below to analyze it.\n");
  log_printf(__LINE__, __func__, "<120>IMPORTANT: The logic below assume that your image file is 'horizontal' (landscape mode).\n");
  log_printf(__LINE__, __func__, "<120>If it is not, you will have to reverse the loop (from sqrt() down to 0) or to begin the loop starting at 0.\n");
  log_printf(__LINE__, __func__, "<120>Note: The process may be long and requires that you press <Enter> many, many times...\n");
  log_printf(__LINE__, __func__, Separator);
  log_printf(__LINE__, __func__, "Press <Enter> to find image definition of <PicoLogo> or press <ESC> to return to menu: ");
  if (stop_flow(TYPE_NONE)) return;

  LCD_clear(BLACK);
  LCD_refresh();
  log_printf(__LINE__, __func__, Separator);
  log_printf(__LINE__, __func__, "Start processing... Watch LCD display until picture becomes unscrambled.\n");
  log_printf(__LINE__, __func__, "Keep pressing <Enter> slowly until picture is unscrambled, then take note of image dimensions and press <ESC>.\n");
  log_printf(__LINE__, __func__, "Be aware that if you press <Enter> many times too fast, system may freeze...\n");
  NbPixels = find_number_of_pixels((UCHAR *)PicoLogo);
  log_printf(__LINE__, __func__, "Number of pixels: %lu pixels   (%lu bytes)\n", NbPixels, (NbPixels * 2));
  log_printf(__LINE__, __func__, " \n");

  for (Loop1UInt16 = (UINT16)(sqrt(NbPixels)); Loop1UInt16 <= 320; ++Loop1UInt16)  // assume that image is not wider than 320 pixels.
  {
    LCD_clear(BLACK);
    LCD_draw_image(0, 0, (UINT16 *)PicoLogo, (UINT16)(NbPixels / Loop1UInt16), Loop1UInt16);
    LCD_refresh();  // update LCD display.
    log_printf(__LINE__, __func__, "File <PicoLogo.img> drawn with width sets at %u pixels (%u X %u).\n", Loop1UInt16, (UINT16)(NbPixels / Loop1UInt16), Loop1UInt16);
    no_op(500);  // give some time for spi communication with LCD display.
    log_printf(__LINE__, __func__, "Press <Enter> to continue testing or press <ESC> to stop image rendering: ");
    if (stop_flow(TYPE_NONE)) break;
  }



  /* Example of code used to find the right image definition... for image "<Nature2.img>". */
  /* NOTE: It may be required to start the "for" loop with a value smaller then the "sqrt()" if the image proportion is out-of-scale. */
  /* NOTE: You may also need to end the "for" loop with a value higher than the LCD display dimensions if the image definition has a dimension greater. */
  /* NOTE: The image may not appear on LCD display if it is too large to be splashed on it. */
  log_printf(__LINE__, __func__, Separator);
  log_printf(__LINE__, __func__, "This section of code will try to find the right image definition of a given RGB-565 formatted picture...\n");
  log_printf(__LINE__, __func__, "Press <Enter> to find image definition of <Nature2.img> or press <ESC> to return to menu: ");
  if (stop_flow(TYPE_NONE)) return;

  LCD_clear(BLACK);
  LCD_refresh();
  log_printf(__LINE__, __func__, Separator);
  log_printf(__LINE__, __func__, "Start processing... Watch LCD display until picture becomes unscrambled.\n");
  log_printf(__LINE__, __func__, "Keep pressing <Enter> until picture is clear, then take note of image dimensions and press <ESC>.\n");
  log_printf(__LINE__, __func__, "Be aware that if you press <Enter> many times too fast, system may freeze...\n");
  NbPixels = find_number_of_pixels((UCHAR *)Nature2);
  log_printf(__LINE__, __func__, "Number of pixels: %lu pixels   (%lu bytes)\n", NbPixels, (NbPixels * 2));

  for (Loop1UInt16 = (UINT16)(sqrt(NbPixels)); Loop1UInt16 <= 320; ++Loop1UInt16)
  {
    LCD_clear(BLACK);
    LCD_draw_image(0, 0, (UINT16 *)Nature2, (UINT16)(NbPixels / Loop1UInt16), Loop1UInt16);
    sleep_ms(100);  // to prevent communication override.
    LCD_refresh();  // update LCD display.
    log_printf(__LINE__, __func__, "File <Nature2.img> drawn with width sets at %u pixels (%u X %u).\n", Loop1UInt16, (UINT16)(NbPixels / Loop1UInt16), Loop1UInt16);
    no_op(500);  // give some time for spi communication with LCD display.
    log_printf(__LINE__, __func__, "Press <Enter> to continue testing or press <ESC> to stop image rendering: ");
    if (stop_flow(TYPE_NONE)) break;
  }



  /* Example of code used to find the right image definition... for image "<SunFall.img>". */
  /* NOTE: It may be required to start the "for" loop with a value smaller then the "sqrt()" if the image proportion is out-of-scale. */
  /* NOTE: You may also need to end the "for" loop with a value higher than the LCD display dimensions if the image definition has a dimension greater. */
  /* NOTE: The image may not appear on LCD display if it is too large to be splashed on it. */
  log_printf(__LINE__, __func__, Separator);
  log_printf(__LINE__, __func__, "This section of code will try to find the right image definition of a given RGB-565 formatted picture...\n");
  log_printf(__LINE__, __func__, "Press <Enter> to find image definition of <SunFall.img> or press <ESC> to return to menu: ");
  if (stop_flow(TYPE_NONE)) return;

  LCD_clear(BLACK);
  LCD_refresh();
  log_printf(__LINE__, __func__, Separator);
  log_printf(__LINE__, __func__, "Start processing... Watch LCD display until picture becomes unscrambled.\n");
  log_printf(__LINE__, __func__, "Keep pressing <Enter> until picture is clear, then take note of image dimensions and press <ESC>.\n");
  log_printf(__LINE__, __func__, "Be aware that if you press <Enter> many times too fast, system may freeze...\n");
  NbPixels = find_number_of_pixels((UCHAR *)SunFall);
  log_printf(__LINE__, __func__, "Number of pixels: %lu pixels   (%lu bytes)\n", NbPixels, (NbPixels * 2));

  for (Loop1UInt16 = (UINT16)(sqrt(NbPixels)); Loop1UInt16 <= 320; ++Loop1UInt16)
  {
    LCD_clear(BLACK);
    LCD_draw_image(0, 0, (UINT16 *)SunFall, (UINT16)(NbPixels / Loop1UInt16), Loop1UInt16);
    sleep_ms(100);  // to prevent communication override.
    LCD_refresh();  // update LCD display.
    log_printf(__LINE__, __func__, "File <SunFall.img> drawn with width sets at %u pixels (%u X %u).\n", Loop1UInt16, (UINT16)(NbPixels / Loop1UInt16), Loop1UInt16);
    no_op(500);  // give some time for spi communication with LCD display.
    log_printf(__LINE__, __func__, "Press <Enter> to continue testing or press <ESC> to stop image rendering: ");
    if (stop_flow(TYPE_NONE)) break;
  }



  /* Example of code used to find the right image definition... for image "<Waveshare3.img>". */
  /* NOTE: It may be required to start the "for" loop with a value smaller then the "sqrt()" if the image proportion is out-of-scale. */
  /* NOTE: You may also need to end the "for" loop with a value higher than the LCD display dimensions if the image definition has a dimension greater. */
  /* NOTE: The image may not appear on LCD display if it is too large to be splashed on it. */
  log_printf(__LINE__, __func__, Separator);
  log_printf(__LINE__, __func__, "This section of code will try to find the right image definition of a given RGB-565 formatted picture...\n");
  log_printf(__LINE__, __func__, "Press <Enter> to find image definition of <Waveshare3.img> or press <ESC> to return to menu: ");
  if (stop_flow(TYPE_NONE)) return;

  LCD_clear(BLACK);
  LCD_refresh();
  log_printf(__LINE__, __func__, Separator);
  log_printf(__LINE__, __func__, "Start processing... Watch LCD display until picture becomes unscrambled.\n");
  log_printf(__LINE__, __func__, "Keep pressing <Enter> until picture is clear, then take note of image dimensions and press <ESC>.\n");
  log_printf(__LINE__, __func__, "Be aware that if you press <Enter> many times too fast, system may freeze...\n");
  NbPixels = find_number_of_pixels((UCHAR *)Waveshare3);
  log_printf(__LINE__, __func__, "Number of pixels: %lu pixels   (%lu bytes)\n", NbPixels, (NbPixels * 2));

  for (Loop1UInt16 = (UINT16)(sqrt(NbPixels)); Loop1UInt16 <= 320; ++Loop1UInt16)
  {
    LCD_clear(BLACK);
    LCD_draw_image(0, 0, (UINT16 *)Waveshare3, (UINT16)(NbPixels / Loop1UInt16), Loop1UInt16);
    sleep_ms(100);  // to prevent communication override.
    LCD_refresh();  // update LCD display.
    log_printf(__LINE__, __func__, "File <Waveshare3.img> drawn with width sets at %u pixels (%u X %u).\n", Loop1UInt16, (UINT16)(NbPixels / Loop1UInt16), Loop1UInt16);
    no_op(500);  // give some time for spi communication with LCD display.
    log_printf(__LINE__, __func__, "Press <Enter> to continue testing or press <ESC> to stop image rendering: ");
    if (stop_flow(TYPE_NONE)) break;
  }

  LCD_clear(BLACK);
  LCD_refresh();

  return;
}





/* $PAGE */
/* $TITLE=input_flush() */
/* ============================================================================================================================================================= *\
                                                                          Flush stdin buffer.
\* ============================================================================================================================================================= */
void input_flush(void)
{
  /* Does not work on Pico... to be investigated. */
  fflush(stdin);
  return;
}


#include "input_string.c"


/* $PAGE */
/* $TITLE=log_header() */
/* ============================================================================================================================================================= *\
                                                              Print Firmware header to log file.
\* ============================================================================================================================================================= */
void log_header(void)
{
  UCHAR String[120];

  /* If no terminal is connected, bypass... */
  if (!stdio_usb_connected()) return;

  log_printf(__LINE__, __func__, Separator);
  log_printf(__LINE__, __func__, "<120>Pico-LCD-Example\n");
  log_printf(__LINE__, __func__, "<120>Firmware version %s     Built on %s %s\n", FIRMWARE_VERSION, __DATE__, __TIME__);
  log_printf(__LINE__, __func__, "<120>Compatible with the SLASH Smart Home ecosystem family\n");
  log_printf(__LINE__, __func__, "<120>Pico unique ID: <%s>\n", PicoUniqueId);
  log_printf(__LINE__, __func__, "<120>Device identifier: <%s>\n", PicoIdentifier);

  switch (LCD_TYPE)
  {
    case (LCD_114):
      snprintf(String, sizeof(String), "Configured for Waveshare's 1.14-inch device.");
    break;

    case (LCD_200):
      snprintf(String, sizeof(String), "Configured for Waveshare's 2-inch device.");
    break;

    case (LCD_EXPLORER):
      snprintf(String, sizeof(String), "Configured for Pimoroni's Pico-Explorer device.");
    break;

    default:
      snprintf(String, sizeof(String), "UNRECOGNIZED DEVICE has been configured.");
    break;
  }

  /* Display type of LCD device selected. */
  log_printf(__LINE__, __func__, "<120>%s\n", String);
  log_printf(__LINE__, __func__, "<120>Current system timer value: %llu\n", time_us_64());

  log_printf(__LINE__, __func__, Separator);

  return;

}


#include "log_printf.c"


/* $PAGE */
/* $TITLE=no_op() */
/* ============================================================================================================================================================= *\
                                      Time delay function to use when sleep_ms() can't be used (inside interrupts or callbacks).
\* ============================================================================================================================================================= */
void no_op(UINT32 MSec)
{
  UINT64 TimerValue;

  TimerValue = time_us_64();
  while (time_us_64() < (TimerValue + (MSec * 1000ll)));

  return;
}





/* $PAGE */
/* $TITLE=stop_flow() */
/* ============================================================================================================================================================= *\
                                              Check if user wants to stop current process flow and return to menu. 
\* ============================================================================================================================================================= */
UINT8 stop_flow(UINT8 Type)
{
  UCHAR String[32];


  switch (Type)
  {
    case (TYPE_NONE):
      /* Nothing to do. */
    break;

    case (TYPE_CONTINUE):
      log_printf(__LINE__, __func__, "Press <Enter> to continue: ");
    break;

    case (TYPE_CONTINUE_OR_MENU):
      log_printf(__LINE__, __func__, "Press <Enter> to continue or <ESC> to return to menu: ");
    break;

    case (TYPE_PROCEED):
      log_printf(__LINE__, __func__, "Press <Enter> to proceed or <ESC> to return to menu: ");
    break;
  }
  input_string(String, 1, 0l);
  if (String[0] == 0x1B)
  {
    LCD_clear(BLACK);
    LCD_refresh();
    return 1;
  }
  return 0;
}





/* $PAGE */
/* $TITLE=term_menu()) */
/* ============================================================================================================================================================= *\
                                          Terminal menu when a CDC USB connection is detected during power up sequence.
\* ============================================================================================================================================================= */
void term_menu(void)
{
  UCHAR String[65];

  UINT8 FlagFound;
  UINT8 Menu;
  UINT8 Parameter[65];

  UINT16 Loop1UInt16;
  UINT16 Rotation;

  UINT32 Dum1UInt32;


  while (1)
  {
    display_LCD_presentation();
    printf("\n\n\n\n");
    log_printf(__LINE__, __func__, Separator);
    log_printf(__LINE__, __func__, "<120>Terminal menu\n");
    log_printf(__LINE__, __func__, Separator1);
    log_printf(__LINE__, __func__, "     1) - Show current LCD display settings.\n");
    log_printf(__LINE__, __func__, "     2) - Perform a hardware reset on LCD display and reinitialize it.\n");
    log_printf(__LINE__, __func__, "     3) - Perform a software reset on LCD display and reinitialize it.\n");
    log_printf(__LINE__, __func__, "     4) - Change rotation setting.\n");
    log_printf(__LINE__, __func__, "     5) - Mirror the framebuffer.\n");
    log_printf(__LINE__, __func__, "     6) - Reset LCD display in native mode.\n");
    log_printf(__LINE__, __func__, "     7) - Image analysis.\n");
    log_printf(__LINE__, __func__, "     8) - Testing function LCD_clear().\n");
    log_printf(__LINE__, __func__, "     9) - Testing function LCD_cls().\n");
    log_printf(__LINE__, __func__, "    10) - Testing function LCD_display_point().\n");
    log_printf(__LINE__, __func__, "    11) - Testing function LCD_draw_box().\n");
    log_printf(__LINE__, __func__, "    12) - Testing function LCD_draw_circle().\n");
    log_printf(__LINE__, __func__, "    13) - Testing function LCD_draw_image().\n");
    log_printf(__LINE__, __func__, "    14) - Testing function LCD_draw_line().\n");
    log_printf(__LINE__, __func__, "    15) - Testing function LCD_draw_point().\n");
    log_printf(__LINE__, __func__, "    16) - Testing function LCD_mirror_framebuffer().\n");
    log_printf(__LINE__, __func__, "    17) - Testing function LCD_part_clear().\n");
    log_printf(__LINE__, __func__, "    18) - Testing function LCD_part_cls().\n");
    log_printf(__LINE__, __func__, "    19) - Testing function LCD_part_refresh().\n");
    log_printf(__LINE__, __func__, "    20) - Testing function LCD_printf().\n");
    log_printf(__LINE__, __func__, "    21) - Testing function LCD_scroll().\n");
    log_printf(__LINE__, __func__, "    22) - Testing function LCD_set_brightness().\n");
    log_printf(__LINE__, __func__, "    23) - Testing function LCD_set_pixel().\n");
    log_printf(__LINE__, __func__, "    24) - Testing function LCD_set_rotation().\n");
    log_printf(__LINE__, __func__, "    25) - Testing function LCD_splash_image().\n");
    log_printf(__LINE__, __func__, "    26) - Proceed with all tests in sequence.\n");
    log_printf(__LINE__, __func__, "    27) - Jump to test zone.\n");
    log_printf(__LINE__, __func__, "    28) - Read LCD display parameters.\n");
    log_printf(__LINE__, __func__, " \n");
    log_printf(__LINE__, __func__, "    77) - Clear terminal screen.\n");
    log_printf(__LINE__, __func__, "    88) - Restart the Firmware.\n");
    log_printf(__LINE__, __func__, "    99) - Switch Pico in upload mode.\n");
    log_printf(__LINE__, __func__, " \n");
    log_printf(__LINE__, __func__, "          Enter your choice: ");
    input_string(String, sizeof(String), 0l);

    /* If user pressed <Enter> only, loop back to menu. */
    if (String[0] == 0x0D) continue;

    /* If user pressed <ESC>, loop back to menu. */
    if (String[0] == 0x1B)
    {
      String[0] = 0x00;
      printf("\n\n\n");

      return;
    }

    /* User pressed a menu option, execute it. */
    Menu = atoi(String);
    switch(Menu)
    {
      case (1):
        /* Show current LCD display settings. */
        printf("\n\n\n");
        LCD_display_info();
        if (stop_flow(TYPE_CONTINUE)) return;
      break;

      case (2):
        /* Perform a hardware reset on LCD display. */
        printf("\n\n\n");
        log_printf(__LINE__, __func__, Separator);
        log_printf(__LINE__, __func__, "<120>Perform a hardware reset on LCD display.\n");
        log_printf(__LINE__, __func__, Separator);
        log_printf(__LINE__, __func__, "Press <G> to do a hardware reset on LCD display and reinitialize it in native mode...\n");
        log_printf(__LINE__, __func__, "...or press <Enter> to abort operation and return to menu: ");
        input_string(String, 1, 0l);
        if ((String[0] == 'G') || (String[0] == 'g'))
        {
          log_printf(__LINE__, __func__, "Performing hardware reset and re-initializing LCD display.\n");
          LCD_exit();  // release memory allocated to framebuffer.
          LCD_hard_reset();
          LCD_init(ROTATE_0, BLACK);
          log_printf(__LINE__, __func__, "Hardware reset has been done on LCD display...\n");
          log_printf(__LINE__, __func__, Separator);
          if (stop_flow(TYPE_CONTINUE)) return;
        }
        else
        {
          log_printf(__LINE__, __func__, "Operator didn't press <G>, hardware reset has not been done.\n");
          log_printf(__LINE__, __func__, Separator);
          sleep_ms(1000);
        }
      break;

      case (3):
        /* Perform a software reset on LCD display. */
        printf("\n\n\n");
        log_printf(__LINE__, __func__, Separator);
        log_printf(__LINE__, __func__, "<120>Perform a software reset on LCD display.\n");
        log_printf(__LINE__, __func__, Separator);
        log_printf(__LINE__, __func__, "Press <G> to do a software reset on LCD display and reinitialize it in native mode...\n");
        log_printf(__LINE__, __func__, "...or press <Enter> to abort operation and return to menu: ");
        input_string(String, 1, 0l);
        if ((String[0] == 'G') || (String[0] == 'g'))
        {
          log_printf(__LINE__, __func__, "Performing software reset and re-initializing LCD display.\n");
          LCD_exit();  // release memory allocated to framebuffer.
          LCD_soft_reset();
          LCD_init(ROTATE_0, BLACK);
          log_printf(__LINE__, __func__, "Software reset has been done on LCD display...\n");
          log_printf(__LINE__, __func__, Separator);
          if (stop_flow(TYPE_CONTINUE)) return;
        }
        else
        {
          log_printf(__LINE__, __func__, "Operator didn't press <G>, software reset has not been done.\n");
          log_printf(__LINE__, __func__, Separator);
          sleep_ms(1000);
        }
      break;

      case (4):
        printf("\n\n\n");
        log_printf(__LINE__, __func__, Separator);
        log_printf(__LINE__, __func__, "<120>Change LCD display rotation setting.\n");
        log_printf(__LINE__, __func__, "<120>Current rotation setting is: %u degrees.\n", StructLCD.Rotation);
        log_printf(__LINE__, __func__, Separator);
        switch (StructLCD.DeviceType)
        {
          case LCD_114:
          case LCD_200:
            log_printf(__LINE__, __func__, "Native mode (or rotation 0 degree) means LCD display in portrait mode, USB connector at the bottome of the display.\n");
            log_printf(__LINE__, __func__, "Changing from 0 to 90 degrees means that you make a quarter of a turn in clockwise direction with the LCD display.\n");
            log_printf(__LINE__, __func__, "The same applies when changing from 90 to 180 degrees or when changing from 180 to 270 degrees.\n");
          break;

          case LCD_EXPLORER:
            log_printf(__LINE__, __func__, "Native mode (or rotation 0 degree) means LCD display at the right of Pico Explorer Base and Pico on the upper left.\n");
            log_printf(__LINE__, __func__, "from the front of the device. Changing from 0 to 90 degrees means that you make a quarter of a turn in clockwise direction\n");
            log_printf(__LINE__, __func__, "with the Pico Explorer. The same applies when changing from 90 to 180 degrees or when changing from 180 to 270 degrees.\n");
          break;

          default:
            log_printf(__LINE__, __func__, "Unrecognized device (%u).\n", StructLCD.DeviceType);
            return;
          break;
        }

        log_printf(__LINE__, __func__, " \n");
        log_printf(__LINE__, __func__, "Enter 0, 90, 180 or 270 to set LCD display rotation setting or press <Enter> to return to menu: ");
        input_string(String, sizeof(String), 0l);
        if (String[0] == 0x0D)
        {
          log_printf(__LINE__, __func__, "User pressed <Enter>, returning to menu with rotation setting set to %u degrees...\n", StructLCD.Rotation);
          log_printf(__LINE__, __func__, Separator);
          sleep_ms(1000);
          return;
        }
          
        Rotation = atoi(String);
        if ((Rotation == 0) || (Rotation == 90) || (Rotation == 180) || (Rotation == 270))
        {
          LCD_set_rotation(Rotation);
          log_printf(__LINE__, __func__, "Rotation has been set to %u degrees\n", Rotation);
          log_printf(__LINE__, __func__, Separator);
          sleep_ms(1000);
        }
        else
        {
          log_printf(__LINE__, __func__, "Invalid rotation setting specified (%u)\n", Rotation);
          log_printf(__LINE__, __func__, "Rotation setting unchanged at %u-degrees.\n", StructLCD.Rotation);
          log_printf(__LINE__, __func__, Separator);
          sleep_ms(1000);
        }
      break;

      case (5):
        FlagFound = FLAG_OFF;
        printf("\n\n\n");
        log_printf(__LINE__, __func__, Separator);
        log_printf(__LINE__, __func__, "<120>Mirror the framebuffer.\n");
        log_printf(__LINE__, __func__, Separator);
        log_printf(__LINE__, __func__, "Function LCD_mirror_framebuffer() allows inverting current content of framebuffer.\n");
        log_printf(__LINE__, __func__, "Mirroring can be <none>, <horizontal>, <vertical>, or <origin>.\n");
        log_printf(__LINE__, __func__, "Horizontal mirroring exchanges the right with the left while displaying.\n");
        log_printf(__LINE__, __func__, "Vertical mirroring exchanges the top with the bottom while displaying.\n");
        log_printf(__LINE__, __func__, "Origin mirroring exchanges both the right with the left and the top with the bottom.\n");
        log_printf(__LINE__, __func__, " \n");
        log_printf(__LINE__, __func__, "Enter the type of mirroring you want (N, H, V or O) or <Enter> to return to menu with no change to framebuffer: ");
        input_string(String, 1, 0l);
        if (String[0] == 0x0D)
        {
          log_printf(__LINE__, __func__, "User pressed <Enter>. No change has been done to LCD display.\n", StructLCD.Mirroring[StructLCD.MirrorMode]);
          log_printf(__LINE__, __func__, Separator);
          return;
        }

        if ((String[0] == 'N') || (String[0] == 'n'))
        {
          log_printf(__LINE__, __func__, "LCD display mirror to None.\n");
          LCD_mirror_framebuffer(0, 0, StructLCD.Height, StructLCD.Width, MIRROR_NONE);
          log_printf(__LINE__, __func__, "FrameBuffer has been set to NO mirroring.\n");
          FlagFound = FLAG_ON;
        }

        if ((String[0] == 'H') || (String[0] == 'h'))
        {
          log_printf(__LINE__, __func__, "LCD display mirror to Horizontal.\n");
          LCD_mirror_framebuffer(0, 0, StructLCD.Height, StructLCD.Width, MIRROR_HORIZONTAL);
          log_printf(__LINE__, __func__, "FrameBuffer has been set to HORIZONTAL mirroring.\n");
          FlagFound = FLAG_ON;
        }

        if ((String[0] == 'V') || (String[0] == 'v'))
        {
          log_printf(__LINE__, __func__, "LCD display mirror to Vertical.\n");
          LCD_mirror_framebuffer(0, 0, StructLCD.Height, StructLCD.Width, MIRROR_VERTICAL);
          log_printf(__LINE__, __func__, "FrameBuffer has been set to VERTICAL mirroring.\n");
          FlagFound = FLAG_ON;
        }

        if ((String[0] == 'O') || (String[0] == 'o'))
        {
          log_printf(__LINE__, __func__, "LCD display mirror to Origin.\n");
          LCD_mirror_framebuffer(0, 0, StructLCD.Height, StructLCD.Width, MIRROR_ORIGIN);
          log_printf(__LINE__, __func__, "FrameBuffer has been set to ORIGIN mirroring.\n");
          FlagFound = FLAG_ON;
        }

        if (FlagFound)
        {
          if (stop_flow(TYPE_CONTINUE)) return;
          log_printf(__LINE__, __func__, Separator);
          if (stop_flow(TYPE_CONTINUE)) return;
        }
        else
        {
          log_printf(__LINE__, __func__, "Unrecognized selection entered for framebuffer mirroring. Must be one of: N, H, V or O.\n");
          log_printf(__LINE__, __func__, "Framebuffer mirroring has been unchanged at %s.\n", StructLCD.Mirroring[StructLCD.MirrorMode]);
          log_printf(__LINE__, __func__, Separator);
          sleep_ms(1000);
        }
      break;

      case (6):
        /* Reset LCD display in native mode. */
        printf("\n\n\n");
        log_printf(__LINE__, __func__, Separator);
        log_printf(__LINE__, __func__, "<120>Reset LCD display in native mode.\n");
        log_printf(__LINE__, __func__, Separator);
        log_printf(__LINE__, __func__, "Press <G> to proceed with native mode reset or press <ESC> to return to menu: ");
        input_string(String, 1, 0l);
        if ((String[0] == 'G') || (String[0] == 'g'))
        {
          LCD_init(ROTATE_0, WHITE);
        }
        else
        {
          log_printf(__LINE__, __func__, "Operator didn't press <G>... LCD display has not been reset in native mode.\n");
        }
        log_printf(__LINE__, __func__, Separator);
      break;

      case (7):
        /* Image analysis. */
        printf("\n\n\n");
        log_printf(__LINE__, __func__, Separator);
        log_printf(__LINE__, __func__, "<120>Proceed with image analysis.\n");
        log_printf(__LINE__, __func__, Separator);
        log_printf(__LINE__, __func__, "Press <Enter> to proceed with image analysis or press <ESC> to return to menu: ");
        if (stop_flow(TYPE_NONE)) break;
        image_analysis();
        log_printf(__LINE__, __func__, Separator);
      break;

      case (8):
        /* Test function LCD_clear(). */
        printf("\n\n\n");
        log_printf(__LINE__, __func__, Separator);
        log_printf(__LINE__, __func__, "<120>LCD_clear() function tests.\n");
        log_printf(__LINE__, __func__, Separator);
        log_printf(__LINE__, __func__, "Press <Enter> to proceed with function LCD_clear() tests or press <ESC> to return to menu: ");
        if (stop_flow(TYPE_NONE)) break;
        test_lcd_clear();
      break;

      case (9):
        /* Test function LCD_cls(). */
        printf("\n\n\n");
        test_lcd_cls();
      break;

      case (10):
        /* Test function LCD_display_point(). */
        printf("\n\n\n");
        log_printf(__LINE__, __func__, Separator);
        log_printf(__LINE__, __func__, "<120>LCD_display_point() function tests.\n");
        log_printf(__LINE__, __func__, Separator);
        log_printf(__LINE__, __func__, "Press <Enter> to proceed with function LCD_display_point() tests or press <ESC> to return to menu: ");
        if (stop_flow(TYPE_NONE)) break;
        test_lcd_display_point();
      break;

      case (11):
        /* Test function LCD_draw_box(). */
        printf("\n\n\n");
        log_printf(__LINE__, __func__, Separator);
        log_printf(__LINE__, __func__, "<120>LCD_draw_box() function tests.\n");
        log_printf(__LINE__, __func__, Separator);
        log_printf(__LINE__, __func__, "Press <Enter> to proceed with function LCD_draw_box() tests or press <ESC> to return to menu: ");
        if (stop_flow(TYPE_NONE)) break;
        test_lcd_draw_box();
      break;

      case (12):
        /* Test function LCD_draw_circle(). */
        printf("\n\n\n");
        log_printf(__LINE__, __func__, Separator);
        log_printf(__LINE__, __func__, "<120>LCD_draw_circle() function tests.\n");
        log_printf(__LINE__, __func__, Separator);
        log_printf(__LINE__, __func__, "Press <Enter> to proceed with function LCD_draw_circle() tests or press <ESC> to return to menu: ");
        if (stop_flow(TYPE_NONE)) break;
        test_lcd_draw_circle();
      break;

      case (13):
        /* Test function LCD_draw_image(). */
        printf("\n\n\n");
        log_printf(__LINE__, __func__, Separator);
        log_printf(__LINE__, __func__, "<120>LCD_draw_image() function tests.\n");
        log_printf(__LINE__, __func__, Separator);
        log_printf(__LINE__, __func__, "Press <Enter> to proceed with function LCD_draw_image() tests or press <ESC> to return to menu: ");
        if (stop_flow(TYPE_NONE)) break;
        test_lcd_draw_image();
      break;

      case (14):
        /* Test function LCD_draw_line(). */
        printf("\n\n\n");
        log_printf(__LINE__, __func__, Separator);
        log_printf(__LINE__, __func__, "<120>LCD_draw_line() function tests.\n");
        log_printf(__LINE__, __func__, Separator);
        log_printf(__LINE__, __func__, "Press <Enter> to proceed with function LCD_draw_line() tests or press <ESC> to return to menu: ");
        if (stop_flow(TYPE_NONE)) break;
        test_lcd_draw_line();
      break;

      case (15):
        /* Test function LCD_draw_point(). */
        log_printf(__LINE__, __func__, Separator);
        log_printf(__LINE__, __func__, "<120>LCD_draw_point() function tests.\n");
        log_printf(__LINE__, __func__, Separator);
        log_printf(__LINE__, __func__, "Press <Enter> to proceed with function LCD_draw_point() tests or press <ESC> to return to menu: ");
        if (stop_flow(TYPE_NONE)) break;
        test_lcd_draw_point();
      break;

      case (16):
        /* Test function LCD_mirror_framebuffer(). */
        log_printf(__LINE__, __func__, Separator);
        log_printf(__LINE__, __func__, "<120>LCD_mirror_framebuffer() function tests.\n");
        log_printf(__LINE__, __func__, Separator);
        log_printf(__LINE__, __func__, "Press <Enter> to proceed with function LCD_mirror_framebuffer() tests or press <ESC> to return to menu: ");
        if (stop_flow(TYPE_NONE)) break;
        test_lcd_mirror_framebuffer();
      break;

      case (17):
        /* Test function LCD_part_clear(). */
        log_printf(__LINE__, __func__, Separator);
        log_printf(__LINE__, __func__, "<120>LCD_part_clear() function tests.\n");
        log_printf(__LINE__, __func__, Separator);
        log_printf(__LINE__, __func__, "Press <Enter> to proceed with function LCD_part_clear() tests or press <ESC> to return to menu: ");
        if (stop_flow(TYPE_NONE)) break;
        test_lcd_part_clear();
      break;

      case (18):
        /* Test function LCD_part_cls(). */
        log_printf(__LINE__, __func__, Separator);
        log_printf(__LINE__, __func__, "<120>LCD_part_cls() function tests.\n");
        log_printf(__LINE__, __func__, Separator);
        log_printf(__LINE__, __func__, "Press <Enter> to proceed with function LCD_part_cls() tests or press <ESC> to return to menu: ");
        if (stop_flow(TYPE_NONE)) break;
        test_lcd_part_cls();
      break;

      case (19):
        /* Test function LCD_part_refresh(). */
        log_printf(__LINE__, __func__, Separator);
        log_printf(__LINE__, __func__, "<120>LCD_part_refresh() function tests.\n");
        log_printf(__LINE__, __func__, Separator);
        log_printf(__LINE__, __func__, "Press <Enter> to proceed with function LCD_part_refresh() tests or press <ESC> to return to menu: ");
        if (stop_flow(TYPE_NONE)) break;
        test_lcd_part_refresh();
      break;

      case (20):
        /* Test function LCD_printf(). */
        printf("\n\n\n");
        log_printf(__LINE__, __func__, Separator);
        log_printf(__LINE__, __func__, "<120>LCD_printf() function tests.\n");
        log_printf(__LINE__, __func__, Separator);
        log_printf(__LINE__, __func__, "Press <Enter> to proceed with function LCD_printf() tests or press <ESC> to return to menu: ");
        if (stop_flow(TYPE_NONE)) break;
        test_lcd_printf();
      break;

      case (21):
        /* Test function LCD_scroll(). */
        log_printf(__LINE__, __func__, Separator);
        log_printf(__LINE__, __func__, "<120>LCD_scroll() function tests.\n");
        log_printf(__LINE__, __func__, Separator);
        log_printf(__LINE__, __func__, "Press <Enter> to proceed with function LCD_scroll() tests or press <ESC> to return to menu: ");
        if (stop_flow(TYPE_NONE)) break;
       test_lcd_scroll();
      break;

      case (22):
        /* Test function LCD_set_brightness(). */
        log_printf(__LINE__, __func__, Separator);
        log_printf(__LINE__, __func__, "<120>LCD_set_brightness() function tests.\n");
        log_printf(__LINE__, __func__, Separator);
        log_printf(__LINE__, __func__, "Press <Enter> to proceed with function LCD_set_brightness() tests or press <ESC> to return to menu: ");
        if (stop_flow(TYPE_NONE)) break;
        test_lcd_set_brightness();
      break;

      case (23):
        /* Test function LCD_set_pixel(). */
        log_printf(__LINE__, __func__, Separator);
        log_printf(__LINE__, __func__, "<120>LCD_set_pixel() function tests.\n");
        log_printf(__LINE__, __func__, Separator);
        log_printf(__LINE__, __func__, "Press <Enter> to proceed with function LCD_set_pixel() tests or press <ESC> to return to menu: ");
        if (stop_flow(TYPE_NONE)) break;
        test_lcd_set_pixel();
      break;

      case (24):
        /* Test function LCD_set_rotation(). */
        log_printf(__LINE__, __func__, Separator);
        log_printf(__LINE__, __func__, "<120>LCD_set_rotation() function tests.\n");
        log_printf(__LINE__, __func__, Separator);
        log_printf(__LINE__, __func__, "Press <Enter> to proceed with function LCD_set_rotation() tests or press <ESC> to return to menu: ");
        if (stop_flow(TYPE_NONE)) break;
        test_lcd_set_rotation();
      break;

      case (25):
        /* Test function LCD_splash_image(). */
        log_printf(__LINE__, __func__, Separator);
        log_printf(__LINE__, __func__, "<120>LCD_splash_image() function tests.\n");
        log_printf(__LINE__, __func__, Separator);
        log_printf(__LINE__, __func__, "Press <Enter> to proceed with function LCD_splash_image() tests or press <ESC> to return to menu: ");
        if (stop_flow(TYPE_NONE)) break;
        test_lcd_splash_image();
      break;

      case (26):
        /* Proceed with all tests in sequence. */
        log_printf(__LINE__, __func__, "CLS");
        log_printf(__LINE__, __func__, "HOME");
        log_printf(__LINE__, __func__, Separator);
        log_printf(__LINE__, __func__, "<120>Performing all function tests in sequence.\n");
        log_printf(__LINE__, __func__, "<120>All tests will be performed for each rotation setting, beginning with native mode (rotation 0-degree).\n");
        log_printf(__LINE__, __func__, "<120>Then, tests will be repeated with rotation sets to 90-degree, then 180-degree and finally 270-degree.\n");
        FlagAllTests = FLAG_ON;
        for (Loop1UInt16 = 0; Loop1UInt16 < 4; ++Loop1UInt16)
        {
          switch (Loop1UInt16)
          {
            case (0):
              LCD_set_rotation(ROTATE_0);
            break;

            case (1):
              LCD_set_rotation(ROTATE_90);
            break;

            case (2):
              LCD_set_rotation(ROTATE_180);
            break;

            case (3):
              LCD_set_rotation(ROTATE_270);
            break;
          }
          log_printf(__LINE__, __func__, "<120>Current rotation mode: %u-degree.\n", StructLCD.Rotation);
          display_LCD_presentation();
          log_printf(__LINE__, __func__, Separator);
          LCD_display_info();
  
          log_printf(__LINE__, __func__, "Press <Enter> to proceed with all tests in sequence or press <ESC> to return to menu: ");
          if (stop_flow(TYPE_NONE))
          {
            log_printf(__LINE__, __func__, "CLS");
            log_printf(__LINE__, __func__, "HOME");
            break;
          }
 
          /* --------------------------------------------------------- LCD_clear() --------------------------------------------------------------- */
          log_printf(__LINE__, __func__, Separator);
          log_printf(__LINE__, __func__, "Press <Enter> to proceed with LCD_clear() tests or press <ESC> to return to menu: ");
          if (stop_flow(TYPE_NONE)) break;
          log_printf(__LINE__, __func__, "CLS");
          log_printf(__LINE__, __func__, "HOME");
          test_lcd_clear();

          /* ---------------------------------------------------------- LCD_cls() ---------------------------------------------------------------- */
          log_printf(__LINE__, __func__, Separator);
          log_printf(__LINE__, __func__, "Press <Enter> to proceed with LCD_cls() tests or press <ESC> to return to menu: ");
          if (stop_flow(TYPE_NONE)) break;
          log_printf(__LINE__, __func__, "CLS");
          log_printf(__LINE__, __func__, "HOME");
          test_lcd_cls();

          /* ---------------------------------------------------- LCD_display_point() ------------------------------------------------------------ */
          log_printf(__LINE__, __func__, Separator);
          log_printf(__LINE__, __func__, "Press <Enter> to proceed with LCD_display_point() tests or press <ESC> to return to menu: ");
          if (stop_flow(TYPE_NONE)) break;
          log_printf(__LINE__, __func__, "CLS");
          log_printf(__LINE__, __func__, "HOME");
          test_lcd_display_point();

          /* ------------------------------------------------------- LCD_draw_box() -------------------------------------------------------------- */
          log_printf(__LINE__, __func__, Separator);
          log_printf(__LINE__, __func__, "Press <Enter> to proceed with LCD_draw_box() tests or press <ESC> to return to menu: ");
          if (stop_flow(TYPE_NONE)) break;
          log_printf(__LINE__, __func__, "CLS");
          log_printf(__LINE__, __func__, "HOME");
          test_lcd_draw_box();

          /* ------------------------------------------------------- LCD_draw_circle() ----------------------------------------------------------- */
          log_printf(__LINE__, __func__, Separator);
          log_printf(__LINE__, __func__, "Press <Enter> to proceed with LCD_draw_circle() tests or press <ESC> to return to menu: ");
          if (stop_flow(TYPE_NONE)) break;
          log_printf(__LINE__, __func__, "CLS");
          log_printf(__LINE__, __func__, "HOME");
          test_lcd_draw_circle();

          /* ------------------------------------------------------- LCD_draw_image() ------------------------------------------------------------ */
          log_printf(__LINE__, __func__, Separator);
          log_printf(__LINE__, __func__, "Press <Enter> to proceed with LCD_draw_image() tests or press <ESC> to return to menu: ");
          if (stop_flow(TYPE_NONE)) break;
          log_printf(__LINE__, __func__, "CLS");
          log_printf(__LINE__, __func__, "HOME");
          test_lcd_draw_image();

          /* ------------------------------------------------------- LCD_draw_line() ------------------------------------------------------------- */
          log_printf(__LINE__, __func__, Separator);
          log_printf(__LINE__, __func__, "Press <Enter> to proceed with LCD_draw_line() tests or press <ESC> to return to menu: ");
          if (stop_flow(TYPE_NONE)) break;
          log_printf(__LINE__, __func__, "CLS");
          log_printf(__LINE__, __func__, "HOME");
          test_lcd_draw_line();

          /* ------------------------------------------------------- LCD_draw_point() ------------------------------------------------------------ */
          log_printf(__LINE__, __func__, Separator);
          log_printf(__LINE__, __func__, "Press <Enter> to proceed with LCD_draw_point() tests or press <ESC> to return to menu: ");
          if (stop_flow(TYPE_NONE)) break;
          log_printf(__LINE__, __func__, "CLS");
          log_printf(__LINE__, __func__, "HOME");
          test_lcd_draw_point();

          /* ------------------------------------------------------- LCD_part_clear() ------------------------------------------------------------ */
          log_printf(__LINE__, __func__, Separator);
          log_printf(__LINE__, __func__, "Press <Enter> to proceed with LCD_part_clear() tests or press <ESC> to return to menu: ");
          if (stop_flow(TYPE_NONE)) break;
          log_printf(__LINE__, __func__, "CLS");
          log_printf(__LINE__, __func__, "HOME");
          test_lcd_part_clear();

          /* -------------------------------------------------------- LCD_part_cls() ------------------------------------------------------------- */
          log_printf(__LINE__, __func__, Separator);
          log_printf(__LINE__, __func__, "Press <Enter> to proceed with LCD_part_cls() tests or press <ESC> to return to menu: ");
          if (stop_flow(TYPE_NONE)) break;
          log_printf(__LINE__, __func__, "CLS");
          log_printf(__LINE__, __func__, "HOME");
          test_lcd_part_cls();

          /* ------------------------------------------------------ LCD_part_refresh() ----------------------------------------------------------- */
          log_printf(__LINE__, __func__, Separator);
          log_printf(__LINE__, __func__, "Press <Enter> to proceed with LCD_part_refresh() tests or press <ESC> to return to menu: ");
          if (stop_flow(TYPE_NONE)) break;
          log_printf(__LINE__, __func__, "CLS");
          log_printf(__LINE__, __func__, "HOME");
          test_lcd_part_refresh();

          /* --------------------------------------------------------- LCD_printf() -------------------------------------------------------------- */
          log_printf(__LINE__, __func__, Separator);
          log_printf(__LINE__, __func__, "Press <Enter> to proceed with LCD_printf() tests or press <ESC> to return to menu: ");
          if (stop_flow(TYPE_NONE)) break;
          log_printf(__LINE__, __func__, "CLS");
          log_printf(__LINE__, __func__, "HOME");
          test_lcd_printf();

          /* --------------------------------------------------------- LCD_scroll() -------------------------------------------------------------- */
          log_printf(__LINE__, __func__, Separator);
          log_printf(__LINE__, __func__, "Press <Enter> to proceed with LCD_scroll() tests or press <ESC> to return to menu: ");
          if (stop_flow(TYPE_NONE)) break;
          log_printf(__LINE__, __func__, "CLS");
          log_printf(__LINE__, __func__, "HOME");
          test_lcd_scroll();

          /* ----------------------------------------------------- LCD_set_brightness() ---------------------------------------------------------- */
          log_printf(__LINE__, __func__, Separator);
          log_printf(__LINE__, __func__, "Press <Enter> to proceed with LCD_set_brightness() tests or press <ESC> to return to menu: ");
          if (stop_flow(TYPE_NONE)) break;
          log_printf(__LINE__, __func__, "CLS");
          log_printf(__LINE__, __func__, "HOME");
          test_lcd_set_brightness();

          /* ------------------------------------------------------ LCD_mirror_framebuffer() ------------------------------------------------------------ */
          log_printf(__LINE__, __func__, Separator);
          log_printf(__LINE__, __func__, "Press <Enter> to proceed with LCD_mirror_framebuffer() tests or press <ESC> to return to menu: ");
          if (stop_flow(TYPE_NONE)) break;
          log_printf(__LINE__, __func__, "CLS");
          log_printf(__LINE__, __func__, "HOME");
          test_lcd_mirror_framebuffer();

          /* -------------------------------------------------------- LCD_set_pixel() ------------------------------------------------------------- */
          log_printf(__LINE__, __func__, Separator);
          log_printf(__LINE__, __func__, "Press <Enter> to proceed with LCD_set_pixel() tests or press <ESC> to return to menu: ");
          if (stop_flow(TYPE_NONE)) break;
          log_printf(__LINE__, __func__, "CLS");
          log_printf(__LINE__, __func__, "HOME");
          test_lcd_set_pixel();

          /* ------------------------------------------------------ LCD_set_rotation() ----------------------------------------------------------- */
          log_printf(__LINE__, __func__, Separator);
          log_printf(__LINE__, __func__, "Press <Enter> to proceed with LCD_set_rotation() tests or press <ESC> to return to menu: ");
          if (stop_flow(TYPE_NONE)) break;
          log_printf(__LINE__, __func__, "CLS");
          log_printf(__LINE__, __func__, "HOME");
          test_lcd_set_rotation();

          /* ------------------------------------------------------ LCD_splash_image() ----------------------------------------------------------- */
          log_printf(__LINE__, __func__, Separator);
          log_printf(__LINE__, __func__, "Press <Enter> to proceed with LCD_splash_image() tests or press <ESC> to return to menu: ");
          if (stop_flow(TYPE_NONE)) break;
          log_printf(__LINE__, __func__, "CLS");
          log_printf(__LINE__, __func__, "HOME");
          test_lcd_splash_image();

          /* --------------------------------------------------------- Miscellaneous ------------------------------------------------------------- */
          // log_printf(__LINE__, __func__, Separator);
          // log_printf(__LINE__, __func__, "Press <Enter> to proceed with miscellaneous combined tests or press <ESC> to return to menu: ");
          // if (stop_flow(TYPE_NONE)) break;
          // log_printf(__LINE__, __func__, "CLS");
          // log_printf(__LINE__, __func__, "HOME");
          // test_miscellaneous();
        }
        FlagAllTests = FLAG_OFF;
      break;

      case (27):
        /* Jump to test zone. */
        log_printf(__LINE__, __func__, Separator);
        log_printf(__LINE__, __func__, "<120>Jumping to test zone.\n");
        log_printf(__LINE__, __func__, Separator);
        log_printf(__LINE__, __func__, "Press <Enter> to jump to test menu or press <ESC> to return to main menu: ");
        if (stop_flow(TYPE_NONE)) break;
        zone_test1();
        log_printf(__LINE__, __func__, Separator);
      break;

      case (28):
        /* Read LCD display parameters. */
        log_printf(__LINE__, __func__, Separator);
        LCD_read(Parameter, 4);
        log_printf(__LINE__, __func__, "Parameter[0]: 0x%2.2X\n", Parameter[0]);
        log_printf(__LINE__, __func__, "Parameter[1]: 0x%2.2X\n", Parameter[1]);
        log_printf(__LINE__, __func__, "Parameter[2]: 0x%2.2X\n", Parameter[2]);
        log_printf(__LINE__, __func__, "Parameter[3]: 0x%2.2X\n", Parameter[3]);
        Dum1UInt32 = Parameter[0] + (Parameter[1] << 8) + (Parameter[2] << 16) + (Parameter[3] << 24);
        log_printf(__LINE__, __func__, "Concatenated as 32 bits: 0x%8.8X\n", Dum1UInt32);
        log_printf(__LINE__, __func__, "Manufacturer ID: 0x%2.2X\n", (Dum1UInt32 >> 10) & 0xFF);
        log_printf(__LINE__, __func__, "Driver version:  0x%2.2X\n", (Dum1UInt32 >> 20) & 0x3F);
        log_printf(__LINE__, __func__, "Driver ID:       0x%2.2X\n", (Dum1UInt32 >> 30) & 0xFF);
        log_printf(__LINE__, __func__, Separator);
        if (stop_flow(TYPE_CONTINUE)) return;
      break;

      case (77):
        /* Clear terminal screen. */
        log_printf(__LINE__, __func__, "CLS");
        log_printf(__LINE__, __func__, "HOME");
        return;
      break;

      case (88):
        /* Restart the Firmware. */
        log_printf(__LINE__, __func__, Separator);
        log_printf(__LINE__, __func__, "<120>Restart the Firmware.\n");
        log_printf(__LINE__, __func__, Separator);
        log_printf(__LINE__, __func__, "Press <G> to restart Firmware or <Enter> to return to menu: ");
        input_string(String, 1, 0l);
        if ((String[0] == 'G') || (String[0] == 'g'))
        {
          log_printf(__LINE__, __func__, "Restarting Firmware...\n");
          log_printf(__LINE__, __func__, Separator);
          LCD_clear(BLACK);
          LCD_refresh();
          sleep_ms(2000);
          watchdog_enable(1, 1);
        }
        else
        {
          /* User didn't pres <G>. */
          log_printf(__LINE__, __func__, "User didn't press <G> Firmware will not be restarted.\n");
          log_printf(__LINE__, __func__, Separator);
          sleep_ms(1000);
          log_printf(__LINE__, __func__, "CLS");
        }
      break;

      case (99):
        /* Switch the Pico in upload mode. */
        log_printf(__LINE__, __func__, Separator);
        log_printf(__LINE__, __func__, "<120>Switch Pico in upload mode.\n");
        log_printf(__LINE__, __func__, Separator);
        log_printf(__LINE__, __func__, "Press <G> to toggle Pico in upload mode or <Enter> to return to menu: ");
        input_string(String, 1, 0l);
        if ((String[0] == 'G') || (String[0] == 'g'))
        {
          log_printf(__LINE__, __func__, "Toggling the Pico in upload mode...\n");
          log_printf(__LINE__, __func__, Separator);
          LCD_clear(BLACK);
          LCD_refresh();
          sleep_ms(2000);
          reset_usb_boot(0l, 0l);
          sleep_ms(1000);
        }
        else
        {
          /* User didn't pres <G>. */
          log_printf(__LINE__, __func__, "User didn't press <G> Pico will not be toggled in upload mode.\n");
          log_printf(__LINE__, __func__, Separator);
          sleep_ms(1000);
        }
      break;

      default:
        log_printf(__LINE__, __func__, "          Invalid choice... please re-enter [%s]  [%u]\n\n\n\n\n", __LINE__, __func__, String, Menu);
        printf("\n\n");
      break;
    }
  }
}





/* $PAGE */
/* $TITLE=test_lcd_clear() */
/* ============================================================================================================================================================= *\
                                                           Test function LCD_clear() with different colors.
\* ============================================================================================================================================================= */
void test_lcd_clear(void)
{
  UCHAR ColorName[35];

  UINT16 Loop1UInt16;


  log_printf(__LINE__, __func__, Separator);
  log_printf(__LINE__, __func__, "<120>Testing function LCD_clear()\n");
  log_printf(__LINE__, __func__, "<120>The LCD_clear() function erases the content of the framebuffer and sets the background color.\n");
  log_printf(__LINE__, __func__, "<120>A call to LCD_refresh() must be done afterward to transfer the (cleared) content of the framebuffer to LCD display.\n");
  log_printf(__LINE__, __func__, Separator);
  LCD_clear(WHITE);
  LCD_refresh();

  /* Testing LCD_clear() with first standard defined colors. */
  for (Loop1UInt16 = 1; Loop1UInt16 < 8; ++Loop1UInt16)
  {
    LCD_color_name(StructLCD.Color[Loop1UInt16], ColorName);
    log_printf(__LINE__, __func__, "Press <Enter> to proceed with LCD_clear(%s) or press <ESC> to return to menu: ", ColorName);
    if (stop_flow(TYPE_NONE)) return;
    LCD_clear(StructLCD.Color[Loop1UInt16]);
    LCD_refresh();
  }
  log_printf(__LINE__, __func__, " \n");

  log_printf(__LINE__, __func__, "End of LCD_clear() function tests.\n");
  log_printf(__LINE__, __func__, Separator);
  if (stop_flow(TYPE_CONTINUE)) return;
  if (FlagAllTests) display_LCD_presentation();

  return;
}





/* $PAGE */
/* $TITLE=test_lcd_cls() */
/* ============================================================================================================================================================= *\
                                                           Test function LCD_cls() with different colors.
\* ============================================================================================================================================================= */
void test_lcd_cls(void)
{
  UCHAR ColorName[32];

  UINT16 Loop1UInt16;


  log_printf(__LINE__, __func__, Separator);
  log_printf(__LINE__, __func__, "<120>Testing function LCD_cls()\n");
  log_printf(__LINE__, __func__, "<120>The LCD_cls() function blanks the LCD display with the specified background color,\n");
  log_printf(__LINE__, __func__, "<120> without modifying the content of the framebuffer.\n");
  log_printf(__LINE__, __func__, "<120>If a LCD_refresh() is done afterward, the content of the original framebuffer will be restored on LCD display.\n");
  log_printf(__LINE__, __func__, Separator);
  LCD_clear(WHITE);
  LCD_refresh();


  /* Print text on LCD display to show that framebuffer is unchanged by function LCD_cls(). */
  log_printf(__LINE__, __func__, "Some text has been displayed on LCD display before proceeding with LCD_cls() tests.\n");
  fill_text(20);
  log_printf(__LINE__, __func__, " \n");

  /* Testing LCD_cls() with first standard defined colors. */
  for (Loop1UInt16 = 1; Loop1UInt16 < 8; ++Loop1UInt16)
  {
    LCD_color_name(StructLCD.Color[Loop1UInt16], ColorName);
    log_printf(__LINE__, __func__, "Press <Enter> to proceed with LCD_cls(%s) or press <ESC> to return to menu: ", ColorName);
    if (stop_flow(TYPE_NONE)) return;
    LCD_cls(StructLCD.Color[Loop1UInt16]);
  }
  log_printf(__LINE__, __func__, " \n");


  log_printf(__LINE__, __func__, "Press <Enter> to proceed with LCD_refresh() to restore the original framebuffer content and color: ");
  if (stop_flow(TYPE_NONE)) return;
  LCD_refresh();
  log_printf(__LINE__, __func__, " \n");

  log_printf(__LINE__, __func__, "End of LCD_cls() function tests.\n");
  log_printf(__LINE__, __func__, Separator);
  if (stop_flow(TYPE_CONTINUE)) return;
  if (FlagAllTests) display_LCD_presentation();

  return;
}





/* $PAGE */
/* $TITLE=test_lcd_display_point() */
/* ============================================================================================================================================================= *\
                                                                Test function LCD_display_point().
     NOTE: This function bypasses the framebuffer and works directly on LCD display. For this reason, rotation mode is not accounted for and we must translate
           the target coordinates corresponding to current rotation mode.
           In this test, we don't care about the origin point coordinates and the direction taken to draw the line, as long as the final line corresponds
           to what we want. If ever those items are important (original point and direction), user will have to review its algorithm.
\* ============================================================================================================================================================= */
void test_lcd_display_point(void)
{
  UINT16 LineNumber;
  UINT16 ColumnNumber;


  log_printf(__LINE__, __func__, Separator);
  log_printf(__LINE__, __func__, "<120>Testing function LCD_display_point()\n");
  log_printf(__LINE__, __func__, "<120>The LCD_display_point() function sets specific pixels on LCD display.\n");
  log_printf(__LINE__, __func__, "<120>In a way similar to LCD_cls(), LCD_display_point() bypasses the framebuffer and sets pixels directly on LCD display.\n");
  log_printf(__LINE__, __func__, "<120>If a LCD_refresh() is done afterward, content of the original framebuffer will be restored to the display.\n");
  log_printf(__LINE__, __func__, "<120>NOTE: Since LCD_display_point() bypasses the framebuffer, rotation has to be emulated by Firmware.\n");
  log_printf(__LINE__, __func__, Separator);
  LCD_clear(WHITE);
  LCD_refresh();


  /* Print text on LCD display to show that framebuffer is unchanged by function LCD_display_point(). */
  fill_text(20);
  log_printf(__LINE__, __func__, "Some text has been displayed on LCD display before proceeding with LCD_display_point() tests.\n");
  log_printf(__LINE__, __func__, " \n");


  /* Display a few lines at the top of display. */
  for (LineNumber = 5; LineNumber < 60; LineNumber += 5)
  {
    log_printf(__LINE__, __func__, "LCD_display_point() on line number %2u. Press <Enter> to proceed: ", LineNumber);
    if (stop_flow(TYPE_NONE)) return;

    for (ColumnNumber = (StructLCD.Width / 2) - 50; ColumnNumber < (StructLCD.Width / 2) + 50; ++ColumnNumber)
      LCD_display_point(LineNumber, ColumnNumber, BLACK);
  }
  log_printf(__LINE__, __func__, " \n");
  log_printf(__LINE__, __func__, " \n");


  /* Display a few lines at the bottom of display. */
  for (LineNumber = StructLCD.Height - 5; LineNumber > StructLCD.Height - 60; LineNumber -= 5)
  {
    log_printf(__LINE__, __func__, "LCD_display_point() on line number %2u. Press <Enter> to proceed: ", LineNumber);
    if (stop_flow(TYPE_NONE)) return;

    for (ColumnNumber = (StructLCD.Width / 2) - 50; ColumnNumber < (StructLCD.Width / 2) + 50; ++ColumnNumber)
      LCD_display_point(LineNumber, ColumnNumber, BLACK);
  }
  log_printf(__LINE__, __func__, " \n");
  log_printf(__LINE__, __func__, " \n");



  /* Display a few columns at the left of display. */
  for (ColumnNumber = 5; ColumnNumber < 60; ColumnNumber += 5)
  {
    log_printf(__LINE__, __func__, "LCD_display_point() on column number %2u. Press <Enter> to proceed: ", ColumnNumber);
    if (stop_flow(TYPE_NONE)) return;

    for (LineNumber = (StructLCD.Height / 2) - 50; LineNumber < (StructLCD.Height / 2) + 50; ++LineNumber)
      LCD_display_point(LineNumber, ColumnNumber, BLACK);
  }
  log_printf(__LINE__, __func__, " \n");
  log_printf(__LINE__, __func__, " \n");



  /* Display a few columns at the right of display. */
  for (ColumnNumber = StructLCD.Width - 5; ColumnNumber > StructLCD.Width - 60; ColumnNumber -= 5)
  {
    log_printf(__LINE__, __func__, "LCD_display_point() on column number %2u. Press <Enter> to proceed: ", ColumnNumber);
    if (stop_flow(TYPE_NONE)) return;

    for (LineNumber = (StructLCD.Height / 2) - 50; LineNumber < (StructLCD.Height / 2) + 50; ++LineNumber)
      LCD_display_point(LineNumber, ColumnNumber, BLACK);
  }
  log_printf(__LINE__, __func__, " \n");
  log_printf(__LINE__, __func__, " \n");


  log_printf(__LINE__, __func__, "Now that LCD_display_point() tests are done, press <Enter> to restore original framebuffer: ");
  if (stop_flow(TYPE_NONE)) return;
  LCD_refresh();
  log_printf(__LINE__, __func__, " \n");


  log_printf(__LINE__, __func__, "End of LCD_display_point() function tests.\n");
  log_printf(__LINE__, __func__, Separator);
  if (stop_flow(TYPE_CONTINUE)) return;
  if (FlagAllTests) display_LCD_presentation();

  return;
}




/* $PAGE */
/* $TITLE=test_lcd_draw_box() */
/* ============================================================================================================================================================= *\
                                                                   Test function LCD_draw_box().
\* ============================================================================================================================================================= */
void test_lcd_draw_box(void)
{
  UINT16 ColumnStart;
  UINT16 ColumnEnd;
  UINT16 ColumnStep;
  UINT16 LineStart;
  UINT16 LineEnd;
  UINT16 LineStep;
  UINT16 Loop1UInt16;


  log_printf(__LINE__, __func__, Separator);
  log_printf(__LINE__, __func__, "<120>Testing function LCD_draw_box()\n");
  log_printf(__LINE__, __func__, "<120>(and for some tests, function LCD_draw_line() too)\n");
  log_printf(__LINE__, __func__, Separator);
  LCD_clear(BLACK);
  LCD_refresh();
  log_printf(__LINE__, __func__, "Press <Enter> to clear framebuffer and proceed with LCD_draw_box() demo: ");
  if (stop_flow(TYPE_NONE)) return;
  log_printf(__LINE__, __func__, " \n");
  LCD_clear(WHITE);
  LCD_refresh();


  LineStep   = StructLCD.Height / 10;
  ColumnStep = StructLCD.Width  / 10;
  for (Loop1UInt16 = 0; Loop1UInt16 < 10; ++Loop1UInt16)
  {
    LineStart   = Loop1UInt16 * LineStep;
    ColumnStart = Loop1UInt16 * ColumnStep;
    LineEnd     = LineStart   + LineStep   - 1;
    ColumnEnd   = ColumnStart + ColumnStep - 1;

    log_printf(__LINE__, __func__, "LCD_draw_box() from (%u,%u) to (%u,%u) with blue border 3X3. Press <Enter> to proceed: ", LineStart, ColumnStart, LineEnd, ColumnEnd);
    if (stop_flow(TYPE_NONE)) return;
    LCD_clear(WHITE);
    LCD_draw_box(LineStart, ColumnStart, LineEnd, ColumnEnd, BLUE, DOT_PIXEL_3X3, DRAW_FILL_EMPTY);
    LCD_refresh();
  }
  if (stop_flow(TYPE_CONTINUE)) return;
  LCD_clear(BLACK);
  LCD_refresh();
  log_printf(__LINE__, __func__, " \n");
  log_printf(__LINE__, __func__, " \n");
  log_printf(__LINE__, __func__, "Press <Enter> to clear framebuffer and continue with LCD_draw_box() demo: ");
  if (stop_flow(TYPE_NONE)) return;
  log_printf(__LINE__, __func__, " \n");
  log_printf(__LINE__, __func__, " \n");



  for (Loop1UInt16 = 0; Loop1UInt16 < 10; ++Loop1UInt16)
  {
    LineStart   = (9 - Loop1UInt16) * LineStep;
    ColumnStart = (9 - Loop1UInt16) * ColumnStep;
    LineEnd     = LineStart   + LineStep   - 1;
    ColumnEnd   = ColumnStart + ColumnStep - 1;

    log_printf(__LINE__, __func__, "LCD_draw_box() from (%u,%u) to (%u,%u) with red border 1X1 and fill the box. Press <Enter> to proceed: ", LineStart, ColumnStart, LineEnd, ColumnEnd);
    if (stop_flow(TYPE_NONE)) return;
    LCD_clear(WHITE);
    LCD_draw_box(LineStart, ColumnStart, LineEnd, ColumnEnd, RED, DOT_PIXEL_1X1, DRAW_FILL_FULL);
    LCD_refresh();
  }
  if (stop_flow(TYPE_CONTINUE)) return;
  LCD_clear(BLACK);
  LCD_refresh();
  log_printf(__LINE__, __func__, " \n");
  log_printf(__LINE__, __func__, " \n");
  log_printf(__LINE__, __func__, "Press <Enter> to clear framebuffer and continue with LCD_draw_box() demo: ");
  if (stop_flow(TYPE_NONE)) return;
  log_printf(__LINE__, __func__, " \n");
  log_printf(__LINE__, __func__, " \n");



  LineStep   = StructLCD.Height / 5;
  ColumnStep = StructLCD.Width  / 5;
  for (Loop1UInt16 = 0; Loop1UInt16 < 5; ++Loop1UInt16)
  {
    LineStart   = Loop1UInt16 * LineStep;
    ColumnStart = Loop1UInt16 * ColumnStep;
    LineEnd     = LineStart   + LineStep   - 1;
    ColumnEnd   = ColumnStart + ColumnStep - 1;

    log_printf(__LINE__, __func__, "LCD_draw_box() from (%u,%u) to (%u,%u) with green border 4X4. Press <Enter> to proceed: ", LineStart, ColumnStart, LineEnd, ColumnEnd);
    if (stop_flow(TYPE_NONE)) return;
    LCD_clear(WHITE);
    LCD_draw_box(LineStart, ColumnStart, LineEnd, ColumnEnd, GREEN, DOT_PIXEL_4X4, DRAW_FILL_EMPTY);
    LCD_refresh();
  }
  if (stop_flow(TYPE_CONTINUE)) return;
  LCD_clear(BLACK);
  LCD_refresh();
  log_printf(__LINE__, __func__, " \n");
  log_printf(__LINE__, __func__, " \n");
  log_printf(__LINE__, __func__, "Press <Enter> to clear framebuffer and continue with LCD_draw_box() demo: ");
  if (stop_flow(TYPE_NONE)) return;
  log_printf(__LINE__, __func__, " \n");
  log_printf(__LINE__, __func__, " \n");



  for (Loop1UInt16 = 0; Loop1UInt16 < 5; ++Loop1UInt16)
  {
    LineStart   = (4 - Loop1UInt16) * LineStep;
    ColumnStart = (4 - Loop1UInt16) * ColumnStep;
    LineEnd     = LineStart   + LineStep   - 1;
    ColumnEnd   = ColumnStart + ColumnStep - 1;

    log_printf(__LINE__, __func__, "LCD_draw_box() from (%u,%u) to (%u,%u) with magenta border 2X2 and blue lines inside. Press <Enter> to proceed: ", LineStart, ColumnStart, LineEnd, ColumnEnd);
    if (stop_flow(TYPE_NONE)) return;
    LCD_clear(WHITE);
    LCD_draw_box(LineStart,  ColumnStart, LineEnd,   ColumnEnd, MAGENTA, DOT_PIXEL_2X2, DRAW_FILL_EMPTY);
    LCD_draw_line(LineStart, ColumnStart, LineEnd,   ColumnEnd, BLUE, DOT_PIXEL_2X2, LINE_STYLE_SOLID);
    LCD_draw_line(LineEnd,   ColumnStart, LineStart, ColumnEnd, BLUE, DOT_PIXEL_2X2, LINE_STYLE_SOLID);
    LCD_refresh();
  }
  if (stop_flow(TYPE_CONTINUE)) return;
  LCD_clear(BLACK);
  LCD_refresh();
  log_printf(__LINE__, __func__, " \n");
  log_printf(__LINE__, __func__, " \n");
  log_printf(__LINE__, __func__, "Press <Enter> to clear framebuffer and continue with LCD_draw_box() demo: ");
  if (stop_flow(TYPE_NONE)) return;
  log_printf(__LINE__, __func__, " \n");
  log_printf(__LINE__, __func__, " \n");



  LineStep   = StructLCD.Height / 15;
  ColumnStep = StructLCD.Width  / 15;
  for (Loop1UInt16 = 0; Loop1UInt16 < 10; ++Loop1UInt16)
  {
    LineStart   = Loop1UInt16 * LineStep;
    ColumnStart = Loop1UInt16 * ColumnStep;
    LineEnd     = StructLCD.Height  - (Loop1UInt16 * LineStep)   - 1;
    ColumnEnd   = StructLCD.Width - (Loop1UInt16 * ColumnStep) - 1;

    log_printf(__LINE__, __func__, "LCD_draw_box() from (%u,%u) to (%u,%u) with red border 3X3. Press <Enter> to proceed: ", LineStart, ColumnStart, LineEnd, ColumnEnd);
    if (stop_flow(TYPE_NONE)) return;
    LCD_clear(WHITE);
    LCD_draw_box(LineStart, ColumnStart, LineEnd, ColumnEnd, RED, DOT_PIXEL_3X3, DRAW_FILL_EMPTY);
    LCD_refresh();
  }
  if (stop_flow(TYPE_CONTINUE)) return;
  LCD_clear(BLACK);
  LCD_refresh();
  log_printf(__LINE__, __func__, " \n");

  log_printf(__LINE__, __func__, "End of LCD_draw_box() function tests.\n");
  log_printf(__LINE__, __func__, Separator);
  if (stop_flow(TYPE_CONTINUE)) return;
  if (FlagAllTests) display_LCD_presentation();

  return;
}





/* $PAGE */
/* $TITLE=test_lcd_draw_char() */
/* ============================================================================================================================================================= *\
                                                                   Test function LCD_draw_char().
                        NOTE: Code has been left, but function test_lcd_printf() should be used instead (it is a more "high-level" function).
\* ============================================================================================================================================================= */
void test_lcd_draw_char(void)
{
  UINT16 LineStart;
  UINT16 ColumnStart;


  log_printf(__LINE__, __func__, Separator);
  log_printf(__LINE__, __func__, "<120>Testing function LCD_draw_char()\n");
  log_printf(__LINE__, __func__, Separator);

  printf("\n\n\n");
  log_printf(__LINE__, __func__, "Press <Enter> to clear LCD display before proceeding to next tests: ");
  if (stop_flow(TYPE_NONE)) return;
  LCD_clear(WHITE);
  LCD_refresh();

  for (LineStart = 0; LineStart < 6; ++LineStart)
  {
    log_printf(__LINE__, __func__, "LCD_draw_char() on line number %u\n", LineStart);
    log_printf(__LINE__, __func__, "Press <Enter> to proceed: ");
    if (stop_flow(TYPE_PROCEED)) return;
    LCD_clear(WHITE);
    LCD_refresh();

    LCD_draw_char(LineStart,   0, 'A',  &Font24, RED, WHITE);
    LCD_draw_char(LineStart,  25, 'B',  &Font24, RED, WHITE);
    LCD_draw_char(LineStart,  50, 'C',  &Font24, RED, WHITE);
    LCD_draw_char(LineStart,  75, 'D',  &Font24, RED, WHITE);
    LCD_draw_char(LineStart, 100, 'E',  &Font24, RED, WHITE);

    LCD_draw_line(LineStart,     115, LineStart,     145, RED, DOT_PIXEL_1X1, LINE_STYLE_SOLID);  // draw reference lines.
    LCD_draw_line(LineStart + 1, 115, LineStart + 1, 145, RED, DOT_PIXEL_1X1, LINE_STYLE_SOLID);  // draw reference lines.

    LCD_draw_line(LineStart + 24, 115, LineStart + 24, 145, RED, DOT_PIXEL_1X1, LINE_STYLE_SOLID);  // draw reference lines.
    LCD_refresh();
  }

  for (LineStart = StructLCD.Height - 25; LineStart > StructLCD.Height - 30; --LineStart)
  {
    log_printf(__LINE__, __func__, "LCD_draw_char() on line number %u\n", LineStart);
    if (stop_flow(TYPE_PROCEED)) return;
    LCD_clear(WHITE);
    LCD_refresh();

    LCD_draw_char(LineStart,   0, 'A', &Font24, RED, WHITE);
    LCD_draw_char(LineStart,  25, 'B', &Font24, RED, WHITE);
    LCD_draw_char(LineStart,  50, 'C', &Font24, RED, WHITE);
    LCD_draw_char(LineStart,  75, 'D', &Font24, RED, WHITE);
    LCD_draw_char(LineStart, 100, 'E', &Font24, RED, WHITE);

    LCD_draw_line(LineStart, 115, LineStart, 145, RED, DOT_PIXEL_1X1, LINE_STYLE_SOLID);  // draw reference lines.

    LCD_draw_line(LineStart + 23, 115, LineStart + 23, 145, RED, DOT_PIXEL_1X1, LINE_STYLE_SOLID);  // draw reference lines.
    LCD_draw_line(LineStart + 24, 115, LineStart + 24, 145, RED, DOT_PIXEL_1X1, LINE_STYLE_SOLID);  // draw reference lines.
    LCD_refresh();
  }



  printf("\n\n\n");
  log_printf(__LINE__, __func__, "Press <Enter> to clear LCD display before proceeding to next tests: ");
  if (stop_flow(TYPE_NONE)) return;
  LCD_clear(WHITE);
  LCD_refresh();

  for (ColumnStart = 0; ColumnStart < 6; ++ColumnStart)
  {
    log_printf(__LINE__, __func__, "LCD_draw_char() on column number %u\n", ColumnStart);
    if (stop_flow(TYPE_PROCEED)) return;
    LCD_clear(WHITE);
    LCD_refresh();

    LCD_draw_char(  0, ColumnStart, 'A', &Font24, RED, WHITE);
    LCD_draw_char( 30, ColumnStart, 'B', &Font24, RED, WHITE);
    LCD_draw_char( 60, ColumnStart, 'C', &Font24, RED, WHITE);
    LCD_draw_char( 90, ColumnStart, 'D', &Font24, RED, WHITE);
    LCD_draw_char(120, ColumnStart, 'E', &Font24, RED, WHITE);

    LCD_draw_line(150, ColumnStart,      175, ColumnStart,     RED, DOT_PIXEL_1X1, LINE_STYLE_SOLID);  // draw reference lines.
    LCD_draw_line(155, ColumnStart + 1,  180, ColumnStart + 1, RED, DOT_PIXEL_1X1, LINE_STYLE_SOLID);  // draw reference lines.
    LCD_draw_line(155, ColumnStart + 14, 175, ColumnStart + 14, RED, DOT_PIXEL_1X1, LINE_STYLE_SOLID);  // draw reference lines.
    LCD_refresh();
  }

  for (ColumnStart = StructLCD.Width - 16; ColumnStart > StructLCD.Width - 21; --ColumnStart)
  {
    log_printf(__LINE__, __func__, "LCD_draw_char() on column number %u\n", ColumnStart);
    if (stop_flow(TYPE_PROCEED)) return;
    LCD_clear(WHITE);
    LCD_refresh();

    LCD_draw_char(  0, ColumnStart, 'A', &Font24, RED, WHITE);
    LCD_draw_char( 30, ColumnStart, 'B', &Font24, RED, WHITE);
    LCD_draw_char( 60, ColumnStart, 'C', &Font24, RED, WHITE);
    LCD_draw_char( 90, ColumnStart, 'D', &Font24, RED, WHITE);
    LCD_draw_char(120, ColumnStart, 'E', &Font24, RED, WHITE);

    LCD_draw_line(150, ColumnStart,     175, ColumnStart,     RED, DOT_PIXEL_1X1, LINE_STYLE_SOLID);  // draw reference lines.
    LCD_draw_line(155, ColumnStart + 1, 180, ColumnStart + 1, RED, DOT_PIXEL_1X1, LINE_STYLE_SOLID);  // draw reference lines.

    LCD_draw_line(155, ColumnStart + 14, 175, ColumnStart + 14, RED, DOT_PIXEL_1X1, LINE_STYLE_SOLID);  // draw reference lines.
    LCD_refresh();
  }

  if (stop_flow(TYPE_CONTINUE)) return;
  LCD_clear(BLACK);
  LCD_refresh();
  log_printf(__LINE__, __func__, " \n");

  log_printf(__LINE__, __func__, "End of LCD_draw_char() function tests.\n");
  log_printf(__LINE__, __func__, Separator);
  if (stop_flow(TYPE_CONTINUE)) return;
  if (FlagAllTests) display_LCD_presentation();

  return;
}




/* $PAGE */
/* $TITLE=test_lcd_draw_circle() */
/* ============================================================================================================================================================= *\
                                                                 Test function LCD_draw_circle().
\* ============================================================================================================================================================= */
void test_lcd_draw_circle(void)
{
  INT16 LineStart;
  INT16 ColumnStart;

  UINT16 Loop1UInt16;
  UINT16 LineStep;
  UINT16 ColumnStep;
  UINT16 Radius;

  log_printf(__LINE__, __func__, Separator);
  log_printf(__LINE__, __func__, "<120>Testing function LCD_draw_circle()\n");
  log_printf(__LINE__, __func__, "<120>NOTE: Warning messages show that you can specify circle coordinates that exceed LCD displayable area.\n");
  log_printf(__LINE__, __func__, "<120>Part of the circle which is inside LCD displayable area will show up\n");
  log_printf(__LINE__, __func__, "<120>while remaining will be ignored (beside the warning message).\n");
  log_printf(__LINE__, __func__, Separator);


  LineStep    = StructLCD.Height / 10;
  ColumnStep  = StructLCD.Width  / 10;
  Radius      = StructLCD.Width  / 10;
  LCD_clear(BLACK);
  LCD_refresh();



  for (Loop1UInt16 = 0; Loop1UInt16 < 11; ++Loop1UInt16)
  {
    LineStart = Loop1UInt16 * LineStep - 1;
    if (LineStart < 0) LineStart = 0;

    ColumnStart = Loop1UInt16 * ColumnStep - 1;
    if (ColumnStart < 0) ColumnStart = 0;

    log_printf(__LINE__, __func__, "Draw circle with center at (%d,%d) and radius %u. Press <Enter> to proceed: ", LineStart, ColumnStart, Radius);
    if (stop_flow(TYPE_NONE)) return;

    LCD_clear(WHITE);
    LCD_refresh();
    LCD_draw_circle(LineStart, ColumnStart, Radius, BLUE, DOT_PIXEL_1X1, DRAW_FILL_FULL);
    LCD_refresh();
    log_printf(__LINE__, __func__, " \n");
  }
  log_printf(__LINE__, __func__, " \n");
  if (stop_flow(TYPE_CONTINUE)) return;
  log_printf(__LINE__, __func__, " \n");
  log_printf(__LINE__, __func__, " \n");
  LCD_clear(BLACK);
  LCD_refresh();



  for (Loop1UInt16 = 0; Loop1UInt16 < 11; ++Loop1UInt16)
  {
    LineStart   = (10 - Loop1UInt16) * LineStep;
    ColumnStart = (10 - Loop1UInt16) * ColumnStep;

    log_printf(__LINE__, __func__, "Draw circle with center at (%d,%d) and radius %u. Press <Enter> to proceed: ", LineStart, ColumnStart, Radius);
    if (stop_flow(TYPE_NONE)) return;

    LCD_clear(WHITE);
    LCD_refresh();
    LCD_draw_circle(LineStart, ColumnStart, Radius, RED, DOT_PIXEL_1X1, DRAW_FILL_EMPTY);
    LCD_refresh();
    log_printf(__LINE__, __func__, " \n");
  }
  log_printf(__LINE__, __func__, " \n");
  if (stop_flow(TYPE_CONTINUE)) return;
  log_printf(__LINE__, __func__, " \n");
  log_printf(__LINE__, __func__, " \n");
  LCD_clear(BLACK);
  LCD_refresh();



  LineStart   = StructLCD.Height / 2;
  ColumnStart = StructLCD.Width  / 2;
  for (Loop1UInt16 = 0; Loop1UInt16 < 10; ++Loop1UInt16)
  {
    if (StructLCD.Height < StructLCD.Width)
      Radius = Loop1UInt16 * (StructLCD.Height / 12);  // lanscape mode
    else
      Radius = Loop1UInt16 * (StructLCD.Width / 12);  // portrait mode

    log_printf(__LINE__, __func__, "Draw circle with center at (%d,%d) and radius %u. Press <Enter> to proceed: ", LineStart, ColumnStart, Radius);
    if (stop_flow(TYPE_NONE)) return;

    LCD_clear(WHITE);
    LCD_refresh();
    LCD_draw_circle(LineStart, ColumnStart, Radius, BLUE, DOT_PIXEL_2X2, DRAW_FILL_EMPTY);
    LCD_refresh();
    log_printf(__LINE__, __func__, " \n");
  }
  log_printf(__LINE__, __func__, " \n");
  if (stop_flow(TYPE_CONTINUE)) return;
  log_printf(__LINE__, __func__, " \n");
  LCD_clear(WHITE);
  LCD_refresh();

  log_printf(__LINE__, __func__, "End of LCD_draw_circle() function tests.\n");
  log_printf(__LINE__, __func__, Separator);
  if (stop_flow(TYPE_CONTINUE)) return;
  if (FlagAllTests) display_LCD_presentation();

  return;
}





/* $PAGE */
/* $TITLE=test_lcd_draw_image() */
/* ============================================================================================================================================================= *\
                                                                   Test function LCD_draw_image().
\* ============================================================================================================================================================= */
void test_lcd_draw_image(void)
{
  UINT16 ImageHeight;
  UINT16 ImageWidth;
  UINT16 LineStart;
  UINT16 ColumnStart;


  log_printf(__LINE__, __func__, Separator);
  log_printf(__LINE__, __func__, "<120>Testing function LCD_draw_image()\n");
  log_printf(__LINE__, __func__, "<120>The LCD_draw_image() function transfers an image bitmap to virtual framebuffer.\n");
  log_printf(__LINE__, __func__, Separator);

  log_printf(__LINE__, __func__, "Press <Enter> to clear framebuffer and begin LCD_draw_image() tests: ");
  if (stop_flow(TYPE_NONE)) return;
  log_printf(__LINE__, __func__, " \n");


  /* Display WaterFall image. */
  log_printf(__LINE__, __func__, "Press <Enter> to display WaterFall image (WaterFall.img): ");
  if (stop_flow(TYPE_NONE)) return;
  LCD_clear(BLACK);
  /* WaterFall image dimensions. */
  ImageHeight = 240;
  ImageWidth  = 240;
  /* Center image vertically. If image is higher than display height, begin at line 0 and chop bottom of image. */
  if (ImageHeight <= StructLCD.Height)
    LineStart = ((StructLCD.Height - ImageHeight) / 2);
  else
    LineStart = 0;
  /* Center image horizontally. If image is wider than display width, begin at column 0 and chop right side of image. */
  if (ImageWidth <= StructLCD.Width)
    ColumnStart = ((StructLCD.Width - ImageWidth) / 2);
  else
    ColumnStart = 0;
  LCD_draw_image(LineStart, ColumnStart, (UINT16 *)WaterFall, ImageHeight, ImageWidth);
  LCD_refresh();  // update LCD display.



  /* Display Raspberry Pi Pico logo image. */
  log_printf(__LINE__, __func__, "Press <Enter> to display Raspberry Pi Pico logo image (PicoLogo.img): ");
  if (stop_flow(TYPE_NONE)) return;
  LCD_clear(BLACK);
  /* PicoLogo image dimensions. */
  ImageHeight = 23;
  ImageWidth  = 140;
  /* Center image vertically. If image is higher than display height, begin at line 0 and chop bottom of image. */
  if (ImageHeight <= StructLCD.Height)
    LineStart = ((StructLCD.Height - ImageHeight) / 2);
  else
    LineStart = 0;
  /* Center image horizontally. If image is wider than display width, begin at column 0 and chop right side of image. */
  if (ImageWidth <= StructLCD.Width)
    ColumnStart = ((StructLCD.Width - ImageWidth) / 2);
  else
    ColumnStart = 0;
  LCD_draw_image(LineStart, ColumnStart, (UINT16 *)PicoLogo, ImageHeight, ImageWidth);
  LCD_refresh();  // update LCD display.



  /* Display Waveshare1 (logo and stylished name) image. */
  log_printf(__LINE__, __func__, "Press <Enter> to display Waveshare1 (logo and stylished name) image: ");
  if (stop_flow(TYPE_NONE)) return;
  LCD_clear(BLACK);
  /* Waveshare1 image dimensions. */
  ImageHeight =  88;
  ImageWidth  = 240;
  /* Center image vertically. If image is higher than display height, begin at line 0 and chop bottom of image. */
  if (ImageHeight <= StructLCD.Height)
    LineStart = ((StructLCD.Height - ImageHeight) / 2);
  else
    LineStart = 0;
  /* Center image horizontally. If image is wider than display width, begin at column 0 and chop right side of image. */
  if (ImageWidth <= StructLCD.Width)
    ColumnStart = ((StructLCD.Width - ImageWidth) / 2);
  else
    ColumnStart = 0;
  LCD_draw_image(LineStart, ColumnStart, (UINT16 *)Waveshare1, ImageHeight, ImageWidth);
  LCD_refresh();  // update LCD display.


  log_printf(__LINE__, __func__, " \n");
  log_printf(__LINE__, __func__, "End of LCD_draw_image() function tests.\n");
  log_printf(__LINE__, __func__, Separator);
  if (stop_flow(TYPE_CONTINUE)) return;
  if (FlagAllTests) display_LCD_presentation();

  return;
}





/* $PAGE */
/* $TITLE=test_lcd_draw_line() */
/* ============================================================================================================================================================= *\
                                                                 Test function LCD_draw_line().
\* ============================================================================================================================================================= */
void test_lcd_draw_line(void)
{
  UINT16 LineStart;
  UINT16 LineEnd;
  UINT16 ColumnStart;
  UINT16 ColumnEnd;

  log_printf(__LINE__, __func__, Separator);
  log_printf(__LINE__, __func__, "<120>Testing function LCD_draw_line()\n");
  log_printf(__LINE__, __func__, Separator);
  log_printf(__LINE__, __func__, "Press <Enter> to clear LCD display before proceeding to next tests: ");
  if (stop_flow(TYPE_NONE)) return;
  log_printf(__LINE__, __func__, " \n");
  LCD_clear(WHITE);
  LCD_refresh();



  ColumnStart = 0;
  ColumnEnd   = StructLCD.Width - 1;
  for (LineStart = 5; LineStart < 105; LineStart += 5)
  {
    log_printf(__LINE__, __func__, "LCD_draw_line() from (%u,%u) up to (%u,%u). Press <Enter> to proceed: ", LineStart, ColumnStart, LineStart, ColumnEnd);
    if (stop_flow(TYPE_NONE)) return;
    LCD_draw_line(LineStart, ColumnStart, LineStart, ColumnEnd, RED, DOT_PIXEL_2X2, LINE_STYLE_SOLID);
    LCD_refresh();
    ColumnStart += 5;
    ColumnEnd   -= 5;
  }
  log_printf(__LINE__, __func__, " \n");
  log_printf(__LINE__, __func__, " \n");



  ColumnStart = 0;
  ColumnEnd   = StructLCD.Width - 1;
  for (LineStart = StructLCD.Height - 6; LineStart > StructLCD.Height - 105; LineStart -= 5)
  {
    log_printf(__LINE__, __func__, "LCD_draw_line() from (%u,%u) up to (%u,%u). Press <Enter> to proceed: ", LineStart, ColumnStart, LineStart, ColumnEnd);
    if (stop_flow(TYPE_NONE)) return;
    LCD_draw_line(LineStart, ColumnStart, LineStart, ColumnEnd, RED, DOT_PIXEL_2X2, LINE_STYLE_SOLID);
    LCD_refresh();
    ColumnStart += 5;
    ColumnEnd   -= 5;
  }
  log_printf(__LINE__, __func__, " \n");
  log_printf(__LINE__, __func__, " \n");



  LineStart = 0;
  LineEnd   = StructLCD.Height - 1;
  for (ColumnStart = 5; ColumnStart < 100; ColumnStart += 5)
  {
    log_printf(__LINE__, __func__, "LCD_draw_line() from (%u,%u) up to (%u,%u). Press <Enter> to proceed: ", LineStart, ColumnStart, LineEnd, ColumnStart);
    if (stop_flow(TYPE_NONE)) return;
    LCD_draw_line(LineStart, ColumnStart, LineEnd, ColumnStart, GREEN, DOT_PIXEL_3X3, LINE_STYLE_SOLID);
    LCD_refresh();
    LineStart += 5;
    LineEnd   -= 5;
  }
  log_printf(__LINE__, __func__, " \n");
  log_printf(__LINE__, __func__, " \n");



  LineStart = 0;
  LineEnd   = StructLCD.Height - 1;
  for (ColumnStart = StructLCD.Width - 6; ColumnStart > StructLCD.Width - 100; ColumnStart -= 5)
  {
    log_printf(__LINE__, __func__, "LCD_draw_line() from (%u,%u) up to (%u,%u). Press <Enter> to proceed: ", LineStart, ColumnStart, LineEnd, ColumnStart);
    if (stop_flow(TYPE_NONE)) return;
    LCD_draw_line(LineStart, ColumnStart, LineEnd, ColumnStart, GREEN, DOT_PIXEL_3X3, LINE_STYLE_SOLID);
    LCD_refresh();
    LineStart += 5;
    LineEnd   -= 5;
  }
  log_printf(__LINE__, __func__, " \n");

  log_printf(__LINE__, __func__, "End of LCD_draw_line() function tests.\n");
  log_printf(__LINE__, __func__, Separator);
  if (stop_flow(TYPE_CONTINUE)) return;
  if (FlagAllTests) display_LCD_presentation();

  return;
}





/* $PAGE */
/* $TITLE=test_lcd_draw_point() */
/* ============================================================================================================================================================= *\
                                                                  Test function LCD_draw_point().
\* ============================================================================================================================================================= */
void test_lcd_draw_point(void)
{
  UINT16 Loop1UInt16;
  UINT16 ColumnStart;
  UINT16 LineStart;


  log_printf(__LINE__, __func__, Separator);
  log_printf(__LINE__, __func__, "<120>Testing function LCD_draw_point()\n");
  log_printf(__LINE__, __func__, Separator);
  log_printf(__LINE__, __func__, "Press <Enter> to clear framebuffer and proceed with LCD_draw_point() tests: ");
  if (stop_flow(TYPE_NONE)) return;
  log_printf(__LINE__, __func__, " \n");
  LCD_clear(BLACK);
  LCD_refresh();

  log_printf(__LINE__, __func__, "Press <Enter> to clear LCD display before proceeding to next tests: ");
  if (stop_flow(TYPE_NONE)) return;
  log_printf(__LINE__, __func__, " \n");
  LCD_clear(WHITE);
  LCD_refresh();



  /* Draw many points on the same line separated by spaces. */
  for (LineStart = 5; LineStart < 50; LineStart += 5)
  {
    log_printf(__LINE__, __func__, "LCD_draw_point() on line number %2u (to draw a train of many points 2X2). Press <Enter> to proceed: ", LineStart);
    if (stop_flow(TYPE_NONE)) return;

    for (Loop1UInt16 = 0; Loop1UInt16 < StructLCD.Width - 10; ++Loop1UInt16)
    {
      LCD_draw_point(LineStart, (Loop1UInt16 * 20) + 1, RED, DOT_PIXEL_2X2, DOT_FILL_RIGHTUP);
      LCD_draw_point(LineStart, (Loop1UInt16 * 20) + 2, RED, DOT_PIXEL_2X2, DOT_FILL_RIGHTUP);
      LCD_draw_point(LineStart, (Loop1UInt16 * 20) + 3, RED, DOT_PIXEL_2X2, DOT_FILL_RIGHTUP);
      LCD_draw_point(LineStart, (Loop1UInt16 * 20) + 4, RED, DOT_PIXEL_2X2, DOT_FILL_RIGHTUP);
      LCD_draw_point(LineStart, (Loop1UInt16 * 20) + 5, RED, DOT_PIXEL_2X2, DOT_FILL_RIGHTUP);
      LCD_draw_point(LineStart, (Loop1UInt16 * 20) + 6, RED, DOT_PIXEL_2X2, DOT_FILL_RIGHTUP);
      LCD_draw_point(LineStart, (Loop1UInt16 * 20) + 7, RED, DOT_PIXEL_2X2, DOT_FILL_RIGHTUP);
      LCD_draw_point(LineStart, (Loop1UInt16 * 20) + 8, RED, DOT_PIXEL_2X2, DOT_FILL_RIGHTUP);
      LCD_draw_point(LineStart, (Loop1UInt16 * 20) + 9, RED, DOT_PIXEL_2X2, DOT_FILL_RIGHTUP);
    }
    LCD_refresh();
  }
  log_printf(__LINE__, __func__, " \n");
  log_printf(__LINE__, __func__, " \n");



  /* Draw many points on the same line separated by spaces. */
  for (LineStart = StructLCD.Height - 6; LineStart > StructLCD.Height - 50; LineStart -= 5)
  {
    log_printf(__LINE__, __func__, "LCD_draw_point() on line number %u (to draw a train of many points 2X2). Press <Enter> to proceed: ", LineStart);
    if (stop_flow(TYPE_NONE)) return;

    for (Loop1UInt16 = 0; Loop1UInt16 < StructLCD.Width - 10; ++Loop1UInt16)
    {
      LCD_draw_point(LineStart, (Loop1UInt16 * 20) + 1, BLUE, DOT_PIXEL_2X2, DOT_FILL_RIGHTUP);
      LCD_draw_point(LineStart, (Loop1UInt16 * 20) + 2, BLUE, DOT_PIXEL_2X2, DOT_FILL_RIGHTUP);
      LCD_draw_point(LineStart, (Loop1UInt16 * 20) + 3, BLUE, DOT_PIXEL_2X2, DOT_FILL_RIGHTUP);
      LCD_draw_point(LineStart, (Loop1UInt16 * 20) + 4, BLUE, DOT_PIXEL_2X2, DOT_FILL_RIGHTUP);
      LCD_draw_point(LineStart, (Loop1UInt16 * 20) + 5, BLUE, DOT_PIXEL_2X2, DOT_FILL_RIGHTUP);
      LCD_draw_point(LineStart, (Loop1UInt16 * 20) + 6, BLUE, DOT_PIXEL_2X2, DOT_FILL_RIGHTUP);
      LCD_draw_point(LineStart, (Loop1UInt16 * 20) + 7, BLUE, DOT_PIXEL_2X2, DOT_FILL_RIGHTUP);
      LCD_draw_point(LineStart, (Loop1UInt16 * 20) + 8, BLUE, DOT_PIXEL_2X2, DOT_FILL_RIGHTUP);
      LCD_draw_point(LineStart, (Loop1UInt16 * 20) + 9, BLUE, DOT_PIXEL_2X2, DOT_FILL_RIGHTUP);
    }
    LCD_refresh();
  }
  log_printf(__LINE__, __func__, " \n");
  log_printf(__LINE__, __func__, " \n");



  /* Draw many points on the same column separated by spaces. */
  for (ColumnStart = 5; ColumnStart < 50; ColumnStart += 5)
  {
    log_printf(__LINE__, __func__, "LCD_draw_point() on column number %2u (to draw a train of many points 1X1). Press <Enter> to proceed: ", ColumnStart);
    if (stop_flow(TYPE_NONE)) return;

    for (Loop1UInt16 = 0; Loop1UInt16 < StructLCD.Height - 10; ++Loop1UInt16)
    {
      LCD_draw_point((Loop1UInt16 * 20) + 1, ColumnStart, RED, DOT_PIXEL_1X1, DOT_FILL_RIGHTUP);
      LCD_draw_point((Loop1UInt16 * 20) + 2, ColumnStart, RED, DOT_PIXEL_1X1, DOT_FILL_RIGHTUP);
      LCD_draw_point((Loop1UInt16 * 20) + 3, ColumnStart, RED, DOT_PIXEL_1X1, DOT_FILL_RIGHTUP);
      LCD_draw_point((Loop1UInt16 * 20) + 4, ColumnStart, RED, DOT_PIXEL_1X1, DOT_FILL_RIGHTUP);
      LCD_draw_point((Loop1UInt16 * 20) + 5, ColumnStart, RED, DOT_PIXEL_1X1, DOT_FILL_RIGHTUP);
      LCD_draw_point((Loop1UInt16 * 20) + 6, ColumnStart, RED, DOT_PIXEL_1X1, DOT_FILL_RIGHTUP);
      LCD_draw_point((Loop1UInt16 * 20) + 7, ColumnStart, RED, DOT_PIXEL_1X1, DOT_FILL_RIGHTUP);
      LCD_draw_point((Loop1UInt16 * 20) + 8, ColumnStart, RED, DOT_PIXEL_1X1, DOT_FILL_RIGHTUP);
      LCD_draw_point((Loop1UInt16 * 20) + 9, ColumnStart, RED, DOT_PIXEL_1X1, DOT_FILL_RIGHTUP);
    }
    LCD_refresh();
  }
  log_printf(__LINE__, __func__, " \n");
  log_printf(__LINE__, __func__, " \n");

  
  
  /* Draw many points on the same column separated by spaces. */
  for (ColumnStart = StructLCD.Width - 6; ColumnStart > StructLCD.Width - 50; ColumnStart -= 5)
  {
    log_printf(__LINE__, __func__, "LCD_draw_point() on column number %u (to draw a train of many points 1X1). Press <Enter> to proceed: ", ColumnStart);
    if (stop_flow(TYPE_NONE)) return;

    for (Loop1UInt16 = 0; Loop1UInt16 < StructLCD.Height - 10; ++Loop1UInt16)
    {
      LCD_draw_point((Loop1UInt16 * 20) + 1, ColumnStart, RED, DOT_PIXEL_1X1, DOT_FILL_RIGHTUP);
      LCD_draw_point((Loop1UInt16 * 20) + 2, ColumnStart, RED, DOT_PIXEL_1X1, DOT_FILL_RIGHTUP);
      LCD_draw_point((Loop1UInt16 * 20) + 3, ColumnStart, RED, DOT_PIXEL_1X1, DOT_FILL_RIGHTUP);
      LCD_draw_point((Loop1UInt16 * 20) + 4, ColumnStart, RED, DOT_PIXEL_1X1, DOT_FILL_RIGHTUP);
      LCD_draw_point((Loop1UInt16 * 20) + 5, ColumnStart, RED, DOT_PIXEL_1X1, DOT_FILL_RIGHTUP);
      LCD_draw_point((Loop1UInt16 * 20) + 6, ColumnStart, RED, DOT_PIXEL_1X1, DOT_FILL_RIGHTUP);
      LCD_draw_point((Loop1UInt16 * 20) + 7, ColumnStart, RED, DOT_PIXEL_1X1, DOT_FILL_RIGHTUP);
      LCD_draw_point((Loop1UInt16 * 20) + 8, ColumnStart, RED, DOT_PIXEL_1X1, DOT_FILL_RIGHTUP);
      LCD_draw_point((Loop1UInt16 * 20) + 9, ColumnStart, RED, DOT_PIXEL_1X1, DOT_FILL_RIGHTUP);
    }
    LCD_refresh();
  }
  log_printf(__LINE__, __func__, " \n");

  log_printf(__LINE__, __func__, "End of LCD_draw_point() function tests.\n");
  log_printf(__LINE__, __func__, Separator);
  if (stop_flow(TYPE_CONTINUE)) return;
  if (FlagAllTests) display_LCD_presentation();

  return;
}





/* $PAGE */
/* $TITLE=test_lcd_part_clear() */
/* ============================================================================================================================================================= *\
                                                                  Test function LCD_part_clear().
\* ============================================================================================================================================================= */
void test_lcd_part_clear(void)
{
  UINT16 LineStart;
  UINT16 ColumnStart;
  UINT16 LineEnd;
  UINT16 ColumnEnd;


  log_printf(__LINE__, __func__, Separator);
  log_printf(__LINE__, __func__, "<120>Testing function LCD_part_clear()\n");
  log_printf(__LINE__, __func__, "<120>LCD_part_clear() erases the framebuffer area defined by the start and end coordinates and sets the color as specified.\n");
  log_printf(__LINE__, __func__, "<120>It is required to trigger a LCD_refresh() to update LCD display with the content of the framebuffer.\n");
  log_printf(__LINE__, __func__, Separator);
  log_printf(__LINE__, __func__, "Press <Enter> to clear framebuffer and proceed with LCD_part_clear() tests: ");
  if (stop_flow(TYPE_NONE)) return;
  log_printf(__LINE__, __func__, " \n");
  LCD_clear(BLACK);
  LCD_refresh();

  log_printf(__LINE__, __func__, "Press <Enter> to fill LCD display with text before testing LCD_part_clear() function: ");
  if (stop_flow(TYPE_NONE)) return;
  log_printf(__LINE__, __func__, " \n");
  fill_text(20);



  LineStart   = 25;
  ColumnStart = 30;
  LineEnd     = 90;
  ColumnEnd   = 100;
  log_printf(__LINE__, __func__, "NOTE: Area to be cleared may be located anywhere on LCD display.\n");
  log_printf(__LINE__, __func__, "Press <Enter> to clear display from (%u,%u) up to (%u,%u): ", LineStart, ColumnStart, LineEnd, ColumnEnd);
  if (stop_flow(TYPE_NONE)) return;
  LCD_part_clear(LineStart, ColumnStart, LineEnd, ColumnEnd, WHITE);
  LCD_refresh();



  LineStart   = 150;
  ColumnStart = 100;
  LineEnd     = 200;
  ColumnEnd   = StructLCD.Width - 1;
  log_printf(__LINE__, __func__, "Press <Enter> to clear display from (%u,%u) up to (%u,%u): ", LineStart, ColumnStart, LineEnd, ColumnEnd);
  if (stop_flow(TYPE_NONE)) return;
  LCD_part_clear(LineStart, ColumnStart, LineEnd, ColumnEnd, WHITE);
  LCD_refresh();

  log_printf(__LINE__, __func__, " \n");
  log_printf(__LINE__, __func__, "End of LCD_part_clear() function tests.\n");
  log_printf(__LINE__, __func__, Separator);
  if (stop_flow(TYPE_CONTINUE)) return;
  if (FlagAllTests) display_LCD_presentation();

  return;
}





/* $PAGE */
/* $TITLE=test_lcd_part_cls() */
/* ============================================================================================================================================================= *\
                                                                  Test function LCD_part_cls().
   NOTE: Function LCD_part_cls(), as its couterpart LCD_cls(), works directly on LCD display and does not change the framebuffer. That means coordinates must
         be translated and that also means that an eventual LCD_refresh() would restore the original framebuffer content.
\* ============================================================================================================================================================= */
void test_lcd_part_cls(void)
{
  UINT16 LineStart;
  UINT16 ColumnStart;
  UINT16 LineEnd;
  UINT16 ColumnEnd;


  log_printf(__LINE__, __func__, Separator);
  log_printf(__LINE__, __func__, "<120>Testing function LCD_part_cls()\n");
  log_printf(__LINE__, __func__, "<120>LCD_part_cls() clears the LCD display area defined by the start and end coordinates and set the color as specified.\n");
  log_printf(__LINE__, __func__, "<120>The framebuffer is not changed and will be restored on LCD display by a call to LCD_refresh().\n");
  log_printf(__LINE__, __func__, Separator);
  log_printf(__LINE__, __func__, "Press <Enter> to clear framebuffer and proceed with LCD_part_cls() tests: ");
  if (stop_flow(TYPE_NONE)) return;
  log_printf(__LINE__, __func__, " \n");
  LCD_clear(BLACK);
  LCD_refresh();


  log_printf(__LINE__, __func__, "Press <Enter> to fill LCD display with text before testing LCD_part_cls() function: ");
  if (stop_flow(TYPE_NONE)) return;
  log_printf(__LINE__, __func__, " \n");
  fill_text(20);



  LineStart   =   0;
  ColumnStart =   0;
  LineEnd     = StructLCD.Height / 2;
  ColumnEnd   = StructLCD.Width - 1;
  log_printf(__LINE__, __func__, "Press <Enter> to clear display from (%u,%u) up to (%u,%u): ", LineStart, ColumnStart, LineEnd, ColumnEnd);
  if (stop_flow(TYPE_NONE)) return;
  LCD_part_cls(LineStart, ColumnStart, LineEnd, ColumnEnd, WHITE);



  LineStart   = StructLCD.Height / 4 * 3;
  ColumnStart = 0;
  LineEnd     = StructLCD.Height - 1;
  ColumnEnd   = StructLCD.Width / 2;
  log_printf(__LINE__, __func__, "Press <Enter> to clear display from (%u,%u) up to (%u,%u): ", LineStart, ColumnStart, LineEnd, ColumnEnd);
  if (stop_flow(TYPE_NONE)) return;
  LCD_part_cls(LineStart, ColumnStart, LineEnd, ColumnEnd, WHITE);



  log_printf(__LINE__, __func__, "Press <Enter> to restore original framebuffer content: ");
  if (stop_flow(TYPE_NONE)) return;
  LCD_refresh();


  log_printf(__LINE__, __func__, " \n");
  log_printf(__LINE__, __func__, "End of LCD_part_cls() function tests.\n");
  log_printf(__LINE__, __func__, Separator);
  if (stop_flow(TYPE_CONTINUE)) return;
  if (FlagAllTests) display_LCD_presentation();

  return;
}





/* $PAGE */
/* $TITLE=test_lcd_part_refresh() */
/* ============================================================================================================================================================= *\
                                                                  Test function LCD_part_refresh().
\* ============================================================================================================================================================= */
void test_lcd_part_refresh(void)
{
  UINT16 LineStart;
  UINT16 ColumnStart;
  UINT16 LineNumber;
  UINT16 ImageHeight;
  UINT16 ImageWidth;


  log_printf(__LINE__, __func__, Separator);
  log_printf(__LINE__, __func__, "<120>Testing function LCD_part_refresh()\n");
  log_printf(__LINE__, __func__, "<120>LCD_part_refresh() updates only the specified area of the LCD display defined by the start and end coordinates.\n");
  log_printf(__LINE__, __func__, "<120>This may be userful (and faster), for example if we want to update only the clock in a small area of LCD display.\n");
  log_printf(__LINE__, __func__, Separator);
  log_printf(__LINE__, __func__, "Press <Enter> to clear framebuffer and proceed with LCD_part_refresh() tests: ");
  if (stop_flow(TYPE_NONE)) return;
  log_printf(__LINE__, __func__, " \n");
  LCD_clear(BLACK);
  LCD_refresh();


  log_printf(__LINE__, __func__, "Press <Enter> to fill LCD display with text before testing LCD_part_refresh() function: ");
  if (stop_flow(TYPE_NONE)) return;
  log_printf(__LINE__, __func__, " \n");
  fill_text(20);


  ImageHeight = 240;
  ImageWidth  = 240;

  if (ImageHeight < StructLCD.Height)
    LineStart = (StructLCD.Height - ImageHeight) / 2;
  else
    LineStart = 0;


  if (ImageWidth < StructLCD.Width)
    ColumnStart = (StructLCD.Width - ImageWidth) / 2;
  else
    ColumnStart = 0;


  log_printf(__LINE__, __func__, "LineStart: %u   ColumnStart: %u\n", LineStart, ColumnStart);
  LCD_draw_image(LineStart, ColumnStart, (UINT16 *)WaterFall, ImageHeight, ImageWidth);


  // if ((StructLCD.Rotation != 0) && (StructLCD.Rotation != 180)) LCD_refresh();
  printf("\n\n");
  log_printf(__LINE__, __func__, "An image has been transferred to the framebuffer (but LCD_refresh() has not been done yet).\n");
  log_printf(__LINE__, __func__, "Press <Enter> many times to refresh the LCD display from framebuffer one line at a time using LCD_part_refresh()\n");
  log_printf(__LINE__, __func__, "You may press <ESC> at any time to return to menu.\n");
  for (LineNumber = LineStart; LineNumber < LineStart + ImageHeight; ++LineNumber)
  {
    LCD_part_refresh(LineNumber, ColumnStart, LineNumber, ColumnStart + ImageWidth - 1);
    log_printf(__LINE__, __func__, "Pixel line number %u has been refreshed on LCD display from framebuffer. Press <Enter> to continue: ", LineNumber);
    if (stop_flow(TYPE_NONE)) return;
  }


  // if ((StructLCD.Rotation != 0) && (StructLCD.Rotation != 180)) LCD_refresh();
  log_printf(__LINE__, __func__, " \n");
  log_printf(__LINE__, __func__, "End of LCD_part_refresh() function tests.\n");
  log_printf(__LINE__, __func__, Separator);
  if (stop_flow(TYPE_CONTINUE)) return;
  if (FlagAllTests) display_LCD_presentation();

  return;
}




/* $PAGE */
/* $TITLE=test_lcd_printf() */
/* ============================================================================================================================================================= *\
                                                                    Test function LCD_printf().
\* ============================================================================================================================================================= */
void test_lcd_printf(void)
{
#ifdef RELEASE_VERSION
  UINT8 FlagLocalDebug = FLAG_OFF;  // must be turned Off at all time.
#else   // RELEASE_VERSION
  UINT8 FlagLocalDebug = FLAG_OFF;   // may be modified for debug purposes.
#endif  // RELEASE_VERSION

  UCHAR Character;
  UCHAR Text[32];

  UINT16 LineStart;
  UINT16 ColumnStart;
  UINT16 Loop1UInt16;

  sFONT FontType;


  log_printf(__LINE__, __func__, Separator);
  log_printf(__LINE__, __func__, "<120>Testing function LCD_printf()\n");
  log_printf(__LINE__, __func__, "<120>NOTE: Specified line number for LCD_printf() corresponds to the top pixel line where printing begins.\n");
  log_printf(__LINE__, __func__, Separator);
  log_printf(__LINE__, __func__, "Press <Enter> to clear LCD display before proceeding to next tests: ");
  if (stop_flow(TYPE_NONE)) return;
  log_printf(__LINE__, __func__, " \n");
  LCD_clear(BLACK);
  LCD_refresh();

  /* Build string to be displayed on a single line. */
  FontType = Font24;
  Character = 'A';
  Text[0]   = 0x00;

  if (FlagLocalDebug)
  {
    log_printf(__LINE__, __func__, "Font width:  %2u\n", FontType.Width);
    log_printf(__LINE__, __func__, "Font height: %2u\n", FontType.Height);
  }

  for (Loop1UInt16 = 0; (((strlen(Text) + 1) * FontType.Width) < StructLCD.Width); ++Loop1UInt16)
  {
    Text[Loop1UInt16]     = Character + Loop1UInt16;
    Text[Loop1UInt16 + 1] = 0x00;  //  end-of-string.
  }



  for (LineStart = 0; LineStart < (StructLCD.Height - FontType.Height); LineStart += (FontType.Height + 10))
  {
    log_printf(__LINE__, __func__, "LCD_printf() on line number %u. Press <Enter> to proceed: ", LineStart);
    if (stop_flow(TYPE_NONE)) return;
    LCD_clear(WHITE);
    LCD_refresh();
    LCD_printf(LineStart, 0, RED, WHITE, &Font24, Text);
    LCD_refresh();
  }
  log_printf(__LINE__, __func__, " \n");
  log_printf(__LINE__, __func__, " \n");



  for (LineStart = StructLCD.Height - FontType.Height; LineStart < StructLCD.Height; LineStart += 3)
  {
    log_printf(__LINE__, __func__, "NOTE: For the next test, part of the characters may be outside the LCD display boundaries.\n");
    log_printf(__LINE__, __func__, "      The test shows that the part of the characters inside LCD boundaries will be properly displayed.\n");
    log_printf(__LINE__, __func__, "      Keep in mind that even if the whole characters appear on the display, the specified character set\n");
    log_printf(__LINE__, __func__, "      may have blank pixels above, under, to the left and / or to the right of the character that we see\n");
    log_printf(__LINE__, __func__, "      on LCD display.\n");
    log_printf(__LINE__, __func__, "LCD_printf() on pixel line number %u. Press <Enter> to proceed: ", LineStart);
    if (stop_flow(TYPE_NONE)) return;
    LCD_clear(WHITE);
    LCD_refresh();
    LCD_printf(LineStart, 0, RED, WHITE, &Font24, Text);
    LCD_refresh();
   log_printf(__LINE__, __func__, " \n");
  }
  log_printf(__LINE__, __func__, " \n");



  for (ColumnStart = 5; ColumnStart < 50; ColumnStart += FontType.Width)
  {
    log_printf(__LINE__, __func__, "LCD_printf() on column number %u. Press <Enter> to proceed: ", ColumnStart);
    if (stop_flow(TYPE_NONE)) return;
    LCD_clear(WHITE);
    LCD_refresh();

    Character = 'A';
    LineStart = 0;
    for (LineStart = 0; (LineStart + FontType.Height) < StructLCD.Height; LineStart += FontType.Height)
    {
      LCD_printf(LineStart, ColumnStart, RED, WHITE, &Font24, "%c", Character);
      ++Character;
    }
    LCD_refresh();
  }
  log_printf(__LINE__, __func__, " \n");



  for (ColumnStart = StructLCD.Width - FontType.Width - 5; ColumnStart > StructLCD.Width - FontType.Width - 55; ColumnStart -= FontType.Width)
  {
    log_printf(__LINE__, __func__, "LCD_printf() on column number %u. Press <Enter> to proceed: ", ColumnStart);
    if (stop_flow(TYPE_NONE)) return;
    LCD_clear(WHITE);
    LCD_refresh();

    Character = 'A';
    LineStart = 0;
    for (LineStart = 0; (LineStart + FontType.Height) < StructLCD.Height; LineStart += FontType.Height)
    {
      LCD_printf(LineStart, ColumnStart, RED, WHITE, &Font24, "%c", Character);
      ++Character;
    }
    LCD_refresh();
  }
  log_printf(__LINE__, __func__, " \n");



  /* Make a demo of each character set. */
  log_printf(__LINE__, __func__, "Press <Enter> to fill LCD display with 24 pixel-height character set: ");
  if (stop_flow(TYPE_NONE)) return;
  fill_text(24);


  log_printf(__LINE__, __func__, "Press <Enter> to fill LCD display with 20 pixel-height character set: ");
  if (stop_flow(TYPE_NONE)) return;
  fill_text(20);


  log_printf(__LINE__, __func__, "Press <Enter> to fill LCD display with 16 pixel-height character set: ");
  if (stop_flow(TYPE_NONE)) return;
  fill_text(16);


  log_printf(__LINE__, __func__, "Press <Enter> to fill LCD display with 12 pixel-height character set: ");
  if (stop_flow(TYPE_NONE)) return;
  fill_text(12);


  log_printf(__LINE__, __func__, "Press <Enter> to fill LCD display with 8 pixel-height character set: ");
  if (stop_flow(TYPE_NONE)) return;
  fill_text(8);

  log_printf(__LINE__, __func__, " \n");
  log_printf(__LINE__, __func__, "End of LCD_printf() function tests.\n");
  log_printf(__LINE__, __func__, Separator);
  if (stop_flow(TYPE_CONTINUE)) return;
  if (FlagAllTests) display_LCD_presentation();

  return;
}





/* $PAGE */
/* $TITLE=test_lcd_scroll() */
/* ============================================================================================================================================================= *\
                                                                  Test function LCD_scroll().
\* ============================================================================================================================================================= */
void test_lcd_scroll(void)
{
  UINT16 ImageHeight;
  UINT16 ImageWidth;
  UINT16 LineStart;
  UINT16 ColumnStart;
  UINT16 ScrollNumber;


  log_printf(__LINE__, __func__, Separator);
  log_printf(__LINE__, __func__, "<120>Testing function LCD_scroll()\n");
  log_printf(__LINE__, __func__, "<120>LCD_scroll() will scroll up the specified area of the LCD display defined by the start and end coordinates.\n");
  log_printf(__LINE__, __func__, "<120>The last line that has been scrolled will be replaced by a 'blank' line of the specified color.\n");
  log_printf(__LINE__, __func__, "<120>It is important to remember that, given the serial protocol used between the Pico and the LCD display,\n");
  log_printf(__LINE__, __func__, "<120>scrolling the display is a relatively slow operation.\n");
  log_printf(__LINE__, __func__, "<120>It may be done for small areas of LCD display,\n");
  log_printf(__LINE__, __func__, "<120>but will quickly become a nightmare if a big area is selected for scrolling.\n");
  log_printf(__LINE__, __func__, "<120>The PicoLogo will sroll up one line at a time and will gradually disappear when reaching the specified border limit.\n");
  log_printf(__LINE__, __func__, Separator);
  log_printf(__LINE__, __func__, "Press <Enter> to clear framebuffer and proceed with LCD_scroll() tests: ");
  if (stop_flow(TYPE_NONE)) return;
  LCD_clear(BLACK);
  LCD_refresh();

  log_printf(__LINE__, __func__, "Press <Enter> to clear LCD display before proceeding to next tests: ");
  if (stop_flow(TYPE_NONE)) return;
  LCD_clear(BLACK);
  LCD_refresh();

  /* Center image horizontally and vertically. */
  ImageHeight  = 23;
  ImageWidth   = 140;
  LineStart    = ((StructLCD.Height - ImageHeight) / 2);
  ColumnStart  = ((StructLCD.Width  - ImageWidth)  / 2);
  ScrollNumber = 25;
  LCD_clear(WHITE);
  LCD_draw_line(LineStart - ScrollNumber - 2, 0, LineStart - ScrollNumber - 2, StructLCD.Width - 1, BLACK, DOT_PIXEL_1X1, LINE_STYLE_SOLID);
  LCD_draw_image(LineStart, ColumnStart, (UINT16 *)PicoLogo, ImageHeight, ImageWidth);
  LCD_draw_line(LineStart + ImageHeight + 1, 0, LineStart + ImageHeight + 1, StructLCD.Width - 1, BLACK, DOT_PIXEL_1X1, LINE_STYLE_SOLID);
  LCD_refresh();


  log_printf(__LINE__, __func__, "Press <Enter> to scroll up the PicoLogo between both horizontal lines: ");
  if (stop_flow(TYPE_NONE)) return;
  LCD_scroll(0, LineStart, ColumnStart, LineStart + ImageHeight, ColumnStart + ImageWidth, ScrollNumber, WHITE);


  log_printf(__LINE__, __func__, " \n");
  log_printf(__LINE__, __func__, "End of LCD_scroll() function tests.\n");
  log_printf(__LINE__, __func__, Separator);
  if (stop_flow(TYPE_CONTINUE)) return;
  if (FlagAllTests) display_LCD_presentation();

  return;
}





/* $PAGE */
/* $TITLE=test_lcd_set_brightness() */
/* ============================================================================================================================================================= *\
                                                                Test function LCD_set_brightness().
\* ============================================================================================================================================================= */
void test_lcd_set_brightness(void)
{
  UINT16 Loop1UInt16;
  UINT16 OriginalBrightness;


  log_printf(__LINE__, __func__, Separator);
  log_printf(__LINE__, __func__, "<120>Testing function LCD_set_brightness()\n");
  log_printf(__LINE__, __func__, "<120>Current brightness setting is %u%c\n", StructLCD.Brightness, 0x25);
  log_printf(__LINE__, __func__, Separator);


  /* Keep track of original brightness. */
  OriginalBrightness = StructLCD.Brightness;
  // LCD_clear(WHITE);
  // LCD_refresh();

  for (Loop1UInt16 = 0; Loop1UInt16 <= 10; ++Loop1UInt16)
  {
    log_printf(__LINE__, __func__, "Press <Enter> to set brightness to %u%c%c: ", 100 - (Loop1UInt16 * 10), 0x25, 0x25);
    if (stop_flow(TYPE_NONE))
    {
      log_printf(__LINE__, __func__, "Restoring brightness to %u%c%c\n", OriginalBrightness, 0x25, 0x25);
      LCD_set_brightness(OriginalBrightness);  // restore original brightness before exiting.
      return;
    }

    StructLCD.Brightness = (100 - (Loop1UInt16 * 10));
    LCD_set_brightness(StructLCD.Brightness);
    log_printf(__LINE__, __func__, "Brightness has been set to %u%c%c\n", StructLCD.Brightness, 0x25, 0x25);
    log_printf(__LINE__, __func__, " \n");
  }

  log_printf(__LINE__, __func__, "Press <Enter> to restore brightness to %u%c%c: ", OriginalBrightness, 0x25, 0x25);
  if (stop_flow(TYPE_NONE));
  LCD_set_brightness(OriginalBrightness);  // restore original brightness when done.
  log_printf(__LINE__, __func__, " \n");
  log_printf(__LINE__, __func__, "End of LCD_set_brightness() function tests.\n");
  log_printf(__LINE__, __func__, Separator);
  if (stop_flow(TYPE_CONTINUE)) return;
  if (FlagAllTests) display_LCD_presentation();

  return;
}





/* $PAGE */
/* $TITLE=test_lcd_mirror_framebuffer()) */
/* ============================================================================================================================================================= *\
                                                                 Test function LCD_mirror_framebuffer().
\* ============================================================================================================================================================= */
void test_lcd_mirror_framebuffer(void)
{
  /* Display function title. */
  log_printf(__LINE__, __func__, Separator);
  log_printf(__LINE__, __func__, "<120>Testing function LCD_mirror_framebuffer()\n");
  log_printf(__LINE__, __func__, "<120>The ST7789 IC controls LCD scan and refresh from framebuffer. All rotation modes are available on the ST7789\n");
  log_printf(__LINE__, __func__, "<120>(0, 90, 180, or 270 degrees). However, not all mirroring modes can ba emulated with it. And even if some mirroring mode\n");
  log_printf(__LINE__, __func__, "<120>are available, they sometimes come with an inadequate scan direction.\n");
  log_printf(__LINE__, __func__, "<120>For this reason, the function LCD_mirror_framebuffer() has been designed, allowing the Firmware to emulate mirroring.\n");
  log_printf(__LINE__, __func__, "<120>Mirroring may be done on a selected area of LCD display.\n");
  log_printf(__LINE__, __func__, "<120>Current mirror mode is <%s>.\n", StructLCD.Mirroring[StructLCD.MirrorMode]);
  log_printf(__LINE__, __func__, Separator);


  LCD_clear(WHITE);
  LCD_refresh();
  fill_text(20);
  LCD_refresh();

  /* Convert current framebuffer to horizontal mirroring. */
  log_printf(__LINE__, __func__, "Press <Enter> to convert the whole LCD display to <horizontal mirroring>: ");
  if (stop_flow(TYPE_NONE)) return;
  LCD_mirror_framebuffer(0, 0, StructLCD.Height - 1, StructLCD.Width - 1, MIRROR_HORIZONTAL);
  LCD_refresh();
  log_printf(__LINE__, __func__, "Whole LCD display has been converted to <horizontal mirroring>\n");
  printf("\n\n");

  /* Since LCD display is already in horizontal mirroring, reprocessing it will return it to <no mirroring>. */
  log_printf(__LINE__, __func__, "Press <Enter> to return display to <no mirroring>: ");
  if (stop_flow(TYPE_NONE)) return;
  LCD_mirror_framebuffer(0, 0, StructLCD.Height - 1, StructLCD.Width - 1, MIRROR_NONE);
  LCD_refresh();
  log_printf(__LINE__, __func__, "Whole LCD display has been restored to <no mirroring>\n");
  printf("\n\n");




  /* Convert part of current framebuffer to horizontal mirroring. */
  log_printf(__LINE__, __func__, "Press <Enter> to convert an area of LCD display to <horizontal mirroring>: ");
  if (stop_flow(TYPE_NONE)) return;
  LCD_mirror_framebuffer(35, 30, StructLCD.Height / 2, StructLCD.Width - 30, MIRROR_HORIZONTAL);
  LCD_refresh();
  log_printf(__LINE__, __func__, "Part of LCD display has been converted to <horizontal mirroring>\n");
  printf("\n\n");

  /* Since LCD display is already in horizontal mirroring, reprocessing it will return it to <no mirroring>. */
  log_printf(__LINE__, __func__, "Press <Enter> to return display to <no mirroring>: ");
  if (stop_flow(TYPE_NONE)) return;
  LCD_mirror_framebuffer(35, 30, StructLCD.Height / 2, StructLCD.Width - 30, MIRROR_NONE);
  LCD_refresh();
  log_printf(__LINE__, __func__, "Part of LCD display has been restored to <no mirroring>\n");
  printf("\n\n");



  /* Convert current framebuffer to vertical mirroring. */
  log_printf(__LINE__, __func__, "Press <Enter> to convert the whole LCD display to <vertical mirroring>: ");
  if (stop_flow(TYPE_NONE)) return;
  LCD_mirror_framebuffer(0, 0, StructLCD.Height - 1, StructLCD.Width - 1, MIRROR_VERTICAL);
  LCD_refresh();
  log_printf(__LINE__, __func__, "Whole LCD display has been converted to <vertical mirroring>\n");
  printf("\n\n");

  /* Since LCD display is already in vertical mirroring, reprocessing it will return it to <no mirroring>. */
  log_printf(__LINE__, __func__, "Press <Enter> to return display to <no mirroring>: ");
  if (stop_flow(TYPE_NONE)) return;
  LCD_mirror_framebuffer(0, 0, StructLCD.Height - 1, StructLCD.Width - 1, MIRROR_NONE);
  LCD_refresh();
  log_printf(__LINE__, __func__, "Whole LCD display has been restored to <no mirroring>\n");
  printf("\n\n");



  /* Convert part of current framebuffer to vertical mirroring. */
  log_printf(__LINE__, __func__, "Press <Enter> to convert an area of LCD display to <vertical mirroring>: ");
  if (stop_flow(TYPE_NONE)) return;
  LCD_mirror_framebuffer(35, 30, StructLCD.Height / 2, StructLCD.Width - 30, MIRROR_VERTICAL);
  LCD_refresh();
  log_printf(__LINE__, __func__, "Part of LCD display has been converted to <vertical mirroring>\n");
  printf("\n\n");

  /* Since LCD display is already in vertical mirroring, reprocessing it will return it to <no mirroring>. */
  log_printf(__LINE__, __func__, "Press <Enter> to return display to <no mirroring>: ");
  if (stop_flow(TYPE_NONE)) return;
  LCD_mirror_framebuffer(35, 30, StructLCD.Height / 2, StructLCD.Width - 30, MIRROR_NONE);
  LCD_refresh();
  log_printf(__LINE__, __func__, "Part of LCD display has been restored to <no mirroring>\n");
  printf("\n\n");



  /* Convert current framebuffer to origin mirroring. */
  log_printf(__LINE__, __func__, "Press <Enter> to convert the whole LCD display to <origin mirroring>: ");
  if (stop_flow(TYPE_NONE)) return;
  LCD_mirror_framebuffer(0, 0, StructLCD.Height - 1, StructLCD.Width - 1, MIRROR_ORIGIN);
  LCD_refresh();
  log_printf(__LINE__, __func__, "Whole LCD display has been converted to <origin mirroring>\n");
  printf("\n\n");

  /* Since LCD display is already in origin mirroring, reprocessing it will return it to <no mirroring>. */
  log_printf(__LINE__, __func__, "Press <Enter> to return display to <no mirroring>: ");
  if (stop_flow(TYPE_NONE)) return;
  LCD_mirror_framebuffer(0, 0, StructLCD.Height - 1, StructLCD.Width - 1, MIRROR_NONE);
  LCD_refresh();
  log_printf(__LINE__, __func__, "Whole LCD display has been restored to <no mirroring>\n");
  printf("\n\n");



  /* Convert part of current framebuffer to origin mirroring. */
  log_printf(__LINE__, __func__, "Press <Enter> to convert an area of LCD display to <origin mirroring>: ");
  if (stop_flow(TYPE_NONE)) return;
  LCD_mirror_framebuffer(35, 30, StructLCD.Height / 2, StructLCD.Width - 30, MIRROR_ORIGIN);
  LCD_refresh();
  log_printf(__LINE__, __func__, "Part of LCD display has been converted to <origin mirroring>\n");
  printf("\n\n");

  /* Since LCD display is already in origin mirroring, reprocessing it will return it to <no mirroring>. */
  log_printf(__LINE__, __func__, "Press <Enter> to return display to <no mirroring>: ");
  if (stop_flow(TYPE_NONE)) return;
  LCD_mirror_framebuffer(35, 30, StructLCD.Height / 2, StructLCD.Width - 30, MIRROR_NONE);
  LCD_refresh();
  log_printf(__LINE__, __func__, "Part of LCD display has been restored to <no mirroring>\n");
  printf("\n\n");



  log_printf(__LINE__, __func__, " \n");
  log_printf(__LINE__, __func__, "End of LCD_mirror_framebuffer() tests.\n");
  if (stop_flow(TYPE_CONTINUE)) return;
  if (FlagAllTests) display_LCD_presentation();

  return;
}





/* $PAGE */
/* $TITLE=test_lcd_set_pixel() */
/* ============================================================================================================================================================= *\
                                                                    Test function LCD_set_pixel().
\* ============================================================================================================================================================= */
void test_lcd_set_pixel(void)
{
  UINT16 Loop1UInt16;
  UINT16 LineStart;
  UINT16 ColumnStart;


  log_printf(__LINE__, __func__, Separator);
  log_printf(__LINE__, __func__, "<120>Testing function LCD_set_pixel()\n");
  log_printf(__LINE__, __func__, Separator);
  log_printf(__LINE__, __func__, "Press <Enter> to clear LCD display before proceeding to next tests: ");
  if (stop_flow(TYPE_NONE)) return;
  log_printf(__LINE__, __func__, " \n");
  LCD_clear(WHITE);
  LCD_refresh();


  /* Draw many pixels on the same line separated by spaces. */
  for (LineStart = 5; LineStart < 50; LineStart += 5)
  {
    log_printf(__LINE__, __func__, "LCD_set_pixel() on line number %2u (to draw a train of many pixels). Press <Enter> to proceed: ", LineStart);
    if (stop_flow(TYPE_NONE)) return;

    for (Loop1UInt16 = 0; Loop1UInt16 < StructLCD.Width - 10; Loop1UInt16 += 25)
    {
      LCD_set_pixel(LineStart, Loop1UInt16 + 1, RED);
      LCD_set_pixel(LineStart, Loop1UInt16 + 2, RED);
      LCD_set_pixel(LineStart, Loop1UInt16 + 3, RED);
      LCD_set_pixel(LineStart, Loop1UInt16 + 4, RED);
      LCD_set_pixel(LineStart, Loop1UInt16 + 5, RED);
      LCD_set_pixel(LineStart, Loop1UInt16 + 6, RED);
      LCD_set_pixel(LineStart, Loop1UInt16 + 7, RED);
      LCD_set_pixel(LineStart, Loop1UInt16 + 8, RED);
      LCD_set_pixel(LineStart, Loop1UInt16 + 9, RED);
    }
    LCD_refresh();
  }
  log_printf(__LINE__, __func__, " \n");
  log_printf(__LINE__, __func__, " \n");



  /* Draw many pixels on the same line separated by spaces. */
  for (LineStart = StructLCD.Height - 6; LineStart > StructLCD.Height - 50; LineStart -= 5)
  {
    log_printf(__LINE__, __func__, "LCD_set_pixel() on line number %u (to draw a train of many pixels). Press <Enter> to proceed: ", LineStart);
    if (stop_flow(TYPE_NONE)) return;

    for (Loop1UInt16 = 0; Loop1UInt16 < StructLCD.Width - 10; Loop1UInt16 += 25)
    {
      LCD_set_pixel(LineStart, Loop1UInt16 + 1, BLUE);
      LCD_set_pixel(LineStart, Loop1UInt16 + 2, BLUE);
      LCD_set_pixel(LineStart, Loop1UInt16 + 3, BLUE);
      LCD_set_pixel(LineStart, Loop1UInt16 + 4, BLUE);
      LCD_set_pixel(LineStart, Loop1UInt16 + 5, BLUE);
      LCD_set_pixel(LineStart, Loop1UInt16 + 6, BLUE);
      LCD_set_pixel(LineStart, Loop1UInt16 + 7, BLUE);
      LCD_set_pixel(LineStart, Loop1UInt16 + 8, BLUE);
      LCD_set_pixel(LineStart, Loop1UInt16 + 9, BLUE);
    }
    LCD_refresh();
  }
  log_printf(__LINE__, __func__, " \n");
  log_printf(__LINE__, __func__, " \n");



  /* Draw many pixels on the same column separated by spaces. */
  for (ColumnStart = 5; ColumnStart < 50; ColumnStart += 5)
  {
    log_printf(__LINE__, __func__, "LCD_set_pixel() on column number %2u (to draw a train of many pixels). Press <Enter> to proceed: ", ColumnStart);
    if (stop_flow(TYPE_NONE)) return;

    for (Loop1UInt16 = 0; Loop1UInt16 < StructLCD.Height - 10; Loop1UInt16 += 25)
    {
      LCD_set_pixel(Loop1UInt16 + 1, ColumnStart, RED);
      LCD_set_pixel(Loop1UInt16 + 2, ColumnStart, RED);
      LCD_set_pixel(Loop1UInt16 + 3, ColumnStart, RED);
      LCD_set_pixel(Loop1UInt16 + 4, ColumnStart, RED);
      LCD_set_pixel(Loop1UInt16 + 5, ColumnStart, RED);
      LCD_set_pixel(Loop1UInt16 + 6, ColumnStart, RED);
      LCD_set_pixel(Loop1UInt16 + 7, ColumnStart, RED);
      LCD_set_pixel(Loop1UInt16 + 8, ColumnStart, RED);
      LCD_set_pixel(Loop1UInt16 + 9, ColumnStart, RED);
    }
    LCD_refresh();
  }
  log_printf(__LINE__, __func__, " \n");
  log_printf(__LINE__, __func__, " \n");

  
  
  /* Draw many pixels on the same column separated by spaces. */
  for (ColumnStart = StructLCD.Width - 6; ColumnStart > StructLCD.Width - 50; ColumnStart -= 5)
  {
    log_printf(__LINE__, __func__, "LCD_set_pixel() on column number %u (to draw a train of many pixels). Press <Enter> to proceed: ", ColumnStart);
    if (stop_flow(TYPE_NONE)) return;

    for (Loop1UInt16 = 0; Loop1UInt16 < StructLCD.Height - 10; Loop1UInt16 += 25)
    {
      LCD_set_pixel(Loop1UInt16 + 1, ColumnStart, RED);
      LCD_set_pixel(Loop1UInt16 + 2, ColumnStart, RED);
      LCD_set_pixel(Loop1UInt16 + 3, ColumnStart, RED);
      LCD_set_pixel(Loop1UInt16 + 4, ColumnStart, RED);
      LCD_set_pixel(Loop1UInt16 + 5, ColumnStart, RED);
      LCD_set_pixel(Loop1UInt16 + 6, ColumnStart, RED);
      LCD_set_pixel(Loop1UInt16 + 7, ColumnStart, RED);
      LCD_set_pixel(Loop1UInt16 + 8, ColumnStart, RED);
      LCD_set_pixel(Loop1UInt16 + 9, ColumnStart, RED);
    }
    LCD_refresh();
  }
  log_printf(__LINE__, __func__, " \n");

  log_printf(__LINE__, __func__, "End of LCD_set_pixel() function tests.\n");
  log_printf(__LINE__, __func__, Separator);
  if (stop_flow(TYPE_CONTINUE)) return;
  if (FlagAllTests) display_LCD_presentation();

  return;
}





/* $PAGE */
/* $TITLE=test_lcd_set_rotation() */
/* ============================================================================================================================================================= *\
                                                                 Test function LCD_set_rotation().
\* ============================================================================================================================================================= */
void test_lcd_set_rotation(void)
{
  UINT16 OriginalRotation;
  UINT16 RotationNumber;


  log_printf(__LINE__, __func__, Separator);
  log_printf(__LINE__, __func__, "<120>Testing function LCD_set_rotation()\n");
  log_printf(__LINE__, __func__, Separator);


  /* Keep track of original rotation setting. */
  OriginalRotation = StructLCD.Rotation;

  LCD_clear(BLACK);
  LCD_refresh();

  /* Proceed with tests for each valid rotation value: 0, 90, 180, 270 degrees. */
  for (RotationNumber = 0; RotationNumber < 4; ++RotationNumber)
  {
    switch (RotationNumber)
    {
      case (0):
        LCD_set_rotation(ROTATE_0);
      break;

      case (1):
        LCD_set_rotation(ROTATE_90);
      break;

      case (2):
        LCD_set_rotation(ROTATE_180);
      break;

      case (3):
        LCD_set_rotation(ROTATE_270);
      break;
    }

    log_printf(__LINE__, __func__, "Proceed with rotation tests at %u-degree.\n", StructLCD.Rotation);
    log_printf(__LINE__, __func__, " \n");

    log_printf(__LINE__, __func__, "Press <Enter> to fill screen with text at current rotation setting: ");
    if (stop_flow(TYPE_NONE)) return;
    fill_text(16);

    LCD_part_clear(30, 0, 45, StructLCD.Width - 1, WHITE);
    LCD_draw_line(30, 0, 30, StructLCD.Width - 1, RED, DOT_PIXEL_1X1, LINE_STYLE_SOLID);
    LCD_printf(32, 999, RED, WHITE, &Font16, "Rotation %u-degree", StructLCD.Rotation);
    LCD_draw_line(46, 0, 46, StructLCD.Width - 1, RED, DOT_PIXEL_1X1, LINE_STYLE_SOLID);
    LCD_refresh();

    log_printf(__LINE__, __func__, "Press <Enter> to LCD_cls() display in red: ");
    if (stop_flow(TYPE_NONE)) return;
    LCD_cls(RED);

    log_printf(__LINE__, __func__, "Press <Enter> to LCD_cls() display in blue: ");
    if (stop_flow(TYPE_NONE)) return;
    LCD_cls(BLUE);

    log_printf(__LINE__, __func__, "Press <Enter> to LCD_cls() display in green: ");
    if (stop_flow(TYPE_NONE)) return;
    LCD_cls(GREEN);

    log_printf(__LINE__, __func__, "Press <Enter> to restore original framebuffer and refresh display: ");
    if (stop_flow(TYPE_NONE)) return;
    LCD_refresh();

    log_printf(__LINE__, __func__, "Press <Enter> to continue: ");
    if (stop_flow(TYPE_NONE)) return;
    LCD_clear(BLACK);
    LCD_refresh();
    log_printf(__LINE__, __func__, " \n");
  }

  /* Restore original rotation setting. */
  LCD_set_rotation(OriginalRotation);
  log_printf(__LINE__, __func__, "Rotation setting has been restored to its original value: %u\n", StructLCD.Rotation);
  log_printf(__LINE__, __func__, " \n");
  log_printf(__LINE__, __func__, "End of LCD_rotation() tests.\n");
  log_printf(__LINE__, __func__, Separator);
  if (stop_flow(TYPE_CONTINUE));
  if (FlagAllTests) display_LCD_presentation();

  return;
}





/* $PAGE */
/* $TITLE=test_lcd_splash_image() */
/* ============================================================================================================================================================= *\
                                                                Test function LCD_splash_image().
\* ============================================================================================================================================================= */
void test_lcd_splash_image(void)
{
  UINT16 ImageHeight;
  UINT16 ImageWidth;
  UINT16 LineStart;
  UINT16 ColumnStart;


  log_printf(__LINE__, __func__, Separator);
  log_printf(__LINE__, __func__, "<120>Testing function LCD_splash_image()\n");
  log_printf(__LINE__, __func__, "<120>The LCD_splash_image() function transfers part of the framebuffer\n");
  log_printf(__LINE__, __func__, "<120>(or another memory location whose pointer is passed as a parameter) to LCD display.\n");
  log_printf(__LINE__, __func__, "<120>For 65k colors, color depth is 2 bytes per pixel (RGB-565) for a total of %u X %u X 2 = %lu bytes per image\n", StructLCD.Height, StructLCD.Width, StructLCD.Height * StructLCD.Width * 2);
  log_printf(__LINE__, __func__, Separator);


  /* Demo of function LCD_splash_image() to transfer an image bitmap to LCD display. */
  log_printf(__LINE__, __func__, "Transfering WaterFall image to LCD display.\n");
  log_printf(__LINE__, __func__, "NOTE: A pointer to WaterFall image is used, so the framebuffer remains unchanged.\n");
  log_printf(__LINE__, __func__, "(and a LCD_refresh() is not required since the image is transferred directly to LCD display).\n");
  if (stop_flow(TYPE_CONTINUE)) return;
  LCD_clear(BLACK);
  LCD_refresh();
  ImageHeight = 240;
  ImageWidth  = 240;
  if (ImageHeight < StructLCD.Height)
    LineStart = (StructLCD.Height - ImageHeight) / 2;
  else
    LineStart = 0;
  if (ImageWidth < StructLCD.Width)
    ColumnStart = (StructLCD.Width  - ImageWidth)  / 2;
  else
    ColumnStart = 0;

  LCD_splash_image(LineStart, ColumnStart, LineStart + ImageHeight - 1, ColumnStart + ImageWidth - 1, (UINT16 *)WaterFall);
  log_printf(__LINE__, __func__, "Image <WaterFall> has been transferred to LCD display.\n");
  log_printf(__LINE__, __func__, Separator);
  if (stop_flow(TYPE_CONTINUE)) return;





  /* Demo of function LCD_splash_image() to transfer an image bitmap to LCD display. */
  log_printf(__LINE__, __func__, "Transfering PicoLogo image to LCD display.\n");
  log_printf(__LINE__, __func__, "NOTE: A pointer to PicoLogo image is used, so the framebuffer remains unchanged.\n");
  log_printf(__LINE__, __func__, "(and a LCD_refresh() is not required since the image is transferred directly to LCD display).\n");
  if (stop_flow(TYPE_CONTINUE)) return;
  LCD_clear(BLACK);
  LCD_refresh();
  ImageHeight =  23;
  ImageWidth  = 140;
  if (ImageHeight < StructLCD.Height)
    LineStart = (StructLCD.Height - ImageHeight) / 2;
  else
    LineStart = 0;
  if (ImageWidth < StructLCD.Width)
    ColumnStart = (StructLCD.Width  - ImageWidth)  / 2;
  else
    ColumnStart = 0;

  LCD_splash_image(LineStart, ColumnStart, LineStart + ImageHeight - 1, ColumnStart + ImageWidth - 1, (UINT16 *)PicoLogo);
  log_printf(__LINE__, __func__, "Image <PicoLogo> has been transferred to LCD display.\n");
  log_printf(__LINE__, __func__, Separator);
  if (stop_flow(TYPE_CONTINUE)) return;





  /* Demo of function LCD_splash_image() to transfer an image bitmap to LCD display. */
  if ((StructLCD.Rotation == 90) || (StructLCD.Rotation == 270))
  {
    log_printf(__LINE__, __func__, "Transfering SunFall image to LCD display.\n");
    log_printf(__LINE__, __func__, "NOTE: A pointer to SunFall image is used, so the framebuffer remains unchanged.\n");
    log_printf(__LINE__, __func__, "(and a LCD_refresh() is not required since the image is transferred directly to LCD display).\n");
    if (stop_flow(TYPE_CONTINUE)) return;
    LCD_clear(BLACK);
    LCD_refresh();
    ImageHeight = 240;
    ImageWidth  = 320;
    if (ImageHeight <= StructLCD.Height)
      LineStart = (StructLCD.Height - ImageHeight) / 2;
    else
      LineStart = 0;
    if (ImageWidth <= StructLCD.Width)
      ColumnStart = (StructLCD.Width  - ImageWidth)  / 2;
    else
      ColumnStart = 0;

    LCD_splash_image(LineStart, ColumnStart, LineStart + ImageHeight - 1, ColumnStart + ImageWidth - 1, (UINT16 *)SunFall);
    log_printf(__LINE__, __func__, "Image <SunFall> has been transferred to LCD display.\n");
    log_printf(__LINE__, __func__, Separator);
    if (stop_flow(TYPE_CONTINUE)) return;
  }

  log_printf(__LINE__, __func__, " \n");
  log_printf(__LINE__, __func__, "End of LCD_splash_image() function tests.\n");
  log_printf(__LINE__, __func__, Separator);
  if (stop_flow(TYPE_CONTINUE)) return;
  if (FlagAllTests) display_LCD_presentation();

  return;
}





/* $PAGE */
/* $TITLE=test_line_draw() */
/* ============================================================================================================================================================= *\
                                                              Test to draw lines on LCD display.
\* ============================================================================================================================================================= */
void test_line_draw(void)
{
  UINT16 Loop2UInt16;


  log_printf(__LINE__, __func__, Separator);
  log_printf(__LINE__, __func__, "<120>Testing function LCD_line_draw()\n");
  log_printf(__LINE__, __func__, Separator);
  log_printf(__LINE__, __func__, "Press <Enter> to display a double line on LCD display: ");
  if (stop_flow(TYPE_NONE)) return;

  /* Draw two lines (one under the other to make it thicker) in the Framebuffer. */
  for (Loop2UInt16 = 5; Loop2UInt16 < 14; ++Loop2UInt16)
    LCD_set_pixel(10, Loop2UInt16, BLACK);
  for (Loop2UInt16 = 5; Loop2UInt16 < 14; ++Loop2UInt16)
    LCD_set_pixel(11, Loop2UInt16, BLACK);

  /* Splash the Framebuffer to LCD display. */
  LCD_refresh();

  log_printf(__LINE__, __func__, "End of LCD_line_draw() function tests.\n");
  if (stop_flow(TYPE_CONTINUE)) return;
  if (FlagAllTests) display_LCD_presentation();

  return;
}





/* $PAGE */
/* $TITLE=test_miscellaneous()) */
/* ============================================================================================================================================================= *\
                                                                 Perform miscellaneous combined tests.
\* ============================================================================================================================================================= */
void test_miscellaneous(void)
{
  UCHAR String[65];

  UINT16 Loop1UInt16;
  UINT16 Loop2UInt16;


  log_printf(__LINE__, __func__, Separator);
  log_printf(__LINE__, __func__, "<120>Miscellaneous combined tests\n");
  log_printf(__LINE__, __func__, Separator);
  log_printf(__LINE__, __func__, "Press <Enter> to proceed with miscellaneous tests or press <ESC> to return to menu: ");
  if (stop_flow(TYPE_NONE)) return;


  /* Draw two boxes: first one around the magenta crossed lines drawn above and another over the cyan crossed lines drawn above. */
  log_printf(__LINE__, __func__, "Press <Enter> to draw boxes: ");
  if (stop_flow(TYPE_NONE)) return;

  // log_printf(__LINE__, __func__, "Draw two rectangles: one around and one over the crossed lines drawn above.\n");
  LCD_draw_box(10, 35, 40,  65,  RED, DOT_PIXEL_2X2, DRAW_FILL_EMPTY);
  LCD_draw_box(10, 90, 55, 135, BLUE, DOT_PIXEL_2X2, DRAW_FILL_FULL);
  LCD_refresh();  // transfer Framebuffer to LCD display.



  /* Demo of function LCD_printf() to display text and numbers with different background and foregrounf colors. */
  log_printf(__LINE__, __func__, "Press <Enter> to print different texts: ");
  if (stop_flow(TYPE_NONE)) return;

  LCD_printf( 72,  43,  BLACK,  WHITE, &Font16, "%3.5f", 9.87654321);
  LCD_printf( 68, 153, 0xFFF0, 0x000F, &Font20, "ABC");
  LCD_printf(100, 999,  WHITE,    RED, &Font24, "Waveshare");
  LCD_refresh();  // transfer Framebuffer to LCD display.



  /* LCD demo to show display resolution. */
  /* First: Line resolution. */
  log_printf(__LINE__, __func__, "Press <Enter> to display one-pixel lines across the display to show display resolution: ");
  if (stop_flow(TYPE_NONE)) return;
  
  for (Loop1UInt16 = 5; Loop1UInt16 < StructLCD.Height; Loop1UInt16 += 15)
  {
    for (Loop2UInt16 = 5; Loop2UInt16 < StructLCD.Width; ++Loop2UInt16)
    {
      LCD_set_pixel(Loop1UInt16, Loop2UInt16, BLACK);
    }
    LCD_refresh();
  }


  /* Next: Column resolution. */
  log_printf(__LINE__, __func__, "Press <Enter> to display one-pixel columns across the display to show display resolution: ");
  if (stop_flow(TYPE_NONE)) return;
  
  LCD_clear(WHITE);
  for (Loop1UInt16 = 5; Loop1UInt16 < StructLCD.Width; Loop1UInt16 += 15)
  {
    for (Loop2UInt16 = 5; Loop2UInt16 < StructLCD.Height; ++Loop2UInt16)
    {
      LCD_set_pixel(Loop1UInt16, Loop2UInt16, BLACK);
    }
    LCD_refresh();
  }



  /* LCD_printf() tests. */
  log_printf(__LINE__, __func__, "Demo of function LCD_printf()\n");
  log_printf(__LINE__, __func__, "Press <Enter> to print 1st demo: ");
  input_string(String, 1, 0l);
  printf("\n");
  LCD_printf(0, 0, BLACK, WHITE, &Font16, "printf 1");
  LCD_refresh();
  log_printf(__LINE__, __func__, "Press <Enter> to print 2nd demo: ");
  input_string(String, 1, 0l);
  printf("\n");


  LCD_printf(0, 0, BLACK, WHITE, &Font16, "Printing line 1");
  LCD_refresh();
  log_printf(__LINE__, __func__, "Press <Enter> to print 3rd demo: ");
  input_string(String, 1, 0l);
  printf("\n");


  LCD_printf(0, 0, BLACK, WHITE, &Font16, "Ovewriting line 1");
  LCD_refresh();
  log_printf(__LINE__, __func__, "Press <Enter> to print 4th demo: ");
  input_string(String, 1, 0l);
  printf("\n");


  LCD_printf(20, 0, BLACK, WHITE, &Font16, "Printf %u, 0x%X", 0xFF, 175);
  LCD_refresh();
  log_printf(__LINE__, __func__, "Press <Enter> to print 5th demo: ");
  input_string(String, 1, 0l);
  printf("\n");


  LCD_printf(40, 999, BLACK, WHITE, &Font16, "Printf centered");
  LCD_refresh();
  log_printf(__LINE__, __func__, "Press <Enter> to print 6th demo: ");
  input_string(String, 1, 0l);
  printf("\n");

  
  LCD_printf(60, 999, BLACK, WHITE, &Font16, "Printf %u, 0x%X", 0xFF, 175);
  LCD_refresh();
  log_printf(__LINE__, __func__, "Press <Enter> to print 7th demo: ");
  input_string(String, 1, 0l);
  printf("\n");

  
  LCD_printf(80, 999, BLACK, WHITE, &Font16, "This is text centered");
  LCD_refresh();
  log_printf(__LINE__, __func__, "Press <Enter> to proceed with four buttons demos: ");
  input_string(String, 1, 0l);
  printf("\n");

  
  LCD_cls(WHITE);
  LCD_clear(WHITE);
  LCD_refresh();


  /* Demo of LCD module's four buttons. */
  log_printf(__LINE__, __func__, "Demo of LCD module's four buttons.\n");
  log_printf(__LINE__, __func__, "Button demo will exit after 10 button presses.\n");
  #undef LCD_DEBUG_DRAW_POINT  // make sure this debug is not On for button demo.
  Loop1UInt16 = 0;
  while (1)
  {
    if (gpio_get(BUTTON0) == 0)
    {
      LCD_draw_box(5, 10, 35, 40, 0xF800, DOT_PIXEL_2X2, DRAW_FILL_FULL);
      ++Loop1UInt16;
    }
    else
    {
      LCD_draw_box(5, 10, 35, 40, 0x000F, DOT_PIXEL_2X2, DRAW_FILL_EMPTY);
    }
            

    if (gpio_get(BUTTON1) == 0)
    {
      LCD_draw_box(100, 10, 130, 40, 0xF800, DOT_PIXEL_2X2, DRAW_FILL_FULL);
      ++Loop1UInt16;
    }
    else
    {
      LCD_draw_box(100, 10, 130, 40, 0x000F, DOT_PIXEL_2X2, DRAW_FILL_EMPTY);
    }
        

    if (gpio_get(BUTTON2) == 0)
    {
      LCD_draw_box(5, 200, 35, 230, 0xF800, DOT_PIXEL_2X2, DRAW_FILL_FULL);
      ++Loop1UInt16;
    }
    else
    {
      LCD_draw_box(5, 200, 35, 230, 0x000F, DOT_PIXEL_2X2, DRAW_FILL_EMPTY);
    }
        

    if (gpio_get(BUTTON3) == 0)
    {
      LCD_draw_box(100, 200, 130, 230, 0xF800, DOT_PIXEL_2X2, DRAW_FILL_FULL);
      ++Loop1UInt16;
    }
    else
    {
      LCD_draw_box(100, 200, 130, 230, 0x000F, DOT_PIXEL_2X2, DRAW_FILL_EMPTY);
    }

    LCD_printf(60, 999, BLACK, WHITE, &Font24, " Counter: %u ", Loop1UInt16);
    LCD_refresh();
    sleep_ms(100);
    while (gpio_get(BUTTON0) + gpio_get(BUTTON1) + gpio_get(BUTTON2) + gpio_get(BUTTON3) == 0);
    LCD_clear(WHITE);

    if (Loop1UInt16 > 9) break;  // get out of while loop after a few button presses.
  }

  /* Make sure all buttons are blank when done. */
  LCD_draw_box(  5,  10,  35,  40, 0x000F, DOT_PIXEL_2X2, DRAW_FILL_EMPTY);
  LCD_draw_box(100,  10, 130,  40, 0x000F, DOT_PIXEL_2X2, DRAW_FILL_EMPTY);
  LCD_draw_box(  5, 200,  35, 230, 0x000F, DOT_PIXEL_2X2, DRAW_FILL_EMPTY);
  LCD_draw_box(100, 200, 130, 230, 0x000F, DOT_PIXEL_2X2, DRAW_FILL_EMPTY);


  log_printf(__LINE__, __func__, "Done with button tests.\n");
  if (stop_flow(TYPE_CONTINUE)) return;
  LCD_clear(BLACK);
  LCD_refresh();

  return;
}





/* $PAGE */
/* $TITLE=test_set_window() */
/* ============================================================================================================================================================= *\
\* ============================================================================================================================================================= */
void test_rotation(void)
{
  UINT16 Color;
  UINT16 Loop1UInt16;
  UINT16 Loop2UInt16;


  Color = RED;

  for (Loop1UInt16 = 0; Loop1UInt16 < 4; ++Loop1UInt16)
  {
    switch (Loop1UInt16)
    {
      case (0):
        LCD_set_rotation(0);
      break;

      case (1):
        LCD_set_rotation(90);
      break;

      case (2):
        LCD_set_rotation(180);
      break;

      case (3):
        LCD_set_rotation(270);
      break;
    }

    log_printf(__LINE__, __func__, "CLS");
    log_printf(__LINE__, __func__, "HOME");
    log_printf(__LINE__, __func__, "Rotation mode has been set to %u-degree.\n", StructLCD.Rotation);
    LCD_display_info();

    log_printf(__LINE__, __func__, "Before LCD_clear()\n");
    LCD_clear(WHITE);

    log_printf(__LINE__, __func__, "Before LCD_set_window()\n");
    LCD_set_window(10, 10, 100, 100);

    gpio_put(StructLCD.GPIODataCommand, 1);  // set command mode.
    gpio_put(StructLCD.GPIOChipSelect,  0);  // enable chip select active low.
  
    log_printf(__LINE__, __func__, "Before spi_write_blocking()\n");

    for (Loop2UInt16 = 0; Loop2UInt16 < (90 * 90); ++Loop2UInt16)
      spi_write_blocking(StructLCD.SPIPort, (UINT8 *)&Color, 2);

    gpio_put(StructLCD.GPIOChipSelect, 1);  // disable chip select when done.

    log_printf(__LINE__, __func__, "Before second LCD_set_window()\n");
    LCD_set_window(0, 0, StructLCD.Height - 1, StructLCD.Width - 1);
    if (stop_flow(TYPE_CONTINUE)) break;


    log_printf(__LINE__, __func__, "Before LCD_clear()\n");
    LCD_clear(BLUE);

    log_printf(__LINE__, __func__, "Before LCD_refresh()\n");
    LCD_refresh();
    if (stop_flow(TYPE_CONTINUE)) break;

    log_printf(__LINE__, __func__, "Before LCD_cls()\n");
    LCD_cls(GREEN);
    if (stop_flow(TYPE_CONTINUE)) break;
  }
  LCD_set_rotation(ROTATE_0);

  return;
}





/* $PAGE */
/* $TITLE=util_display_data() */
/* ============================================================================================================================================================= *\
                                            Display data whose pointer is sent in argument to an external monitor.
\* ============================================================================================================================================================= */
void util_display_data(UCHAR *Data, UINT32 DataSize)
{
  UCHAR String[256];

  UINT32 Loop1UInt32;
  UINT32 Loop2UInt32;


  log_printf(__LINE__, __func__, Separator);
  log_printf(__LINE__, __func__, "<120>Entering util_display_data() - Data pointer: 0x%X   DataSize: 0x%4.4lX (%u)\n", Data, DataSize, DataSize);
  log_printf(__LINE__, __func__, Separator);
  log_printf(__LINE__, __func__, "                                                                             Printable\n");
  log_printf(__LINE__, __func__, "  Address     Offset                       Hex data                         characters\n");
  log_printf(__LINE__, __func__, "------------------------------------------------------------------------------------------\n");


  for (Loop1UInt32 = 0; Loop1UInt32 < DataSize; Loop1UInt32 += 16)
  {
    /* First, display memory address, offset and hex part. */
    snprintf(String, sizeof(String), "[0x%8.8X] [0x%4.4X] - ", &Data[Loop1UInt32], Loop1UInt32);

    for (Loop2UInt32 = 0; Loop2UInt32 < 16; ++Loop2UInt32)
    {
      if ((Loop1UInt32 + Loop2UInt32) >= DataSize)
        strcat(String, "   ");
      else
        snprintf(&String[strlen(String)], sizeof(String) - strlen(String), "%2.2X ", Data[Loop1UInt32 + Loop2UInt32]);
    }


    log_printf(__LINE__, __func__, String);


    /* Add separator. */
    snprintf(String, sizeof(String), "| ");


    /* Then, display ASCII character if it is displayable ASCII (or <.> if it is not). */
    for (Loop2UInt32 = 0; Loop2UInt32 < 16; ++Loop2UInt32)
    {
      if ((Loop1UInt32 + Loop2UInt32) >= DataSize)
        break; // do not count up to 16 if we already reached end of data to display.

      /* Display character if it is displayable ASCII. */
      if ((Data[Loop1UInt32 + Loop2UInt32] >= 0x20) && (Data[Loop1UInt32 + Loop2UInt32] <= 0x7E) && (Data[Loop1UInt32 + Loop2UInt32] != 0x25))
        snprintf(&String[strlen(String)], sizeof(String) - strlen(String), "%c", Data[Loop1UInt32 + Loop2UInt32]);
      else
        strcat(String, ".");
    }
    printf("%s\n", String);
    sleep_ms(10);  // prevent communication override.
  }
  log_printf(__LINE__, __func__, Separator);

  return;
}





/* $PAGE */
/* $TITLE=zone_test1() */
/* ============================================================================================================================================================= *\
                                                                        Test display colors.
\* ============================================================================================================================================================= */
void zone_test1(void)
{
  UINT16 Color;
  UINT16 Loop1UInt16;


  log_printf(__LINE__, __func__, "Press <Enter> to proceed with LCD display color tests: ");
  if (stop_flow(TYPE_NONE)) return;

  Color = 0x4000;
  for (Loop1UInt16 = 0; Loop1UInt16 < 0xFFFF; ++Loop1UInt16)
  {
    log_printf(__LINE__, __func__, "Press <Enter> to proceed with LCD_cls(0x%4.4X) or press <ESC> to return to menu: ", Color);
    if (stop_flow(TYPE_NONE)) return;
    LCD_cls(Color);
    Color += 0x10;
  }

  return;
}
