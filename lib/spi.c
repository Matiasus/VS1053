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
 * @update      23.10.2022
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
 * @desc    SPI Master Mode Init
 *
 * @param   void
 *
 * @return  void
 */
void SPI_MasterInit (void)
{
  SET_BIT (SPI_DDR, SPI_SS);       // as OUTPUT
  SET_BIT (SPI_DDR, SPI_SCK);      // as OUTPUT
  SET_BIT (SPI_DDR, SPI_MOSI);     // as OUTPUT
  CLR_BIT (SPI_DDR, SPI_MISO);     // as INTPUT
  
  SET_BIT (SPI_SPCR, SPE);         // SPE  - SPI Enale, note: writing a byte to the SPI data reg starts the SPI clock generator,
  SET_BIT (SPI_SPCR, MSTR);        // MSTR - Master device
  SET_BIT (SPI_SPCR, SPR0);        // SPEED - Prescaler fclk/16 = 1MHz
  SET_BIT (SPI_SPCR, SPI2X);       // SPI2X - SPEED * 2 = 2MHz
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
  SPI_SPDR = data;                // Start transmission
  while(!(SPI_SPSR & (1<<SPIF))); // Wait for transmission complete
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
  SPI_SPDR = 0xFF;                // Start transmission, fill dummy byte
  while(!(SPI_SPSR & (1<<SPIF))); // Wait for transmission complete

  return SPI_SPDR;                // Read received data
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
  SPI_SPDR = data;                // Start transmission
  while(!(SPI_SPSR & (1<<SPIF))); // Wait for transmission complete          
  
  return SPI_SPDR;                // Read received data
 }
