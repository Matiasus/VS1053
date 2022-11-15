/**
 * --------------------------------------------------------------------------------------+
 * @desc        VS1053 Driver (VLSI company)
 * --------------------------------------------------------------------------------------+
 *              Copyright (C) 2022 Marian Hrinko.
 *              Written by Marian Hrinko (mato.hrinko@gmail.com)
 *
 * @author      Marian Hrinko
 * @datum       19.10.2022
 * @file        vs1053.c
 * @update      14.11.2022
 * @version     1.0
 * @tested      AVR Atmega328p
 *
 * @depend      vs1053.h
 * --------------------------------------------------------------------------------------+
 * @interface   SPI connected through 7 pins
 * @pins        5V, DGND, MOSI, DREQ,  XCS
 *              5V, MISO,  SCK, XRST, XDCS
 *
 * @sources     https://www.vlsi.fi/fileadmin/datasheets/vs1053.pdf
 *              https://www.vlsi.fi/fileadmin/app_notes/vs10XXan_spi.pdf
 *              https://www.vlsi.fi/player_vs1011_1002_1003/modularplayer/files.html
 *              https://github.com/adafruit/Adafruit_VS1053_Library
 *              https://os.mbed.com/users/silis/code/VS1053//file/5ad25d480d5f/VS1053.cpp/
 */

#include "lcd/ssd1306.h"
#include "vs1053.h"

/**
 * @desc    Activate Command / clear XCS
 *
 * @param   void
 *
 * @return  void
 */
static inline uint8_t VS1053_ActivateCommand (void) { VS1053_PORT &= ~(1 << VS1053_XCS); }

/**
 * @desc    Deactivate Command / set XCS
 *
 * @param   void
 *
 * @return  void
 */
static inline uint8_t VS1053_DeactivateCommand (void) { VS1053_PORT |= (1 << VS1053_XCS); }

/**
 * @desc    Activate Data / clear XDCS
 *
 * @param   void
 *
 * @return  void
 */
static inline uint8_t VS1053_ActivateData (void) { VS1053_PORT &= ~(1 << VS1053_XDCS); }

/**
 * @desc    Deactivate Data / set XDCS
 *
 * @param   void
 *
 * @return  void
 */
static inline uint8_t VS1053_DeactivateData (void) { VS1053_PORT |= (1 << VS1053_XDCS); }

/**
 * @desc    Activate RESET / clear XRST
 *
 * @param   void
 *
 * @return  void
 */
static inline uint8_t VS1053_ActivateReset (void) { VS1053_PORT_RES &= ~(1 << VS1053_XRST); }

/**
 * @desc    Deactivate RESET / set XRST
 *
 * @param   void
 *
 * @return  void
 */
static inline uint8_t VS1053_DeactivateReset (void) { VS1053_PORT_RES |= (1 << VS1053_XRST); }

/**
 * @desc    DREQ Wait
 *
 * @param   void
 *
 * @return  uint8_t
 */
static inline uint8_t VS1053_DreqWait (void)
{
  while (1) {
    if (VS1053_PORT & (1 << VS1053_DREQ)) {
      return 0;
    }
  }
}

/**
 * @desc    Write Serial Command Instruction / big endian /
 *
 * @param   uint8_t addr
 * @param   uint16_t command
 *
 * @return  void
 */
void VS1053_WriteSci (uint8_t addr, uint16_t cmnd)
{
  VS1053_DreqWait ();                       // wait until DREQ is high
  VS1053_ActivateCommand ();                // clear xCS
  SPI_WriteByte (VS1053_WRITE);             // command code for WRITE
  SPI_WriteByte (addr);                     // SCI register number
  SPI_WriteByte ((uint8_t)(cmnd >> 8));     // high byte
  SPI_WriteByte ((uint8_t)(cmnd & 0xFF));   // low byte
  VS1053_DeactivateCommand ();              // set xCS
}

