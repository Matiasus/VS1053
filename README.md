# Library for VS1053

## VS1053 Description
Detailed information are described in [Datasheet VS1053](https://www.vlsi.fi/fileadmin/datasheets/vs1053.pdf), [Application Notes VS10XX](https://www.vlsi.fi/fileadmin/app_notes/vs10XXan.pdf)

## Library
C library is aimed for driving [Datasheet VS1053](https://www.vlsi.fi/fileadmin/datasheets/vs1053.pdf) using 7 pin SPI Bus.

## Hardware connection
| PIN VS1053 | PIN ATMEGA328P | Description |
| :---: | :---: |  :---: |
| 5V | 5V | Supply Voltage |
| AGND | GND | Ground |
| MOSI | | Master OUT slave IN |
| MISO | | Master IN slave OUT |
| SCLK | | Clock for serial bus |
| XCS | | Chip select input |
| DREQ | | Data request, input bus |
| XRST | | Reset |
| XDCS | | Data chip select |

### Usage
Prior defined for MCU Atmega328p / Atmega8 / Atmega16. Need to be carefull with pins definition.

### Tested
Library was tested and proved on a **_VS1053 Breakboard_** with **_Atmega328p_**.
  
## Demonstration

## Links
- [Datasheet VS1053](https://www.vlsi.fi/fileadmin/datasheets/vs1053.pdf)
- [Application Notes VS10XX](https://www.vlsi.fi/fileadmin/app_notes/vs10XXan.pdf)

## Acknowledgement
