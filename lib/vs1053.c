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
 * @desc    Init
 *
 * @param   void
 *
 * @return  void
 */
void VS1053_Init (void)
{
  SET_BIT (VS1053_DDR, VS1053_XDCS); 
  CLR_BIT (VS1053_DDR, VS1053_DREQ);
  SET_BIT (VS1053_DDR_RES, VS1053_XRST);

  // SET_BIT (VS1053_DDR, VS1053_MOSI);
  // SET_BIT (VS1053_DDR, VS1053_SCLK);
  // SET_BIT (VS1053_DDR, VS1053_XCS);
  // CLR_BIT (VS1053_DDR, VS1053_MISO)
  SPI_PortInit ();  
  SPI_SlowSpeedInit ();

  // init reset routine
  VS1053_Reset ();
}

/**
 * @desc    Set volume
 *
 * @param   uint8_t
 * @param   uint8_t
 *
 * @return  void
 */
void VS1053_SetVolume (uint8_t left, uint8_t right)
{
  // set volume integer
  uint16_t volume = (left << 8) | right; 
  // send command
  VS1053_WriteSci (SCI_VOL, volume);
}

/**
 * @desc    Hard reset
 * @source  https://www.vlsi.fi/player_vs1011_1002_1003/modularplayer/vs10xx_8c.html#a3
 *
 * @param   void
 *
 * @return  void
 */
void VS1053_Reset (void) 
{
  // Activate XRST
  CLR_BIT (VS1053_PORT_RES, VS1053_XRST);
  // delay
  _delay_ms (1);
  // Send dummy SPI byte to initialize SPI
  SPI_WriteByte (0xFF);
  
  // Un-reset MP3 chip
  // Deactivate xCS
  SET_BIT (VS1053_PORT, VS1053_XCS);
  // Deactivate xDCS
  SET_BIT (VS1053_PORT, VS1053_XDCS);
  // Deactivate XRST
  SET_BIT (VS1053_PORT_RES, VS1053_XRST);
  // set volume - lowest level
  VS1053_SetVolume (0xff,0xff);
  
  // Set clock register 44100Hz, stereo
  SPI_WriteWord (0xAC45);
  // Wait until DREQ is high
  WAIT_IF_BIT_IS_SET (VS1053_PORT, VS1053_DREQ);

  // Slow sample rate for slow analog part startup 10 Hz
  VS1053_WriteSci (SCI_AUDATA, 0x000A);
  // delay
  _delay_ms (100);
  
  // Switch on the analog parts
  VS1053_SetVolume (0xfe,0xfe);
  // 8kHz, mono
  VS1053_WriteSci (SCI_AUDATA, 0x1F41);
  // Set volume level
  VS1053_SetVolume (0x14,0x14);

  // soft reset
  VS1053_SoftReset();
  // switch to fast speed 2 MHz
  SPI_FastSpeedInit ();
}

/**
 * @desc    Soft reset
 * @source  https://www.vlsi.fi/player_vs1011_1002_1003/modularplayer/vs10xx_8c.html#a2
 *
 * @param   void
 *
 * @return  void
 */