/**
 * @desc    Read Serial Command Instruction / big endian /
 *
 * @param   uint8_t addr
 *
 * @return  uint16_t
 */
uint16_t VS1053_ReadSci (uint8_t addr)
{
  uint16_t data;

  VS1053_DreqWait ();                       // wait until DREQ is high
  VS1053_ActivateCommand ();                // clear xCS
  SPI_WriteByte (VS1053_READ);              // command code for READ
  SPI_WriteByte (addr);                     // SCI register number
  data = (uint16_t)SPI_ReadByte() << 8;     // high byte
  data |= SPI_ReadByte();                   // low byte
  VS1053_DeactivateCommand ();              // set xCS

  return data;                              // return content
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
  while (1)
  {
    VS1053_ActivateCommand ();              // clear xCS
    VS1053_WriteSci (SCI_VOL, 0x0000);      // set full volume
    VS1053_DeactivateCommand ();            // set xCS
    _delay_ms (500);                        // delay

    VS1053_ActivateCommand ();              // clear xCS
    VS1053_WriteSci (SCI_VOL, 0xFFFF);      // set full volume
    VS1053_DeactivateCommand ();            // set xCS
    _delay_ms (500);                        // delay
  }
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

  if (bytes > 32) {                         // error: Too many bytes to transfer!
    return -1;
  }

  VS1053_DreqWait ();                       // wait until DREQ is high
  VS1053_ActivateData ();                   // clear xDCS
  for (i = 0; i < bytes; i++) {             // send data
    SPI_WriteByte (*data++);                //
  }                                         //
  VS1053_DeactivateData ();                 // set xDCS

  return 0;                                 // success
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
  VS1053_ActivateReset ();                  // clear XRST
  _delay_ms (2);                            // after a hardware reset (or at power-up) DREQ will stay down for around 22000 clock cycles,
                                            // which means an approximate 1.8 ms delay if VS1053b is run at 12.288 MHz
  SPI_WriteByte (0xFF);                     // send dummy SPI byte to initialize SPI

  // Un-reset MP3 chip
  VS1053_DeactivateCommand ();              // set xCS
  VS1053_DeactivateData ();                 // set xDCS
  VS1053_DeactivateReset ();                // set XRST
  VS1053_SetVolume (0xff,0xff);             // activate analog powerdown mode

  // SCI_CLOCKF register
  // ---------------------------------------
  //
  // EXAMPLE: 0x9CCC
  // ---------------------------------------
  // 8 4 2 1 | 8 4 2 1 | 8 4 2 1 | 8 4 2 1
  // 1 0 0 1   1 1 0 0   1 1 0 0   1 1 0 0
  //
  // SC_MULT = 4 (XTALI x 3.5)
  // SC_ADD  = 3 (XTALI x 2.0)
  // SC_FREQ = 0x4CC = 1228
  // XTALI = 1228*4000+8000000 = 1291200Hz
  //
  // EXAMPLE: 0x9800
  // ---------------------------------------
  // 8 4 2 1 | 8 4 2 1 | 8 4 2 1 | 8 4 2 1
  // 1 0 0 1   1 0 0 0   0 0 0 0   0 0 0 0
  //
  // SC_MULT = 4 (XTALI x 3.5)
  // SC_ADD  = 3 (XTALI x 2.0)
  // SC_FREQ = 0 then XTALI = 12.288 MHz
  //   12.288MHz * 3.5 and
  //   12.288MHz * 5.5 if more cycles are temporarily needed to decode a WMA or AAC stream
  VS1053_WriteSci (SCI_CLOCKF, 0x9800);     // vlsi sulution 0x9CCC (XTALI=1291200Hz)
  VS1053_DreqWait ();                       // wait until DREQ is high

  VS1053_WriteSci (SCI_AUDATA, 0x000A);     // slow sample rate for slow analog part startup 10 Hz
  _delay_ms (100);                          // delay

  VS1053_SetVolume (0xfe,0xfe);             // switch on the analog parts
  VS1053_WriteSci (SCI_AUDATA, 0x1F41);     // 8kHz, mono
  VS1053_SetVolume (0x66,0x66);             // Set volume level

  VS1053_SoftReset();                       // soft reset
  SPI_FastSpeedInit ();                     // f = fclk/16 * 2 = 1MHz
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
  VS1053_WriteSci (SCI_MODE, 0x0804);       // VS10xx native SPI modes, Soft reset
  _delay_ms (1);                            // delay
  VS1053_DreqWait ();                       // wait until DREQ is high

  VS1053_WriteSci (SCI_CLOCKF, 0x9800);     // set clock cycles / 0x9ccc in source code
  _delay_ms (1);                            // delay
  VS1053_DreqWait ();                       // wait until DREQ is high

  VS1053_ActivateData ();                   // clear xDCS / Select data
  SPI_WriteByte (0);                        // send 4 x 0
  SPI_WriteByte (0);
  SPI_WriteByte (0);
  SPI_WriteByte (0);
  //SPI_ReadByte ();                        // wait for SPI ready to send
  VS1053_DeactivateData ();                 // set xDCS / Deselect data
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
  VS1053_DDR |= (1 << VS1053_XDCS);         // DATA SELECT as output
  VS1053_DDR &= ~(1 << VS1053_DREQ);        // DATA REQUEST as input
  VS1053_PORT |= (1 << VS1053_DREQ);        // DATA REQUEST pullup activate
  VS1053_DDR_RES |= (1 << VS1053_XRST);     // RESET as output

  SPI_PortInit ();                          // output={MOSI;SCLK;CS} input={MISO}
  SPI_SlowSpeedInit ();                     // f = fclk/128 = 62500Hz

  VS1053_Reset ();                          // init reset routine
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
  uint16_t volume = (left << 8) | right;    // set volume integer
  VS1053_WriteSci (SCI_VOL, volume);        // send command
}

