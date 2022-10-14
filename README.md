# LS020 LCD LIBRARY

This is an arduino library for TFT LCD LS020 from Siemens 65 series. This library provides just hardware application layer functions. Most graphic features are provided by Adafruit GFX library.

## Overview

 LS020 supports only SPI to communicate. 
 SPI requires MOSI, SCL, CS. In addition LS020 requires RESET (for display circut dropping)
 and RS (to select data or cmd) pins.

#### LS020:

        - resolution: 176x132
        - on-chip display data RAM (46 kilobytes)
        - pinout:
                                  _   _   _   _   _   _   _ 
            1 LED-                |  |                   |  |
            2 LED+ (9..12v)       |  |    [backside]     |  |
            3 NC                  |  |___________________|  | 
            4 GND                 |   ___________________   |
            5 +2.9v (3.3v nice    |  |                   |  |
                         works)   |  |    LS020B8UD05    |  |
            6 MOSI                |  |   04G002601A  Q   |  |
            7 SCL                 |  |___________________|  |
            8 CS                  |     _ _ _ _ _ _ _ _ _ _ |
            9 RST                 |     | | | | | | | | | | |
            10 RS                 |____10_9_8_7_6_5_4_3_2_1_|

## How to use it

LS020 library depends by Adafruit GFX library and you must install Adafruit GFX for your IDE.

If you use Arduino IDE just add this, Adafruit GFX and Adafruit BusIO libraries via "Sketch -> Include Library -> Add .ZIP Library" in Arduino IDE.\
If you use Platformio IDE just add the following text in your platformio.ini:

    lib_deps=
        https://github.com/kashapovd/LS020-SIEMENS-TFTLCD-Library
        https://github.com/adafruit/Adafruit-GFX-Library
        https://github.com/adafruit/Adafruit_BusIO

## Demonstration ([video](https://youtu.be/rdE-_EuR0MM))
![PCF8814 LCD TEST](demo/demo.gif)

Kashapov Yaroslav, 2020\
feedback: [telegram](https://t.me/kashapovd), <iam@kashapovd.xyz>
