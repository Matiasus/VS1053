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
  uint8_t addr = SSD1306_ADDRESS;

  // LCD SSD1306
  // ------------------------------------------------- 
  // init ssd1306
  SSD1306_Init (addr);
  // clear screen
  SSD1306_ClearScreen ();
  // set position
  SSD1306_SetPosition (0, 0);
  // draw string
  SSD1306_DrawString ("ssd1306 lcd init...");
  // update
  SSD1306_UpdateScreen (addr); 

  // MP3 ENCODER
  // -------------------------------------------------  
  // init VS1053
  VS1053_Init ();
  // set position
  SSD1306_SetPosition (0, 1);
  // draw string
  SSD1306_DrawString ("mp3 reset init..");
  // update
  SSD1306_UpdateScreen (addr);

  // EXIT
  // ------------------------------------------------- 
  // return & exit
  return 0;
}
