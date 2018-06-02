//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
///////          8 Bit Uno TFT LCD Sheild STM32 BluePill Demo       //////////
//////////////////////////////////////////////////////////////////////////////
///////                     by WacKEDmaN 2018                       //////////
//////////////////////////////////////////////////////////////////////////////
///////              Pinout:      STM32         TFT LCD             //////////
///////                Data      PA0-PA7     LCD_D0-LCD_D7          //////////
///////                Reset       PB9          LCD_RST             //////////
///////                CS          PB8          LCD_CS              //////////
///////                CD          PB7          LCD_RS              //////////
///////                WR          PB6          LCD_WR              //////////
///////                RD          PB5          LCD_RD              //////////
//////////////////////////////////////////////////////////////////////////////
///////                          Demos:                             //////////
///////           Amstrad CPC bootscreen with custom font           //////////
///////                        Mandelbrot                           //////////
///////                      Raytraced Scene                        //////////
///////                                                             //////////
///////     48Mhz: Mandelbrot: 53 seconds Raytrace: 60 seconds      //////////
///////     72Mhz: Mandelbrot: 47 seconds Raytrace: 53 seconds      //////////
///////    128Mhz: Mandelbrot: 26 seconds Raytrace: 30 seconds      //////////
///////                                                             //////////
///////                Todo: Add SDCard, Touchscreen                //////////
/////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////

#include <Adafruit_GFX.h>    // Core graphics library
#include <MCUFRIEND_kbv.h>  // LCD Sheild library slightly modified utility\mcufriend_shield.h

#include <Fonts/cpc.h>  // requires cpc.h font placed in libraries\Adafruit_GFX_Library\Fonts folder

#include "mandelbrot.h"
#define RGBTO565(_r, _g, _b) ((((_r) & B11111000)<<8) | (((_g) & B11111100)<<3) | ((_b) >>3))
#include "raytracer.h"

#define LCD_CS PB8 
#define LCD_CD PB7 
#define LCD_WR PB6 
#define LCD_RD PB5 
#define LCD_RESET PB9 

#define BLACK   0x0000
#define BLUE    0x0010
#define RED     0xF800
#define GREEN   0x0400
#define CYAN    0x0410
#define MAGENTA 0x8010
#define YELLOW  0xFFE0
#define WHITE   0xFFFF

//MCUFRIEND_kbv tft;

////////// CPC Bootscreen /////////////////////////
void CPCboot() {
  tft.setFont(&cpc);
  tft.setTextColor(YELLOW,BLACK);
  tft.fillScreen(BLUE);
  tft.setCursor(28, 32); 
  tft.println("Amstrad 64K Microcomputer  (v1)");
  tft.setCursor(28, 44); 
  tft.println("c1984 Amstrad Consumer Elec. plc");
  tft.setCursor(108, 54); 
  tft.println("and Locomotive Soft Ltd.");
  tft.setCursor(28, 66); 
  tft.println("BASIC 1.0");
  tft.setCursor(20, 78);
  tft.println("Ready");
  delay(5000); // show screen for 5 sec before running
  tft.setCursor(20, 90);
  tft.print("r");
  delay(250);
  tft.print("u");
  delay(250);
  tft.print("n");
  delay(500);
}

////// CPC bootscreen end //////

///////////////// Setup ///////////////////////////////////
void setup() {
  Serial.begin(115200);
  Serial.println(F("Touch-Thing!"));
  
  tft.reset();

  uint16_t identifier = tft.readID();  
  Serial.print(F("tft.readID = 0x"));
  Serial.println(identifier, HEX);

  tft.begin(identifier);
  
  tft.setRotation(3);
  CPCboot();
}
/// end setup ////


////////////////// Loop //////////////////////////////////
void loop() {
  // mandelbrot
  tft.fillScreen(BLACK);
  unsigned long t = millis();
  unsigned long mandeltime = t;
  drawMandelbrot();
  Serial.println(F("Done!"));
  Serial.print(F("Mandelbrot Time = "));
  mandeltime = (millis() - t)/1000;
  Serial.print(mandeltime);
  Serial.println(F(" seconds"));
  delay(2000);  
  tft.fillScreen(BLACK);
  tft.setTextColor(YELLOW,BLACK);
  tft.setTextSize(2);
  tft.setCursor(32,32);
  tft.println("Mandelbrot Time");
  tft.setCursor(64,64);
  tft.print(mandeltime);
  tft.print(" Seconds");
  delay(5000);  
  // mandelbrot end

  // raytracer 
  tft.fillScreen(BLACK);
  t = millis();
  unsigned long raytime = t;
//    doRaytrace(1,320,240,4); // fast preview
//    doRaytrace(1,320,240,2); // slower preview
    doRaytrace(1,320,240,1); // full 320x240 1 sample
//  doRaytrace(1,160,120,2); // quarter of the screen
//  doRaytrace(8);  // very high quality 320x240 antialiased, 8 samples
  Serial.println(F("Done!"));
  Serial.print(F("Raytrace Time = "));
  raytime = (millis() - t)/1000;
  Serial.print(raytime);
  Serial.println(F(" seconds"));
  delay(2000);  
  tft.fillScreen(BLACK);
  tft.setTextColor(YELLOW,BLACK);
  tft.setTextSize(2);
  tft.setCursor(32,32);
  tft.println("RayTrace Time");
  tft.setCursor(64,64);
  tft.print(raytime);
  tft.print(" Seconds");
  delay(5000);  
  
}
/// (never)end loop ////

