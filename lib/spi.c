/** 
 * ---------------------------------------------------------------+ 
 * @desc        SPI (Master Mode)
 * ---------------------------------------------------------------+ 
 *              Copyright (C) 2022 Marian Hrinko.
 *              Written by Marian Hrinko (mato.hrinko@gmail.com)
 *
 * @author      Marian Hrinko
 * @datum       21.10.2022
 * @file        spi.c
 * @update      09.11.2022
 * @version     1.0
 * @tested      AVR Atmega328p
 *
 * @depend      
 * ---------------------------------------------------------------+
 * @interface   SPI master mode
 * @pins        SCLK, MOSI, MISO, CS (SS) 
 *
 * @sources    
 */

/* INCLUDE libraries */
#include "spi.h"

/**
 * @desc    SPI Port Init
 *
 * @param   void
 *
 * @return  void
 */
void SPI_PortInit (void)
{
  SET_BIT (SPI_DDR, SPI_SS);       // as OUTPUT
  SET_BIT (SPI_DDR, SPI_SCK);      // as OUTPUT
  SET_BIT (SPI_DDR, SPI_MOSI);     // as OUTPUT
  CLR_BIT (SPI_DDR, SPI_MISO);     // as INTPUT
  SET_BIT (SPI_PORT, SPI_MISO);    // pullup activate
}

/**
 * @desc    SPI Slow Speed Init
 *
 * @param   void
 *
 * @return  void
 */
void SPI_SlowSpeedInit (void)
{
  SPI_SPCR = (1<<SPE) | (1<<MSTR) | (1<<SPR1) | (1<<SPR0);
/*
  // NOTE: SPE must be set as last
  SET_BIT (SPI_SPCR, MSTR);        // Master device
  SET_BIT (SPI_SPCR, SPR0);        // 
  SET_BIT (SPI_SPCR, SPR1);        // Prescaler fclk/128 = 62500Hz
  SET_BIT (SPI_SPCR, SPE);         // SPI Enale, note: writing a byte to the SPI data reg starts the SPI clock generator
*/
}

/**
 * @desc    SPI Fast Speed Init
 *
 * @param   void
 *
 * @return  void
 */
void SPI_FastSpeedInit (void)
{
  SPI_SPSR = (1<<SPI2X);
  SPI_SPCR = (1<<SPE) | (1<<MSTR) | (1<<SPR0);
/*
  // NOTE: SPE must be set as last
  SET_BIT (SPI_SPCR, MSTR);        // Master device
  SET_BIT (SPI_SPCR, SPR0);        // 
  CLR_BIT (SPI_SPCR, SPR1);        // f = fclk/16 = 0.5MHz
  SET_BIT (SPI_SPSR, SPI2X);       // f * 2 = 1MHz
  SET_BIT (SPI_SPCR, SPE);         // SPI Enale, note: writing a byte to the SPI data reg starts the SPI clock generator
*/
}

/**
 * @desc    SPI Write Byte
 *
 * @param   uint8_t
 *
 * @return  void
 */   
void SPI_WriteByte (uint8_t data)
{
  // start transmission
  SPI_SPDR = data;
  // wait for transmission complete
  while(!(SPI_SPSR & (1<<SPIF)))
  ;
}

/**
 * @desc    SPI Write Word / big endian (MSB Byte first)
 *
 * @param   uint16_t
 *
 * @return  void
 */   
void SPI_WriteWord (uint16_t data)
{
  // high byte
  SPI_WriteByte ((uint8_t)(data >> 8));
  // low byte
  SPI_WriteByte ((uint8_t)(data & 0xFF));
}
 
/**
 * @desc    SPI Read Byte
 *
 * @param   void
 *
 * @return  uint8_t
 */   
 uint8_t SPI_ReadByte (void)
 {
  // start transmission, dummy byte
  SPI_SPDR = 0xFF;
  // wait for transmission complete
  while(!(SPI_SPSR & (1<<SPIF)))
  ;
  // return received data
  return SPI_SPDR;
 }
 
/**
 * @desc    SPI Write / Read Byte
 *
 * @param   uint8_t
 *
 * @return  uint8_t
 */   
 uint8_t SPI_WriteReadByte (uint8_t data)
 {
  // start transmission
  SPI_SPDR = data;
  // wait for transmission complete
  while(!(SPI_SPSR & (1<<SPIF)))
  ;
  // return received data
  return SPI_SPDR;
 }
