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
  // *************************************************
  SSD1306_Init (SSD1306_ADDRESS);

  // ------------------------------------------------- 
  // clear screen
  SSD1306_ClearScreen ();
  // set position
  SSD1306_SetPosition (0, 0);
  // draw string
  SSD1306_DrawString ("1. lcd init");
  // update
  SSD1306_UpdateScreen (SSD1306_ADDRESS); 
  // ------------------------------------------------- 

  // MP3 ENCODER
  // *************************************************
  VS1053_Init ();

  VS1053_TestSdi ();

  // EXIT
  // ------------------------------------------------- 
  // return & exit
  return 0;
}
