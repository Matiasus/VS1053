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

/**
 * @desc    Main function
 *
 * @param   Void
 *
 * @return  Void
 */
int main (void)
{
  // init VS1053
  VS1053_Init ();

  // EXIT
  // ------------------------------------------------- 
  // return & exit
  return 0;
}