void VS1053_SoftReset (void)
{
  // VS10xx native SPI modes, Soft reset
  VS1053_WriteSci (SCI_MODE, 0x0804);
  // delay
  _delay_ms (1);
  // Wait until DREQ is high
  WAIT_IF_BIT_IS_SET (VS1053_PORT, VS1053_DREQ);
  
  // Set clock register 44100Hz, stereo
  // 0x9ccc in source code
  SPI_WriteWord (0xAC45);
  // delay
  _delay_ms (1);
  // Wait until DREQ is high
  WAIT_IF_BIT_IS_SET (VS1053_PORT, VS1053_DREQ);

  // Select data
  CLR_BIT (VS1053_PORT, VS1053_XDCS);
  // Send 4 x 0
  SPI_WriteByte (0);
  SPI_WriteByte (0);
  SPI_WriteByte (0);
  SPI_WriteByte (0);
  // wait for SPI ready to send
  SPI_ReadByte ();  
  // DESelect data
  SET_BIT (VS1053_PORT, VS1053_XDCS);  
}

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
  
  while (1)
  {
    // Activate xCS
    CLR_BIT (VS1053_PORT, VS1053_XCS);
    // command
    VS1053_WriteSci (0x00, 0x0820);
    // Deactivate xCS
    SET_BIT (VS1053_PORT, VS1053_XCS);
    // Wait until DREQ is high
    WAIT_IF_BIT_IS_SET (VS1053_PORT, VS1053_DREQ);

    // Activate xCS
    CLR_BIT (VS1053_PORT, VS1053_XCS);
    // data in
    VS1053_WriteSdi (datain, 8);
    // Wait until DREQ is high
    WAIT_IF_BIT_IS_SET (VS1053_PORT, VS1053_DREQ);
    // Deactivate xCS
    SET_BIT (VS1053_PORT, VS1053_XCS);
    // delay
    _delay_ms (500);

    // Activate xCS
    CLR_BIT (VS1053_PORT, VS1053_XCS);
    // data out
    VS1053_WriteSdi (dataout, 4);
    // Wait until DREQ is high
    WAIT_IF_BIT_IS_SET (VS1053_PORT, VS1053_DREQ);
    // Deactivate xCS
    SET_BIT (VS1053_PORT, VS1053_XCS);
    // delay
    _delay_ms (500);
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
    // Activate xCS
    CLR_BIT (VS1053_PORT, VS1053_XCS);
    // Set full volume
    VS1053_WriteSci (0x0B, 0x0000);
    // Deactivate xCS               
    SET_BIT (VS1053_PORT, VS1053_XCS);
    // delay
    _delay_ms (500);
    // Activate xCS           
    CLR_BIT (VS1053_PORT, VS1053_XCS);
    // Set full volume
    VS1053_WriteSci (0x0B, 0xFFFF);
    // Deactivate xCS           
    SET_BIT (VS1053_PORT, VS1053_XCS);
    // delay
    _delay_ms (500);
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
  // Wait until DREQ is high
  WAIT_IF_BIT_IS_SET (VS1053_PORT, VS1053_DREQ);
  // Activate xCS
  CLR_BIT (VS1053_PORT, VS1053_XCS);
  // Write command code
  SPI_WriteByte (VS1053_WRITE);
  // SCI register number
  SPI_WriteByte (addr);
  // High byte
  SPI_WriteByte ((uint8_t)(data >> 8));
  // Low byte
  SPI_WriteByte ((uint8_t)(data & 0xFF));
  // Deactivate xCS
  SET_BIT (VS1053_PORT, VS1053_XCS);
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
  // Wait until DREQ is high
  WAIT_IF_BIT_IS_SET (VS1053_PORT, VS1053_DREQ);
  // Activate xCS
  CLR_BIT (VS1053_PORT, VS1053_XCS);
  // Read command code
  SPI_WriteByte (VS1053_READ);
  // SCI register number
  SPI_WriteByte (addr);
  // High byte
  data = (uint16_t)SPI_ReadByte() << 8;
  // Low byte
  data |= SPI_ReadByte();
  // Deactivate xCS
  SET_BIT (VS1053_PORT, VS1053_XCS);
  // Return content
  return data;
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

  // Error: Too many bytes to transfer!
  if (bytes > 32) {
    return -1;
  }
  // Wait until DREQ is high
  WAIT_IF_BIT_IS_SET (VS1053_PORT, VS1053_DREQ);
  // Activate xDCS
  CLR_BIT (VS1053_PORT, VS1053_XDCS);
  // send data
  for (i = 0; i < bytes; i++) {
    SPI_WriteByte(*data++);
  }
  // Deactivate xDCS
  SET_BIT (VS1053_PORT, VS1053_XDCS);
  // Success
  return 0;
}
