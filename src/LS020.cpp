/**
 * @file LS020.cpp
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
 *      - on-chip display data RAM (estimated 50kb)
 *      - pinout:
 *                              _   _   _   _   _   _   _ 
 *          1 LED-              |  |                   |  |
 *          2 LED+ (9..12v)     |  |    [backside]     |  |
 *          3 NC                |  |___________________|  | 
 *          4 GND               |   ___________________   |
 *          5 +2.9v (3.3v nice) |  |                   |  |
 *                        work  |  |    LS020B8UD05    |  |
 *          6 MOSI              |  |   04G002601A  Q   |  |
 *          7 SCL               |  |___________________|  |
 *          8 CS                |     _ _ _ _ _ _ _ _ _ _ |
 *          9 RST               |     | | | | | | | | | | |
 *          10 RS               |____10_9_8_7_6_5_4_3_2_1_|
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

#include "LS020.h"
#include <SPI.h>
#include <Wire.h> // just for adafruit gfx lib [damn it], don't pay attention

#if defined(ARDUINO) && ARDUINO >= 100  
    // faster than standard digitalWrite() 
    #define SETPIN(pin)         *(portOutputRegister(digitalPinToPort(pin))) |= digitalPinToBitMask(pin)
    #define CLRPIN(pin)         *(portOutputRegister(digitalPinToPort(pin))) &= ~digitalPinToBitMask(pin)
    #define DC(pin, type)       (type) ? SETPIN(pin):CLRPIN(pin);
#else // for other
    #define SETPIN(pin)         digitalWrite(pin, HIGH)
    #define CLRPIN(pin)         digitalWrite(pin, LOW)
    #define DC(pin, type)       digitalWrite(pin, type)
#endif
#define SETRST(pin)             SETPIN(pin)
#define CLRRST(pin)             CLRPIN(pin)
#define SETRS(pin)              SETPIN(pin)
#define CLRRS(pin)              CLRPIN(pin)
#define SETMOSI(pin)            SETPIN(pin) 
#define CLRMOSI(pin)            CLRPIN(pin)
#define SETCS(pin)              SETPIN(pin) 
#define CLRCS(pin)              CLRPIN(pin)
#define SETCLK(pin)             SETPIN(pin) 
#define CLRCLK(pin)             CLRPIN(pin)  
#define TGLCLK(pin)             SETCLK(pin);\
                                CLRCLK(pin)

LS020::LS020 (uint8_t rs, uint8_t rst, uint8_t cs, int8_t mosi, int8_t scl) 
             : Adafruit_GFX(_width = LS020_WIDTH, _height = LS020_HEIGHT) {

    _mosi = mosi;
    _scl = scl;
    _cs = cs;
    _rst = rst;
    _rs = rs;
    _orientation = 0;
}

void LS020::begin() {

    pinMode(_rs, OUTPUT);
    if (_mosi > 0) {

        pinMode(_mosi, OUTPUT);
        pinMode(_scl, OUTPUT);
        CLRCLK(_scl);
    } else {

        SPI.begin();
        SPI.setDataMode(SPI_MODE0);
        SPI.setBitOrder(MSBFIRST);
        SPI.setClockDivider(SPI_CLOCK_DIV2);
    }
    pinMode(_cs, OUTPUT);
    pinMode(_rst, OUTPUT);
    SETCS(_cs);
    CLRRST(_rst);                 // Bring reset low
    delay(100);                   // Wait 100
    SETRST(_rst);                 // Bring out of reset
    delay(100);                   // Wait 100ms

    const uint8_t init_array_0[] = {
        0xEF, 0x00, 0xEE, 0x04, 0x1B, 0x04, 0xFE, 0xFE,
        0xFE, 0xFE, 0xEF, 0x90, 0x4A, 0x04, 0x7F, 0x3F,
        0xEE, 0x04, 0x43, 0x06
    };
 
    const uint8_t init_array_1[] = {
        0xEF, 0x90, 0x09, 0x83, 0x08, 0x00, 0x0B, 0xAF,
        0x0A, 0x00, 0x05, 0x00, 0x06, 0x00, 0x07, 0x00,
        0xEF, 0x00, 0xEE, 0x0C, 0xEF, 0x90, 0x00, 0x80,
        0xEF, 0xB0, 0x49, 0x02, 0xEF, 0x00, 0x7F, 0x01,
        0xE1, 0x81, 0xE2, 0x02, 0xE2, 0x76, 0xE1, 0x83,
        0x80, 0x01, 0xEF, 0x90, 0x00, 0x00
    };
    for (uint8_t i = 0; i < sizeof(init_array_0); i++ ) {
        write8(LS020_CMD, init_array_0[i]);
    }
    delay(7);
    for (uint8_t i = 0; i < sizeof(init_array_1); i++ ) {
        write8(LS020_CMD, init_array_1[i]);
    }
    setRotation(_orientation);
}

inline void LS020::spiwrite(uint8_t byte) {

    CLRCS(_cs);
    if (_mosi > 0) { // software spi detected
        
        for (uint8_t bit = 0; bit < 8; bit++) {

            (byte & 0x80) ? SETMOSI(_mosi) : CLRMOSI(_mosi);
            byte<<=1;
            TGLCLK(_scl);
        }
    } else {

        SPI.transfer(byte);
    }
    SETCS(_cs);
}

inline void LS020::write8(uint8_t type, uint8_t byte) {

    (type) ? SETRS(_rs) : CLRRS(_rs);  // next byte is data or cmd
    spiwrite(byte);
}

inline void LS020::write_reg(uint8_t reg, uint8_t val) {

    SETRS(_rs);
    spiwrite(reg);
    spiwrite(val);
}

inline void LS020::write16(uint8_t type, uint16_t dbyte) {

    (type) ? SETRS(_rs) : CLRRS(_rs);
    spiwrite(dbyte>>8);
    spiwrite(dbyte);
}

void LS020::setRotation(uint8_t rot) {

    uint8_t val01, val05;
    Adafruit_GFX::setRotation(rot);
    switch(rot) {

        case 1:
            _orientation = 1;
            _width  = LS020_HEIGHT;
            _height = LS020_WIDTH;
            val01 = 0x00; 
            val05 = 0x00;
        break;
        case 2:
            _orientation = 2;
            _width  = LS020_WIDTH;
            _height = LS020_HEIGHT;
            val01 = 0x80;               //x0->x1, y1->y0
            val05 = 0x04;               //0x04=rotate, 0x00=normal
        break;
        case 3:
            _orientation = 3;
            _width  = LS020_HEIGHT;
            _height = LS020_WIDTH;
            val01 = 0xC0;               //x1->x0, y1->y0
            val05 = 0x00;               //0x04=rotate, 0x00=normal
        break;
        default:
            _orientation = 0;
            _width  = LS020_WIDTH;
            _height = LS020_HEIGHT;
            val01 = 0x40;               //x1->x0, y0->y1
            val05 = 0x04;               //0x04=rotate, 0x00=normal
        break;
    }

    write_reg(0xEF, 0x90);
    write_reg(0x01, val01);
    write_reg(0x05, val05);

    //setAddrWindow(0, 0, _width-1, _height-1);
}

void LS020::setAddrWindow(uint8_t x0, uint8_t y0, uint8_t x1, uint8_t y1) {

    write_reg(0xEF, 0x90);

    switch(_orientation) {

        case 0:
            write_reg(0x08, y0);                        //set y0
            write_reg(0x09, y1);                        //set y1
            write_reg(0x0A, (LS020_WIDTH-1) - x0);      //set x0
            write_reg(0x0B, (LS020_WIDTH-1) - x1);      //set x1
            write_reg(0x06, y0);                        //set y cursor pos
            write_reg(0x07, (LS020_WIDTH-1) - x0);      //set x cursor pos
        break;
        case 1:
            write_reg(0x08, x0);                        //set y0
            write_reg(0x09, x1);                        //set y1
            write_reg(0x0A, y0);                        //set x0
            write_reg(0x0B, y1);                        //set x1
            write_reg(0x06, x0);                        //set y cursor pos
            write_reg(0x07, y0);                        //set x cursor pos
        break;
        case 2:
             write_reg(0x08, (LS020_HEIGHT-1) - y0);    //set y0
            write_reg(0x09, (LS020_HEIGHT-1) - y1);     //set y1
            write_reg(0x0A, x0);                        //set x0
            write_reg(0x0B, x1);                        //set x1
            write_reg(0x06, (LS020_HEIGHT-1) - y0);     //set y cursor pos
            write_reg(0x07, x0);                        //set x cursor pos
        break;
        case 3:
            write_reg(0x08, (LS020_HEIGHT-1) - x0);     // set x0
            write_reg(0x09, (LS020_HEIGHT-1) - x1);     //set x1
            write_reg(0x0A, (LS020_WIDTH-1) - y0);      //set y0
            write_reg(0x0B, (LS020_WIDTH-1) - y1);      //set y1
            write_reg(0x06, (LS020_HEIGHT-1) - x0);     //set x cursor pos
            write_reg(0x07, (LS020_WIDTH-1) - y0);      //set y cursor pos
        break;
    }
}

void LS020::invertDisplay(const bool invert) {

    write_reg(0xEF, 0xB0);
    write_reg(0x49, (invert) ? 0x03:0x02);
    write_reg(0xEF, 0x90);
}

void LS020::drawPixel (int16_t x, int16_t y, 
                       uint16_t color) {

    if ((x >= 0 && x < _width) 
    && (y >= 0 && y < _height)) {

        setAddrWindow(x, y, x, y);
        write16(LS020_DATA, color);
    }
}

void LS020::fillScreen(uint16_t color) {

    setAddrWindow(0, 0, _width-1, _height-1);
    for(uint32_t size = LS020_WIDTH*LS020_HEIGHT; size!=0; size--) {

        write16(LS020_DATA, color);
    }
}

void LS020::drawRect(int16_t x, int16_t y, 
                      int16_t w, int16_t h, 
                      uint16_t color) {
    
    setAddrWindow(x,y,x+w-1,y);
    for (int16_t size = w; size!=0; size--) {

        write16(LS020_DATA, color);
    }

    setAddrWindow(x,y+h-1,x+w-1,y+h-1);
    for (int16_t size = w; size!=0; size--) {

        write16(LS020_DATA, color);
    }

    setAddrWindow(x,y,x,y+h-1);
    for (int16_t size = h; size!=0; size--) {

        write16(LS020_DATA, color);
    }

    setAddrWindow(x+w-1,y,x+w-1,y+h-1);
    for (int16_t size = h; size!=0; size--) {

        write16(LS020_DATA, color);
    }
}

void LS020::fillRect(int16_t x, int16_t y, 
                     int16_t w, int16_t h, 
                     uint16_t color) {
    
    for (uint8_t row = 0; row < h; row++) {

        setAddrWindow(x, y+row, x+w-1, y+row);

        for (int8_t size = w; size!=0; size--) {

            write16(LS020_DATA, color);
        }
    }
}