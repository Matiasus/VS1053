/** 
 * --------------------------------------------------------------------------------------+
 * @brief       SAY HELLO / VS1053 Driver (VLSI company)
 * --------------------------------------------------------------------------------------+
 *              Copyright (C) 2022 Marian Hrinko.
 *              Written by Marian Hrinko (mato.hrinko@gmail.com)
 *
 * @author      Marian Hrinko
 * @date        19.10.2022
 * @file        main.c
 * @version     1.0.0
 * @test        AVR Atmega328p
 *
 * @depend      lib/vs1053.h, lib/vs1053_hello.h
 * --------------------------------------------------------------------------------------+
 * @interface   SPI connected through 7 pins
 * @pins        5V, DGND, MOSI, DREQ,  XCS
 *              5V, MISO,  SCK, XRST, XDCS
 *
 * @sources     
 */

// INCLUDE libraries
#include "lib/lcd/ssd1306.h"
#include "lib/vs1053.h"
#include "lib/vs1053_hello.h"

/**
 * @desc    Main function
 *
 * @param   Void
 *
 * @return  Int
 */
int main (void)
{
  uint16_t data;

  // init LCD SSD1306
  // -------------------------------------------------------------------------------------
  SSD1306_Init (SSD1306_ADDR);
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
  SSD1306_DrawString ("VS10XX memtest", NORMAL);
  SSD1306_SetPosition (103, 3);
  data = VS1053_TestMemory ();   
  if (data != VS1053_MEMTEST_OK) { 
    return 0;                                                     // mem test fail
  }
  SSD1306_DrawString ("[OK]", NORMAL);

  // sine test
  // -------------------------------------------------------------------------------------
  SSD1306_SetPosition (1, 4);
  SSD1306_DrawString ("VS10XX sinetest", NORMAL);
  VS1053_TestSine (VS10XX_FREQ_1kHz);                             // sine test 1kHz
  VS1053_TestSine (VS10XX_FREQ_5kHz);                             // sine test 5kHz
  SSD1306_SetPosition (103, 4);
  SSD1306_DrawString ("[OK]", NORMAL);

  // get version of MP3 decoder
  // -------------------------------------------------------------------------------------
  SSD1306_SetPosition (10, 0);
  SSD1306_DrawString (VS1053_GetVersion (), UNDERLINE);           // print version
  
  // test say hello
  // http://www.vsdsp-forum.com/phpbb/viewtopic.php?t=65
  // -------------------------------------------------------------------------------------
  SSD1306_SetPosition (1, 5);
  SSD1306_DrawString ("VS10XX say hello", NORMAL);
  SSD1306_SetPosition (103, 5);
  SSD1306_DrawString ("[OK]", NORMAL);
  while (1) {
    VS1053_TestSample (HelloMP3, sizeof(HelloMP3)-1);             // say Hello
  }

  // EXIT
  // -------------------------------------------------------------------------------------
  return 0;
}
