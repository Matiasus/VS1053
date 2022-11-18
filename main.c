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
  // LCD SSD1306
  // ---------------------------------------
  SSD1306_Init (SSD1306_ADDRESS);

  // print header info 
  SSD1306_ClearScreen ();
  SSD1306_DrawLine (0, MAX_X, 4, 4);
  SSD1306_SetPosition (4, 1);
  SSD1306_DrawString ("VS10XX AUDIO DECODER");
  SSD1306_DrawLine (0, MAX_X, 18, 18);

  // print mp3 init
  SSD1306_SetPosition (1, 3);
  SSD1306_DrawString ("VS10XX init");
  SSD1306_UpdateScreen (SSD1306_ADDRESS);

  // MP3 ENCODER
  // ---------------------------------------
  VS1053_Init ();

  // print mp3 init success
  SSD1306_SetPosition (103, 3);
  SSD1306_DrawString ("[OK]");
  SSD1306_UpdateScreen (SSD1306_ADDRESS);

  // Print to lcd
  SSD1306_SetPosition (1, 4);
  SSD1306_DrawString ("VS10XX test");
  SSD1306_UpdateScreen (SSD1306_ADDRESS);
  
  // sine test
  //VS1053_SineTest ();

  // print sine test success
  SSD1306_SetPosition (103, 4);
  SSD1306_DrawString ("[OK]");
  SSD1306_UpdateScreen (SSD1306_ADDRESS);

  // read sci register
  // ---------------------------------------
  SSD1306_SetPosition (1, 5);
  SSD1306_DrawString ("VS10XX vers  [");
  SSD1306_DrawString (VS1053_GetVersion ());
  SSD1306_DrawString ("]");
  SSD1306_UpdateScreen (SSD1306_ADDRESS);

  // EXIT
  // -------------------------------------------------
  return 0;
}