/**
 * @desc    Test SDI - sine test
 * @src     https://www.vlsi.fi/player_vs1011_1002_1003/modularplayer/vs10xx_8c-source.html
 *
 * @param   void
 *
 * @return  void
 */
void VS1053_SineTest (void)
{
  uint8_t sine_activate[] = {0x53, 0xEF, 0x6E, 0x44, 0, 0, 0, 0};
  uint8_t sine_deactivate[] = {0x45, 0x78, 0x69, 0x74, 0, 0, 0, 0};

/*
  // use only if no reset is defined before
  VS1053_ActivateReset ();                  // clear XRST
  _delay_ms (100);                          // delay 100ms
  SPI_WriteByte (0xFF);                     // send dummy SPI byte to initialize SPI

  // Un-reset MP3 chip
  VS1053_DeactivateCommand ();              // set xCS
  VS1053_DeactivateData ();                 // set xDCS
  VS1053_DeactivateReset ();                // set XRST
  _delay_ms (100);                          // delay 100ms
*/

  VS1053_ActivateCommand ();                // clear xCS
  VS1053_WriteSci (0x00, 0x0820);           // SCI_MODE
  VS1053_DeactivateCommand ();              // set xCS
  VS1053_DreqWait ();                       // Wait until DREQ is high

  // sine wave sequence start
  VS1053_ActivateCommand ();                // clear xCS
  VS1053_WriteSdi (sine_activate, 8);       // Sine wave data activate
  VS1053_DreqWait ();                       // Wait until DREQ is high
  VS1053_DeactivateCommand ();              // set xCS
  _delay_ms (500);                          // delay

  // sine wave sequence stop
  VS1053_ActivateCommand ();                // clear xCS
  VS1053_WriteSdi (sine_deactivate, 8);     // Sine wave data deactivate
  VS1053_DreqWait ();                       // Wait until DREQ is high
  VS1053_DeactivateCommand ();              // set xCS
  _delay_ms (500);                          // delay
}
