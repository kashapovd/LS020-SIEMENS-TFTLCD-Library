/***************************************************
  This is example for LS020 LCD using SPI to 
  communicate.
 ****************************************************/

#include "Adafruit_GFX.h"
#include "LS020.h"

// hardware spi:  LS020 tft = LS020(rs_pin, rst_pin, cs_pin);
// LS020 tft = LS020(4,5,10);
// software spi(slowly): LS020 tft = LS020(rs_pin, rst_pin, cs_pin, mosi_pin, scl_pin);
// LS020 tft = LS020(4,5,6,10,11);

LS020 tft = LS020(15,2,0);

unsigned long testText();
unsigned long testFillScreen();
unsigned long testLines(uint16_t color);
unsigned long testLines(uint16_t color);
unsigned long testFastLines(uint16_t color1, uint16_t color2);
unsigned long testRects(uint16_t color);
unsigned long testFilledRects(uint16_t color1, uint16_t color2);
unsigned long testFilledCircles(uint8_t radius, uint16_t color);
unsigned long testCircles(uint8_t radius, uint16_t color);
unsigned long testTriangles();
unsigned long testFilledTriangles();
unsigned long testRoundRects();
unsigned long testFilledRoundRects() ;

void setup() {

  Serial.println("LS020 test!"); 
 
  tft.begin();
  //tft.invertDisplay(true);
  tft.setRotation(3);
  Serial.println(F("Benchmark                Time (microseconds)"));
  delay(10);
  Serial.print(F("Screen fill              "));
  Serial.println(testFillScreen());
  delay(500);

  Serial.print(F("Text                     "));
  Serial.println(testText());
  delay(3000);

  Serial.print(F("Lines                    "));
  Serial.println(testLines(CYAN));
  delay(500);

  Serial.print(F("Horiz/Vert Lines         "));
  Serial.println(testFastLines(RED, BLUE));
  delay(500);
    Serial.print(F("Rectangles (outline)     "));
  Serial.println(testRects(GREEN));
  delay(500);

  Serial.print(F("Rectangles (filled)      "));
  Serial.println(testFilledRects(YELLOW, MAGENTA));
  delay(500);
  Serial.print(F("Rectangles (outline)     "));
  Serial.println(testRects(GREEN));
  delay(500);

  Serial.print(F("Rectangles (filled)      "));
  Serial.println(testFilledRects(YELLOW, MAGENTA));
  delay(500);

  Serial.print(F("Circles (filled)         "));
  Serial.println(testFilledCircles(10, MAGENTA));

  Serial.print(F("Circles (outline)        "));
  Serial.println(testCircles(10, WHITE));
  delay(500);

  Serial.print(F("Triangles (outline)      "));
  Serial.println(testTriangles());
  delay(500);

  Serial.print(F("Triangles (filled)       "));
  Serial.println(testFilledTriangles());
  delay(500);

  Serial.print(F("Rounded rects (outline)  "));
  Serial.println(testRoundRects());
  delay(500);

  Serial.print(F("Rounded rects (filled)   "));
  Serial.println(testFilledRoundRects());
  delay(500);

  Serial.println(F("Done!"));
}


void loop(void) {

    for(uint8_t rotation=0; rotation<4; rotation++) {

        tft.setRotation(rotation);
        testText();
        delay(1000);
    }
}

unsigned long testFillScreen() {

    unsigned long start = micros();
    tft.fillScreen(BLACK);
    yield();
    tft.fillScreen(RED);
    yield();
    tft.fillScreen(GREEN);
    yield();
    tft.fillScreen(BLUE);
    yield();
    tft.fillScreen(BLACK);
    yield();
    return micros() - start;
}

unsigned long testText() {

    tft.fillScreen(BLACK);
    unsigned long start = micros();
    tft.setCursor(0, 0);
    tft.setTextColor(WHITE);  tft.setTextSize(1);
    tft.println("Hello World!");
    tft.setTextColor(YELLOW); tft.setTextSize(2);
    tft.println(1234.56);
    tft.setTextColor(RED);    tft.setTextSize(3);
    tft.println(0xDEADBEEF, HEX);
    tft.setTextColor(GREEN);
    tft.setTextSize(3);
    tft.println("Bonorum");
    tft.setTextSize(1);
    tft.println("Sed ut perspiciatis");
    tft.println("unde omnis iste natus");
    tft.println("error sit voluptatem accusantium ");
    tft.println("doloremque laudantium,"); 
    tft.println("totam rem aperiam,");
    tft.println("eaque ipsa quae ab"); 
    tft.println("illo inventore");
    tft.println("veritatis et quasi architecto beatae vitae dicta");
    return micros() - start;
}

unsigned long testLines(uint16_t color) {

    unsigned long start, t;
    int           x1, y1, x2, y2,
                  w = tft.width(),
                  h = tft.height();

    tft.fillScreen(BLACK);
    yield();
    
    x1 = y1 = 0;
    y2    = h - 1;
    start = micros();
    for(x2=0; x2<w; x2+=6) tft.drawLine(x1, y1, x2, y2, color);
    x2    = w - 1;
    for(y2=0; y2<h; y2+=6) tft.drawLine(x1, y1, x2, y2, color);
    t     = micros() - start; // fillScreen doesn't count against timing

    yield();
    tft.fillScreen(BLACK);
    yield();

    x1    = w - 1;
    y1    = 0;
    y2    = h - 1;
    start = micros();
    for(x2=0; x2<w; x2+=6) tft.drawLine(x1, y1, x2, y2, color);
    x2    = 0;
    for(y2=0; y2<h; y2+=6) tft.drawLine(x1, y1, x2, y2, color);
    t    += micros() - start;

    yield();
    tft.fillScreen(BLACK);
    yield();

    x1    = 0;
    y1    = h - 1;
    y2    = 0;
    start = micros();
    for(x2=0; x2<w; x2+=6) tft.drawLine(x1, y1, x2, y2, color);
    x2    = w - 1;
    for(y2=0; y2<h; y2+=6) tft.drawLine(x1, y1, x2, y2, color);
    t    += micros() - start;

    yield();
    tft.fillScreen(BLACK);
    yield();

    x1    = w - 1;
    y1    = h - 1;
    y2    = 0;
    start = micros();
    for(x2=0; x2<w; x2+=6) tft.drawLine(x1, y1, x2, y2, color);
    x2    = 0;
    for(y2=0; y2<h; y2+=6) tft.drawLine(x1, y1, x2, y2, color);

    yield();
    return micros() - start;
}

