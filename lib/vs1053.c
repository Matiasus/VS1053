/** 
 * ---------------------------------------------------------------+ 
 * @desc        VS1053 Driver (VLSI company)
 * ---------------------------------------------------------------+ 
 *              Copyright (C) 2022 Marian Hrinko.
 *              Written by Marian Hrinko (mato.hrinko@gmail.com)
 *
 * @author      Marian Hrinko
 * @datum       19.10.2022
 * @file        vs1053.c
 * @update      23.10.2022
 * @version     1.0
 * @tested      AVR Atmega328p
 *
 * @depend      
 * ---------------------------------------------------------------+
 * @interface   SPI connected through 7 pins
 * @pins        5V, DGND, MOSI, DREQ,  XCS
 *              5V, MISO,  SCK, XRST, XDCS 
 *
 * @sources     https://www.vlsi.fi/fileadmin/app_notes/vs10XXan_spi.pdf
 */

#include "spi.h"
#include "vs1053.h"

/**
 * @desc    Test SDI - sine test
 *
 * @param   void
 *
 * @return  void
 */  
void VS1053_TestSdi (void) 
{
  uint8_t datain[] = {0x53, 0xEF, 0x6E, 0x30, 0, 0, 0, 0};
  uint8_t dataout[] = {0x45, 0x78, 0x69, 0x74};  
  
  while (1) {
    CLR_BIT (VS1053_PORT, XCS);          // Activate xCS
    VS1053_WriteSci (0x00, 0x0820)       // 
    SET_BIT (VS1053_PORT, XCS);          // Deactivate xCS
    wait for dreq up
    
    CLR_BIT (VS1053_PORT, XCS);          // Activate xCS
    VS1053_WriteSdi (*datain, 8);        // 
    wait for dreq up
    SET_BIT (VS1053_PORT, XCS);          // Deactivate xCS
    _delay_ms(500);                      // delay 500ms
    
    CLR_BIT (VS1053_PORT, XCS);          // Activate xCS
    VS1053_WriteSdi (*dataout, 4);       // 
    wait for dreq up
    SET_BIT (VS1053_PORT, XCS);          // Deactivate xCS    
    _delay_ms(500);                      // delay 500ms
  }  
}

/**
 * @desc    Test SCI
 *
 * @param   void
 *
 * @return  void
 */  
void VS1053_TestSci (void) 
{
  while (1) {
    CLR_BIT (VS1053_PORT, XCS);          // Activate xCS
    VS1053_WriteSci (0x0B, 0x0000)       // Set full volume
    SET_BIT (VS1053_PORT, XCS);          // Deactivate xCS
    _delay_ms(500);                      // delay 500ms
    CLR_BIT (VS1053_PORT, XCS);          // Activate xCS
    VS1053_WriteSci (0x0B, 0xFFFF)       // Set full volume
    SET_BIT (VS1053_PORT, XCS);          // Deactivate xCS
    _delay_ms(500);                      // delay 500ms
  }
}

/**
 * @desc    Write Serial Command Instruction
 *
 * @param   uint8_t addr
 * @param   uint16_t command
 *
 * @return  void
 */
void VS1053_WriteSci (uint8_t addr, uint16_t data) 
{
  WAIT_IF_BIT_IS_SET(VS1053_PORT, DREQ);// Wait until DREQ is high
  CLR_BIT(VS1053_PORT, XCS);            // Activate xCS
  SPI_WriteByte(WRITE);                 // Write command code
  SPI_WriteByte(addr);                  // SCI register number
  SPI_WriteByte((uint8_t)(data >> 8));  // High byte
  SPI_WriteByte((uint8_t)(data & 0xFF));// Low byte
  SET_BIT(VS1053_PORT, XCS);            // Deactivate xCS
}

/**
 * @desc    Read Serial Command Instruction
 *
 * @param   uint8_t addr
 *
 * @return  uint16_t
 */
uint16_t VS1053_ReadSci (uint8_t addr) 
{
  uint16_t data;
  WAIT_IF_BIT_IS_SET(VS1053_PORT, DREQ);// Wait until DREQ is high
  CLR_BIT(VS1053_PORT, XCS);            // Activate xCS
  SPI_WriteByte(READ);                  // Read command code
  SPI_WriteByte(addr);                  // SCI register number
  data = (uint16_t)SPI_ReadByte() << 8; // High byte 
  data |= SPI_ReadByte();               // Low byte
  SET_BIT(VS1053_PORT, XCS);            // Deactivate xCS
  return data;                          // Return content
}

/**
 * @desc    Write Serial Data
 *
 * @param   uint8_t * data
 * @param   uint8_t bytes
 *
 * @return  int
 */  
int VS1053_WriteSdi (const uint8_t *data, uint8_t bytes) 
{
  uint8_t i;
  
  if (bytes > 32) {                     // Error: Too many bytes to transfer!
    return -1;
  }
     
  WAIT_IF_BIT_IS_SET(VS1053_PORT, DREQ);// Wait until DREQ is high
  CLR_BIT(VS1053_PORT, XDCS);           // Activate xDCS
  for (i = 0; i < bytes; i++) {
    SPI_WriteByte(*data++);
  }
  SET_BIT(VS1053_PORT, XDCS);           // Deactivate xDCS
  
  return 0;                             // Success
}

/**
 * @desc    Set DDR
 *
 * @param   void
 *
 * @return  void
 */
void VS1053_SetDDR (void)
{
  // OUTPUT 
  SET_BIT(VS1053_DDR, MOSI);
  SET_BIT(VS1053_DDR, SCLK);
  SET_BIT(VS1053_DDR, XCS);
  SET_BIT(VS1053_DDR, XDCS);
  
  // INPUT
  CLR_BIT(VS1053_DDR, DREQ);
  CLR_BIT(VS1053_DDR, MISO);
}  

