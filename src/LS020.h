/**
 * @file LS020.h
 *
 * @mainpage Arduino library for LS020 TFT LCD.
 *
 * @section Introduction
 *
 *  LS020 supports only SPI to communicate. 
 *  SPI requires MOSI, SCL, CS. In addition LS020 requires RESET (for display circut dropping)
 *  and RS (to select data or cmd) pins.
 *
 *  LS020:
 *      - resolution: 176x132
 *      - on-chip display data RAM (46 kilobytes)
 *      - pinout:
 *                                _   _   _   _   _   _   _ 
 *          1 LED-                |  |                   |  |
 *          2 LED+ (9..12v)       |  |    [backside]     |  |
 *          3 NC                  |  |___________________|  | 
 *          4 GND                 |   ___________________   |
 *          5 +2.9v (3.3v nice    |  |                   |  |
 *                       works)   |  |    LS020B8UD05    |  |
 *          6 MOSI                |  |   04G002601A  Q   |  |
 *          7 SCL                 |  |___________________|  |
 *          8 CS                  |     _ _ _ _ _ _ _ _ _ _ |
 *          9 RST                 |     | | | | | | | | | | |
 *          10 RS                 |____10_9_8_7_6_5_4_3_2_1_|
 * 
 * @section Dependencies
 *
 *  ADAFRUIT GRAPHIC LIB
 *
 * @section Author
 *
 *  Written by Yaroslav @kashapovd Kashapov just for fun, may 2020
 *
 * @section License
 *
 *  GNU GENERAL PUBLIC LICENSE ver. 3
 * 
 */

#ifndef LS020_H
#define LS020_H

#include <Adafruit_GFX.h>
#if defined(ARDUINO) && ARDUINO >= 100
    #include "Arduino.h"
#else
    #include "WProgram.h"
#endif

#define LS020_WIDTH 176
#define LS020_HEIGHT 132

#define LS020_DATA LOW // rs=low is data to write to ram
#define LS020_CMD HIGH  // rs=high is command

// some color palette
#define BLACK 0x0000       ///<   0,   0,   0
#define NAVY 0x000F        ///<   0,   0, 123
#define DARKGREEN 0x03E0   ///<   0, 125,   0
#define DARKCYAN 0x03EF    ///<   0, 125, 123
#define MAROON 0x7800      ///< 123,   0,   0
#define PURPLE 0x780F      ///< 123,   0, 123
#define OLIVE 0x7BE0       ///< 123, 125,   0
#define LIGHTGREY 0xC618   ///< 198, 195, 198
#define DARKGREY 0x7BEF    ///< 123, 125, 123
#define BLUE 0x001F        ///<   0,   0, 255
#define GREEN 0x07E0       ///<   0, 255,   0
#define CYAN 0x07FF        ///<   0, 255, 255
#define RED 0xF800         ///< 255,   0,   0
#define MAGENTA 0xF81F     ///< 255,   0, 255
#define YELLOW 0xFFE0      ///< 255, 255,   0
#define WHITE 0xFFFF       ///< 255, 255, 255
#define ORANGE 0xFD20      ///< 255, 165,   0
#define GREENYELLOW 0xAFE5 ///< 173, 255,  41
#define PINK 0xFC18        ///< 255, 130, 198

class LS020 : public Adafruit_GFX {
    void spiwrite(uint8_t byte);
    void write8(uint8_t type, uint8_t byte);
    void write_reg(uint8_t reg, uint8_t value);
    void write16(uint8_t type, uint16_t dbyte);
    int8_t _mosi, _scl, _rst, _cs;
    uint8_t _rs, _width, _height;
    uint8_t _orientation;

public:
    // software spi if u initialized mosi, sck
    LS020(uint8_t rs, uint8_t rst, uint8_t cs, int8_t mosi = -1, int8_t scl = -1);
    void begin(void);
    void invertDisplay(const bool invert);
    void setRotation(uint8_t rot);
    void setAddrWindow(uint8_t x0, uint8_t y0, uint8_t x1, uint8_t y1);
    void drawPixel(int16_t x, int16_t y, uint16_t color);
    void fillScreen(uint16_t color);

    uint16_t color565(uint8_t red, uint8_t green, uint8_t blue) {
        return ((red & 0xF8) << 8) | ((green & 0xFC) << 3) | (blue >> 3);
    }
    void drawRect(int16_t x, int16_t y, 
                    int16_t w, int16_t h, 
                    uint16_t color);

    void fillRect(int16_t x, int16_t y, 
                    int16_t w, int16_t h, 
                    uint16_t color);

    void drawSquare(int16_t x, int16_t y, 
                    int16_t a, uint16_t color) {
        drawRect(x, y, a, a, color); 
    }

    void fillSquare(int16_t x, int16_t y, 
                    int16_t a, uint16_t color) {
                    fillRect(x, y, a, a, color); 
    }
};

#endif