unsigned long testFastLines(uint16_t color1, uint16_t color2) {

    unsigned long start;
    int           x, y, w = tft.width(), h = tft.height();

    tft.fillScreen(BLACK);
    start = micros();
    for(y=0; y<h; y+=5) tft.drawFastHLine(0, y, w, color1);
    for(x=0; x<w; x+=5) tft.drawFastVLine(x, 0, h, color2);

    return micros() - start;
}

unsigned long testRects(uint16_t color) {

    unsigned long start;
    int           n, i, i2,
                  cx = tft.width()  / 2,
                  cy = tft.height() / 2;

    tft.fillScreen(BLACK);
    n     = min(tft.width(), tft.height());
    start = micros();
    for(i=2; i<n; i+=6) {

        i2 = i / 2;
        tft.drawRect(cx-i2, cy-i2, i, i, color);
    }

    return micros() - start;
}

unsigned long testFilledRects(uint16_t color1, uint16_t color2) {

    unsigned long start, t = 0;
    int           n, i, i2,
                  cx = tft.width()  / 2 - 1,
                  cy = tft.height() / 2 - 1;

    tft.fillScreen(BLACK);
    n = min(tft.width(), tft.height());
    for(i=n; i>0; i-=6) {

        i2    = i / 2;
        start = micros();
        tft.fillRect(cx-i2, cy-i2, i, i, color1);
        t    += micros() - start;
        // Outlines are not included in timing results
        tft.drawRect(cx-i2, cy-i2, i, i, color2);
        yield();
    }

    return t;
}

unsigned long testFilledCircles(uint8_t radius, uint16_t color) {

    unsigned long start;
    int x, y, w = tft.width(), h = tft.height(), r2 = radius * 2;

    tft.fillScreen(BLACK);
    start = micros();
    for(x=radius; x<w; x+=r2) {

      for(y=radius; y<h; y+=r2) {

        tft.fillCircle(x, y, radius, color);
      }
    }

    return micros() - start;
}

unsigned long testCircles(uint8_t radius, uint16_t color) {

    unsigned long start;
    int x, y, r2 = radius * 2,
        w = tft.width()  + radius,
        h = tft.height() + radius;

    // Screen is not cleared for this one -- this is
    // intentional and does not affect the reported time.
    start = micros();
    for(x=0; x<w; x+=r2) {

        for(y=0; y<h; y+=r2) {

            tft.drawCircle(x, y, radius, color);
        }
    }

    return micros() - start;
}

unsigned long testTriangles() {

    unsigned long start;
    int           n, i, cx = tft.width()  / 2 - 1,
                        cy = tft.height() / 2 - 1;

    tft.fillScreen(BLACK);
    n     = min(cx, cy);
    start = micros();

    for(i=0; i<n; i+=5) {

        tft.drawTriangle(
            cx    , cy - i, // peak
            cx - i, cy + i, // bottom left
            cx + i, cy + i, // bottom right
            tft.color565(i, i, i));
    }

    return micros() - start;
}

unsigned long testFilledTriangles() {

    unsigned long start, t = 0;
    int           i, cx = tft.width()  / 2 - 1,
                    cy = tft.height() / 2 - 1;

    tft.fillScreen(BLACK);
    start = micros();
    for(i=min(cx,cy); i>10; i-=5) {

        start = micros();
        tft.fillTriangle(cx, cy - i, cx - i, cy + i, cx + i, cy + i,
            tft.color565(0, i*10, i*10));
        t += micros() - start;
        tft.drawTriangle(cx, cy - i, cx - i, cy + i, cx + i, cy + i,
            tft.color565(i*10, i*10, 0));
        yield();
    }

    return t;
}

unsigned long testRoundRects() {

    unsigned long start;
    int           w, i, i2,
                  cx = tft.width()  / 2 - 1,
                  cy = tft.height() / 2 - 1;

    tft.fillScreen(BLACK);
    w     = min(tft.width(), tft.height());
    start = micros();
    for(i=0; i<w; i+=6) {

        i2 = i / 2;
        tft.drawRoundRect(cx-i2, cy-i2, i, i, i/8, tft.color565(i, 0, 0));
    }

    return micros() - start;
}

unsigned long testFilledRoundRects() {

    unsigned long start;
    int           i, i2,
                  cx = tft.width()  / 2 - 1,
                  cy = tft.height() / 2 - 1;

    tft.fillScreen(BLACK);
    start = micros();
    for(i=min(tft.width(), tft.height()); i>20; i-=6) {

        i2 = i / 2;
        tft.fillRoundRect(cx-i2, cy-i2, i, i, i/8, tft.color565(0, i, 0));
        yield();
    }

    return micros() - start;
}