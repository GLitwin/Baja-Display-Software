#include <SPI.h>

#include <Adafruit_RA8875.h>
#include <Adafruit_GFX.h>
#include <gfxfont.h>


//TESTING VALUES
//***************

char text[] = {"Baja Display Team!"};
//Gauge Values - different ones should be set for each gauge
float maxVal = 100; //Max value to be displayed by the gauge
float minVal = 0; //Min value to be displayed by the gauge
float value = 70; //Used for testing the output of the gauge bar
float range = maxVal - minVal; //range of values that the gauge will be representing
float xFill = 0; //Amount, in pixels, to fill the gauge bar

int gaugeHeight = 100;
int gaugeWidth = 600;
int gaugeLocation[] = {100, 50}; ///x,y

/*
 * Notes:
 * - All screen locations are referenced from the top left corner, top left corner is (0,0); 
 */
//***************


// Library only supports hardware SPI at this time
// Connect SCLK to UNO Digital #13 (Hardware SPI clock)
// Connect MISO to UNO Digital #12 (Hardware SPI MISO)
// Connect MOSI to UNO Digital #11 (Hardware SPI MOSI)

#define RA8875_CS 10
#define RA8875_RESET 9

Adafruit_RA8875 tft = Adafruit_RA8875(RA8875_CS, RA8875_RESET);

void displayText(const char* text, uint16_t textScale, uint16_t foregroundColor, uint16_t backgroundColor, uint16_t xLocation, uint16_t yLocation);
void displayGauge(uint16_t value, uint16_t width, uint16_t height, uint16_t xLocation, uint16_t yLocation, uint16_t minVal, uint16_t maxVal);






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
  tft.PWM1config(true, RA8875_PWM_CLK_DIV2048); // PWM output for backlight
  tft.PWM1out(255);

  //TESTING CODE
  //******************
  tft.fillScreen(RA8875_WHITE);
  displayText(text, 2, RA8875_BLACK, RA8875_WHITE, 100, 300);
  //displayGauge(value, 600, 100, 100, 50, minVal, maxVal);
  //tft.drawRect(100, 50, 600, 100, 0x0000); // 0x0000 is Black
  //tft.fillRect(101, 51, 418, 98, RA8875_RED);
  //******************

}

void loop() {
  //TESTING CODE
  //******************
  //Formula for this ^ would normally be ((value-LastValue)/range)*gaugeWidth but it only goes down by 1 in this test so there is no need for value-Lastvalue
  //This is used to determine how many pixels to erase when the bar decreases by one value because the pizel width of the bar will not be that same as the range of value to be displayed by the gauge
    for (int i = maxVal; i > minVal; i--)
  {
    tft.fillRect(gaugeLocation[0]+xFill-(gaugeWidth/range), gaugeLocation[1]+1,(gaugeWidth/range), gaugeHeight - 2, RA8875_WHITE); //This is used when the gauge decreases to erase the extra - otherwise the bar would stay full
    displayGauge(i, gaugeWidth, gaugeHeight, gaugeLocation[0], gaugeLocation[1], minVal, maxVal);
    delay(30);
  }

  for (int i = minVal; i < maxVal; i++)
  {
    displayGauge(i, gaugeWidth, gaugeHeight, gaugeLocation[0], gaugeLocation[1], minVal, maxVal);
    delay(30);
  }
  //******************

  
}

void displayText(const char* text, uint16_t textScale, uint16_t foregroundColor, uint16_t backgroundColor, uint16_t xLocation, uint16_t yLocation)
{
  tft.textMode();
  tft.textSetCursor(xLocation, yLocation);
  tft.textColor(foregroundColor, backgroundColor);
  tft.textEnlarge(textScale);
  tft.textWrite(text);
}


void displayGauge(uint16_t value, uint16_t width, uint16_t height, uint16_t xLocation, uint16_t yLocation, uint16_t minVal, uint16_t maxVal)
{

  tft.graphicsMode();

  //Floats are used here because decimals are needed in multiplication and devision for xFill
  range = maxVal - minVal;
  float fillColour = 0x0000;
  xFill = 0;


  if (value < (range / 3.0))
  {
    fillColour = RA8875_GREEN; //Green
  }
  else if (value < ((2.0 * range) / 3.0))
  {
    fillColour = RA8875_YELLOW; //Yellow
  }
  else
  {
    fillColour = RA8875_RED; //Red
  }

  xFill = (((value - minVal) / range) * width);

  tft.drawRect(xLocation, yLocation, width, height, 0x0000);  // 0x0000 is Black
  tft.fillRect((xLocation + 1), (yLocation + 1), (xFill - 1), (height - 2), fillColour);
}




