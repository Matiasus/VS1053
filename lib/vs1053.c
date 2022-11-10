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

#include "lcd/ssd1306.h"
#include "spi.h"
#include "vs1053.h"

/**
 * @desc    DREQ Wait
 *
 * @param   void
 *
 * @return  uint8_t
 */
inline uint8_t VS1053_DreqWait (void)
{
  while (1) {
    if (IS_BIT_SET (VS1053_PORT, VS1053_DREQ)) {
      return 0;
    }
  }
}

/**
 * @desc    Init
 *
 * @param   void
 *
 * @return  void
 */
void VS1053_Init (void)
{
  // DATA SELECT as output
  SET_BIT (VS1053_DDR, VS1053_XDCS); 
  // DATA REQUEST as input with pullup activate
  CLR_BIT (VS1053_DDR, VS1053_DREQ);
  SET_BIT (VS1053_PORT, VS1053_DREQ);
  // RESET as output
  SET_BIT (VS1053_DDR_RES, VS1053_XRST);

  // SET_BIT (VS1053_DDR, VS1053_MOSI);
  // SET_BIT (VS1053_DDR, VS1053_SCLK);
  // SET_BIT (VS1053_DDR, VS1053_XCS);
  // CLR_BIT (VS1053_DDR, VS1053_MISO)
  SPI_PortInit ();
  // f = fclk/128 = 62500Hz 
  SPI_SlowSpeedInit ();

  // LCD SSD1306
  // ------------------------------------------------- 
  // set position
  SSD1306_SetPosition (0, 1);
  // draw string
  SSD1306_DrawString ("2. SPI init");
  // update
  SSD1306_UpdateScreen (SSD1306_ADDRESS);
  // -------------------------------------------------  

  // init reset routine
  VS1053_Reset ();

  // LCD SSD1306
  // ------------------------------------------------- 
  // set position
  SSD1306_SetPosition (0, 2);
  // draw string
  SSD1306_DrawString ("3. reset init");
  // update
  SSD1306_UpdateScreen (SSD1306_ADDRESS);
  // -------------------------------------------------  

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

  uint8_t h;
  uint8_t l;
  uint16_t mode;
  char str[20];

  mode = VS1053_ReadSci (SCI_VOL);
  l = (uint8_t) (mode >> 8);
  h = (uint8_t) (mode & 0xff);
  // set position
  SSD1306_SetPosition (0, 3);
  //
  sprintf (str, "4. MODE:%d%d", h,l);
  // draw string
  SSD1306_DrawString (str);
  // update
  SSD1306_UpdateScreen (SSD1306_ADDRESS);
  // -------------------------------------------------

/*

  // 
  // 8 4 2 1 | 8 4 2 1 | 8 4 2 1 | 8 4 2 1
  // 1 0 0 1   1 1 0 0   1 1 0 0   1 1 0 0
  // SC_MULT = 4 (XTALI x 3.5)
  // SC_ADD  = 3 (XTALI x 2.0)
  // SC_FREQ = 0x4CC = 1228
  // XTALI = 1228 * 4000 + 8000000 = 1291200Hz
  VS1053_WriteSci (SCI_CLOCKF, 0x9CCC);
  // Wait until DREQ is high
  VS1053_DreqWait ();

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
  // f = fclk/16 * 2 = 1MHz
  SPI_FastSpeedInit ();
*/
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
  VS1053_DreqWait ();

  // 
  // 0x9ccc in source code
  VS1053_WriteSci (SCI_CLOCKF, 0x9CCC);
  // delay
  _delay_ms (1);
  // Wait until DREQ is high
  VS1053_DreqWait ();

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
 * @src     https://www.vlsi.fi/player_vs1011_1002_1003/modularplayer/vs10xx_8c-source.html
 *
 * @param   void
 *
 * @return  void
 */  
void VS1053_TestSdi (void) 
{
  // sine wave
/*
  uint8_t datain[] = {0x53, 0xEF, 0x6E, 0x44, 0, 0, 0, 0};
  uint8_t dataout[] = {0x45, 0x78, 0x69, 0x74, 0, 0, 0, 0};
*/
  CLR_BIT (VS1053_PORT_RES, VS1053_XRST);   // Activate XRST
  _delay_ms (100);                          // delay 100ms
  SPI_WriteByte (0xFF);                     // Send dummy SPI byte to initialize SPI
 
  // Un-reset MP3 chip
  SET_BIT (VS1053_PORT, VS1053_XCS);        // Deactivate xCS
  SET_BIT (VS1053_PORT, VS1053_XDCS);       // Deactivate xDCS
  SET_BIT (VS1053_PORT_RES, VS1053_XRST);   // Deactivate XRST
  _delay_ms (100);                          // delay 100ms

  // Sine test command / VS10xx Application Notes, chapter 4.8
  CLR_BIT (VS1053_PORT, VS1053_XCS);        // Activate xCS
  SPI_WriteByte (VS1053_WRITE);             // Write command code
  SPI_WriteByte (SCI_MODE);                 // SCI MODE register
  SPI_WriteByte (0x08);                     // High byte
  SPI_WriteByte (0x20);                     // Low byte
  SET_BIT (VS1053_PORT, VS1053_XCS);        // Deactivate xCS
  VS1053_DreqWait ();                       // Wait until DREQ is high
  
  // Send a Sine Test Header to Data port
  CLR_BIT (VS1053_PORT, VS1053_XDCS);       // Activate xDCS 
  SPI_WriteByte (0x53);                     // Write sine sound
  SPI_WriteByte (0xef);                     // 
  SPI_WriteByte (0x6e);                     // 
  SPI_WriteByte (0x44);                     //
  SPI_WriteByte (0x00);                     //
  SPI_WriteByte (0x00);                     // 
  SPI_WriteByte (0x00);                     // 
  SPI_WriteByte (0x00);                     //
//  SPI_ReadByte ();                        // ???  
  SET_BIT (VS1053_PORT, VS1053_XDCS);       // Deactivate xDCS
  _delay_ms (500);                          // delay 500ms
  
  // Stop a Sine Test
  CLR_BIT (VS1053_PORT, VS1053_XDCS);       // Activate xDCS 
  SPI_WriteByte (0x45);                     // Write sine sound
  SPI_WriteByte (0x78);                     // 
  SPI_WriteByte (0x69);                     // 
  SPI_WriteByte (0x74);                     //
  SPI_WriteByte (0x00);                     //
  SPI_WriteByte (0x00);                     // 
  SPI_WriteByte (0x00);                     // 
  SPI_WriteByte (0x00);                     //
//  SPI_ReadByte ();                        // ???  
  SET_BIT (VS1053_PORT, VS1053_XDCS);       // Deactivate xDCS
  _delay_ms (500);                          // delay 500ms
  
  /*
  while (1)
  {
    // Activate xCS
    CLR_BIT (VS1053_PORT, VS1053_XCS);
    // command
    VS1053_WriteSci (0x00, 0x0820);
    // Deactivate xCS
    SET_BIT (VS1053_PORT, VS1053_XCS);
    // Wait until DREQ is high
    VS1053_DreqWait ();

    // Activate xCS
    CLR_BIT (VS1053_PORT, VS1053_XCS);
    // data in
    VS1053_WriteSdi (datain, 8);
    // Wait until DREQ is high
    VS1053_DreqWait ();
    // Deactivate xCS
    SET_BIT (VS1053_PORT, VS1053_XCS);
    // delay
    _delay_ms (500);

    // Activate xCS
    CLR_BIT (VS1053_PORT, VS1053_XCS);
    // data out
    VS1053_WriteSdi (dataout, 4);
    // Wait until DREQ is high
    VS1053_DreqWait ();
    // Deactivate xCS
    SET_BIT (VS1053_PORT, VS1053_XCS);
    // delay
    _delay_ms (500);
  }
  */
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
    VS1053_WriteSci (SCI_VOL, 0x0000);
    // Deactivate xCS               
    SET_BIT (VS1053_PORT, VS1053_XCS);
    // delay
    _delay_ms (500);
    // Activate xCS           
    CLR_BIT (VS1053_PORT, VS1053_XCS);
    // Set full volume
    VS1053_WriteSci (SCI_VOL, 0xFFFF);
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
  VS1053_DreqWait ();

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
  VS1053_DreqWait ();
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
  VS1053_DreqWait ();
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
