/**
 * --------------------------------------------------------------------------------------+
 * @desc        VS1053 Status Info
 * --------------------------------------------------------------------------------------+
 *              Copyright (C) 2022 Marian Hrinko.
 *              Written by Marian Hrinko (mato.hrinko@gmail.com)
 *
 * @author      Marian Hrinko
 * @datum       17.11.2022
 * @file        vs1053_info.h
 * @update      17.11.2022
 * @version     1.0
 * @tested      AVR Atmega328p
 *
 * @depend      
 * --------------------------------------------------------------------------------------+
 * @interface   
 * @pins        
 *
 * @sources     https://www.vlsi.fi/fileadmin/datasheets/vs1053.pdf
 */

#ifndef __VS1053_INFO_H__
#define __VS1053_INFO_H__

  // Includes
  #include <avr/pgmspace.h>

  #define VERS_TEXT_LEN       6
  #define VESR_ITEM_LEN       8

  const char version_0[] PROGMEM = "VS1001"; 
  const char version_1[] PROGMEM = "VS1011"; 
  const char version_2[] PROGMEM = "VS1002"; 
  const char version_3[] PROGMEM = "VS1003"; 
  const char version_4[] PROGMEM = "VS1053"; 
  const char version_5[] PROGMEM = "VS1033"; 
  const char version_6[] PROGMEM = "VS1063"; 
  const char version_7[] PROGMEM = "VS1103";

  const char * const vs10xx_versions[] PROGMEM = {
    version_0, 
    version_1,
    version_2, 
    version_3,
    version_4, 
    version_5,
    version_6, 
    version_7,
  };

#endif
