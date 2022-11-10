/** 
 * ---------------------------------------------------------------+ 
 * @desc        VS1053 Driver (VLSI company)
 * ---------------------------------------------------------------+ 
 *              Copyright (C) 2022 Marian Hrinko.
 *              Written by Marian Hrinko (mato.hrinko@gmail.com)
 *
 * @author      Marian Hrinko
 * @datum       19.10.2022
 * @file        vs1053.h
 * @update      21.10.2022
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

#ifndef __VS1053_H__
#define __VS1053_H__

  #include <avr/io.h>
  #include <util/delay.h>
  #include "spi.h"

  // PORT
  #define VS1053_DDR          SPI_DDR
  #define VS1053_PORT         SPI_PORT

  // PIN
  #define VS1053_SCK          SPI_SCK
  #define VS1053_MOSI         SPI_MOSI
  #define VS1053_MISO         SPI_MISO
  #define VS1053_XCS          SPI_SS       
  #define VS1053_DREQ         1
  #define VS1053_XDCS         0

  // RESET
  #define VS1053_DDR_RES      DDRD
  #define VS1053_PORT_RES     PORTD
  #define VS1053_XRST         7

  // REGISTERS
  #define SCI_MODE            0x0  // Mode control  
  #define SCI_STATUS          0x1  // Status of VS1053b 
  #define SCI_BASS            0x2  // Built-in bass/treble control 
  #define SCI_CLOCKF          0x3  // Clock freq + multiplier
  #define SCI_DECODE_TIME     0x4  // Decode time in seconds
  #define SCI_AUDATA          0x5  // Misc. audio data 
  #define SCI_WRAM            0x6  // RAM write/read  
  #define SCI_WRAMADDR        0x7  // Base address for RAM write/read
  #define SCI_HDAT0           0x8  // Stream header data 0
  #define SCI_HDAT1           0x9  // Stream header data 1
  #define SCI_AIADDR          0xA  // Start address of application
  #define SCI_VOL             0xB  // Volume control  
  #define SCI_AICTRL0         0xC  // Application control register 0
  #define SCI_AICTRL1         0xD  // Application control register 1
  #define SCI_AICTRL2         0xE  // Application control register 2
  #define SCI_AICTRL3         0xF  // Application control register 3

  // reg SCI_MODE
  // SCI_MODE is used to control the operation of VS1053b and defaults to 0x4800 (SM_SDINEW set).
  #define SM_DIFF             0    // Differential [0 normal in-phase audio; 1 left channel inverted]
  #define SM_LAYER12          1    // Allow MPEG layers I & II [0 no; 1 yes]
  #define SM_RESET            2    // Soft reset [0 no reset; 1 reset] 
  #define SM_CANCEL           3    // Cancel decoding current file [0 no; 1 yes]
  #define SM_EARSPEAKER_LO    4    // EarSpeaker low setting [0 off; 1 active]
  #define SM_TESTS            5    // Allow SDI tests [0 allow; 1 not allow]
  #define SM_STREAM           6    // Stream mode [0 no; 1 yes]
  #define SM_EARSPEAKER_HI    7    // EarSpeaker high setting [0 off; 1 active]
  #define SM_DACT             8    // DCLK active edge [0 rising; 1 falling]
  #define SM_SDIORD           9    // SDI bit order [0 MSb first; 1 MSb last]
  #define SM_SDISHARE         10   // Share SPI chip select [0 no; 1 yes]
  #define SM_SDINEW           11   // VS10xx native SPI modes [0 no; 1 yes]
  #define SM_ADPCM            12   // PCM/ADPCM recording active [0 no; 1 yes]
  #define SM_LINE1            14   // MIC / LINE1 selector [0 MICP; 1 LINE1]
  #define SM_CLK_RANGE        15   // Input clock range [0 12..13MHz; 1 24..26MHz]

  // reg SCI_STATUS
  // SCI_STATUS contains information on the current status of VS1053b. It also controls some
  // low-level things that the user does not usually have to care about
  #define SS_DO_NOT_JUMP      15  // Header in decode, do not fast forward/rewind
//  #define SS_SWING        14:12  // Set swing to +0 dB, +0.5 dB, .., or +3.5 dB
  #define SS_VCM_OVERLOAD     11  // GBUF overload indicator ’1’ = overload
  #define SS_VCM_DISABLE      10  // GBUF overload detection ’1’ = disable
//  #define SS_VER            7:4  // Version
  #define SS_APDOWN2          3  //
  #define SS_APDOWN1          2  //
  #define SS_AD_CLOCK         1  //
  #define SS_REFERENCE_SEL    0  //
    
  // READ / WRITE 
  #define VS1053_READ         0x3 
  #define VS1053_WRITE        0x2
  
  // FUNCTION macros
  // bit is set?
  #define IS_BIT_SET(port, bit)             ( ((port) & (1 << (bit))) ? 1 : 0 )
  // bit is clear?
  #define IS_BIT_CLR(port, bit)             ( IS_BIT_SET(port, bit) ? 0 : 1 )
  // wait until bit is set
  #define WAIT_IF_BIT_IS_SET(port, bit)     { while (IS_BIT_CLR(port, bit)); }

  /**
   * @desc    DREQ Wait
   *
   * @param   void
   *
   * @return  uint8_t
   */
  //static inline uint8_t VS1053_DreqWait (void);

  /**
   * @desc    Hard reset
   * @source  https://www.vlsi.fi/player_vs1011_1002_1003/modularplayer/vs10xx_8c.html#a3
   *
   * @param   void
   *
   * @return  void
   */
  void VS1053_Reset (void);

  /**
   * @desc    Soft reset
   * @source  https://www.vlsi.fi/player_vs1011_1002_1003/modularplayer/vs10xx_8c.html#a2
   *
   * @param   void
   *
   * @return  void
   */
  void VS1053_SoftReset (void);

  /**
   * @desc    Test SDI - sine test
   *
   * @param   void
   *
   * @return  void
   */  
  void VS1053_SineTest (void);

  /**
   * @desc    Test SCI
   *
   * @param   void
   *
   * @return  void
   */  
  void VS1053_TestSci (void);

  /**
   * @desc    Write Serial Command Instruction
   *
   * @param   uint8_t addr
   * @param   uint16_t command
   *
   * @return  void
   */
  void VS1053_WriteSci (uint8_t, uint16_t);
  
  /**
   * @desc    Read Serial Command Instruction
   *
   * @param   uint8_t addr
   *
   * @return  uint16_t
   */
  uint16_t VS1053_ReadSci (uint8_t);
  
  /**
   * @desc    Write Serial Data
   *
   * @param   uint8_t * data
   * @param   uint8_t bytes
   *
   * @return  int
   */  
  int VS1053_WriteSdi (const uint8_t *, uint8_t);
  
  /**
   * @desc    Init
   *
   * @param   void
   *
   * @return  void
   */
  void VS1053_Init (void);

  /**
   * @desc    Set volume
   *
   * @param   uint8_t
   * @param   uint8_t
   *
   * @return  void
   */
  void VS1053_SetVolume (uint8_t, uint8_t);

#endif
