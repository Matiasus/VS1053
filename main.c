/** 
 * --------------------------------------------------------------------------------------+
 * @desc        MAIN VS1053 Driver (VLSI company)
 * --------------------------------------------------------------------------------------+
 *              Copyright (C) 2022 Marian Hrinko.
 *              Written by Marian Hrinko (mato.hrinko@gmail.com)
 *
 * @author      Marian Hrinko
 * @date        19.10.2022
 * @update      12.07.2023
 * @file        main.c
 * @version     1.0.0
 * @tested      AVR Atmega328p
 *
 * @depend      lib/lcd/ssd1306.h, lib/vs1053.h
 * --------------------------------------------------------------------------------------+
 * @interface   SPI connected through 7 pins
 * @pins        5V, DGND, MOSI, DREQ,  XCS
 *              5V, MISO,  SCK, XRST, XDCS 
 *
 * @sources     https://www.vlsi.fi/fileadmin/app_notes/vs10XXan_spi.pdf
 */

// INCLUDE libraries
#include "lib/vs1053.h"
#include "lib/lcd/ssd1306.h"

/**
 * @desc    Main function
 *
 * @param   Void
 *
 * @return  Void
 */
int main (void)
{
  // init LCD SSD1306
  // -------------------------------------------------------------------------------------
  SSD1306_Init ();                                                // init lcd
  SSD1306_ClearScreen ();
  SSD1306_SetPosition (10, 0);
  SSD1306_DrawString ("VS10XX AUDIO CODEC", NORMAL);

  // init MP3 decoder
  // -------------------------------------------------------------------------------------
  SSD1306_SetPosition (1, 2);
  SSD1306_DrawString ("VS10XX init", NORMAL);
  VS1053_Init ();                                                 // init decoder
  SSD1306_SetPosition (103, 2);
  SSD1306_DrawString ("[OK]", NORMAL);

  // mem test
  // -------------------------------------------------------------------------------------
  SSD1306_SetPosition (1, 3);
  SSD1306_DrawString ("VS10XX mem test", NORMAL);
  SSD1306_SetPosition (103, 3);
  if (VS1053_MemTest () == VS1003_MEMTEST_OK) {
    SSD1306_DrawString ("[OK]", NORMAL);
  } else {
    SSD1306_DrawString ("[KO]", NORMAL);
  }

  // sine test
  // -------------------------------------------------------------------------------------
  SSD1306_SetPosition (1, 4);
  SSD1306_DrawString ("VS10XX sine test", NORMAL);
  VS1053_SineTest (VS10XX_FREQ_1kHz);                             // sine test 1kHz
  VS1053_SineTest (VS10XX_FREQ_5kHz);                             // sine test 5kHz
  SSD1306_SetPosition (103, 4);
  SSD1306_DrawString ("[OK]", NORMAL);

  // get version of MP3 decoder
  // -------------------------------------------------------------------------------------
  SSD1306_SetPosition (10, 0);
  SSD1306_DrawString (VS1053_GetVersion (), UNDERLINE);          // print decoder version

  // test hello
  // http://www.vsdsp-forum.com/phpbb/viewtopic.php?t=65
  // ------------------------------------------------------------------------------------- 
  while (1) {
    VS1053_Hello ();      
    _delay_ms (1500);
  }

  // EXIT
  // -------------------------------------------------------------------------------------
  return 0;
}
