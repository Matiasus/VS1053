/** 
 * ---------------------------------------------------------------+ 
 * @desc        MAIN VS1053 Driver (VLSI company)
 * ---------------------------------------------------------------+ 
 *              Copyright (C) 2022 Marian Hrinko.
 *              Written by Marian Hrinko (mato.hrinko@gmail.com)
 *
 * @author      Marian Hrinko
 * @datum       19.10.2022
 * @file        main.c
 * @update      23.10.2022
 * @version     1.0
 * @tested      AVR Atmega328p
 *
 * @depend      spi.h, vs1053.h
 * ---------------------------------------------------------------+
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
  //char str[10];

  // LCD SSD1306
  // ---------------------------------------
  SSD1306_Init ();

  // print header info 
  SSD1306_ClearScreen ();
  SSD1306_SetPosition (3, 0);
  SSD1306_DrawString ("VS10XX AUDIO DECODER", NORMAL);

  // print mp3 init
  SSD1306_SetPosition (1, 3);
  SSD1306_DrawString ("VS10XX init", NORMAL);

  // MP3 ENCODER
  // ---------------------------------------
  VS1053_Init ();

  // print mp3 init success
  SSD1306_SetPosition (103, 3);
  SSD1306_DrawString ("[OK]", NORMAL);

  // Print to lcd
  SSD1306_SetPosition (1, 4);
  SSD1306_DrawString ("VS10XX test", NORMAL);
  
  // sine test 1kHz
  VS1053_SineTest (FREQ_1kHz);
  // sine test 5kHz
  VS1053_SineTest (FREQ_5kHz);

  // print sine test success
  SSD1306_SetPosition (103, 4);
  SSD1306_DrawString ("[OK]", NORMAL);

  // read sci register
  // ---------------------------------------
  SSD1306_SetPosition (1, 5);
  SSD1306_DrawString ("VS10XX vers  [", NORMAL);
  SSD1306_DrawString (VS1053_GetVersion (), NORMAL);
  SSD1306_DrawString ("]", NORMAL);
/*
  // read sci register
  // ---------------------------------------
  SSD1306_SetPosition (1, 6);
  SSD1306_DrawString ("VS10XX mem   [", NORMAL);
  sprintf (str, "0x%x", VS1053_memTest (), NORMAL);
  SSD1306_DrawString ("]", NORMAL);
*/
  // EXIT
  // -------------------------------------------------
  return 0;
}
