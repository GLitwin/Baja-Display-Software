#include <Adafruit_RA8875.h>

#include <Adafruit_GFX.h>
#include <gfxfont.h>

#define RA8875_CS 10 //Clear Screen Pin - NEEDS TO BE SET
#define RA8875_RESET 9  //Reset Pin - NEEDS TO BE SET

Adafruit_RA8875 tft = Adafruit_RA8875(RA8875_CS, RA8875_RESET);

void displayText(const char* text, uint16_t textScale, uint16_t foregroundColor, uint16_t backgroundColor, uint16_t xLocation, uint16_t yLocation);
void displayGauge(uint16_t value, uint16_t width, uint16_t height,uint16_t xLocation, uint16_t yLocation, uint16_t minVal, uint16_t maxVal);



void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  Serial.println("RA8875 start");

  tft.begin(RA8875_800x480);

  if (!tft.begin(RA8875_800x480)) {
    Serial.println("RA8875 Not Found!");
    while (1);
  }
  Serial.println("Found RA8875");

  tft.displayOn(true);
  tft.GPIOX(true);      // Enable TFT - display enable tied to GPIOX
  tft.PWM1config(true, RA8875_PWM_CLK_DIV1024); // PWM output for backlight
  tft.PWM1out(255);

  tft.fillScreen(RA8875_WHITE);


}

void loop() {
  // put your main code here, to run repeatedly:
  
}


void displayText(const char* text, uint16_t textScale, uint16_t foregroundColor, uint16_t backgroundColor, uint16_t xLocation, uint16_t yLocation)
{
  tft.textMode();
  tft.textSetCursor(xLocation, yLocation);
  tft.textColor(foregroundColor, backgroundColor);
  tft.textEnlarge(textScale);
  tft.textWrite(text);
}


void displayGauge(uint16_t value, uint16_t width, uint16_t height,uint16_t xLocation, uint16_t yLocation, uint16_t minVal, uint16_t maxVal)
{
  
  tft.graphicsMode();

  
  uint16_t range = maxVal - minVal;
  uint16_t fillColour = 0x0000;
  uint16_t xFill = 0;

  if (value < (range/3.0))
  {
    fillColour = 0x07E0; //Green
  }
  else if (value < ((2.0*range)/3.0))
  {
    fillColour = 0xFFE0; //Yellow
  }
  else
  {
    fillColour = 0xF800; //Red
  }
  

  xFill = ((value - minVal)/range)*(width-2);

  tft.drawRect(xLocation, yLocation, width, height, 0x0000);  // 0x0000 is Black
  tft.fillRect((xLocation+1), (yLocation+1), (xFill - 2),(height - 2),fillColour);
}




