/*********************************************************************
This is an example for our Monochrome OLEDs based on SSD1306 drivers

  Pick one up today in the adafruit shop!
  ------> http://www.adafruit.com/category/63_98

This example is for a 128x64 size display using SPI to communicate
4 or 5 pins are required to interface

Adafruit invests time and resources providing this open source code, 
please support Adafruit and open-source hardware by purchasing 
products from Adafruit!

Written by Limor Fried/Ladyada  for Adafruit Industries.  
BSD license, check license.txt for more information
All text above, and the splash screen must be included in any redistribution
*********************************************************************/

#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define OLED_DC 12	// 11
#define OLED_CS 7	// 12
#define OLED_CLK 13	// 10
#define OLED_MOSI 11	// 9
#define OLED_RESET 6	// 13
Adafruit_SSD1306 display(OLED_MOSI, OLED_CLK, OLED_DC, OLED_RESET, OLED_CS);

#if (SSD1306_LCDHEIGHT != 64)
#error("Height incorrect, please fix Adafruit_SSD1306.h!");
#endif

// text display tests
// Size 1 = 6 wide x 8 tall.  include 1 pixel to left and below character
//          21 Characters wide x 8 tall
// Size 2 = 12 wide x 16 tall.
//          10 Characters wide x 4 tall
#define HEIGHT 32
#define WIDTH 128
#define SIZE1HEIGHT 8
#define SIZE1WIDTH 6

void setup()   {                
  Serial.begin(9600);
  
  // by default, we'll generate the high voltage from the 3.3v line internally! (neat!)
  display.begin(SSD1306_SWITCHCAPVCC);
  // init done
  
  display.display(); // show splashscreen
  //delay(500);
  display.clearDisplay();   // clears the screen and buffer
  
  //ShowCurrentWithScroll();
  //ShowPreviousCurrentNext();
}

void ShowCurrentWithScroll()
{
  int i, currentIndex;
  
  //TODO Dynamically create this...
  int dualVCOShapeLength = 3;
  char* dualVCOShape[]={"ideal", "analog1", "analog2"};
  int dualVCOShapeLengths[]={5, 7, 7}; //minus the null terminators
  
  
  int textSize = 2;
  display.setTextColor(WHITE);
  //TopCenter: display.setCursor((WIDTH - previousLength * SIZE1WIDTH * textSize)/2 , 0);
  //Middle Center: display.setCursor((WIDTH - currentLength * SIZE1WIDTH * textSize)/2 , (HEIGHT - SIZE1HEIGHT * textSize) / 2);
  //Bottom Center: display.setCursor((WIDTH - nextLength * SIZE1WIDTH * textSize)/2 , HEIGHT - SIZE1HEIGHT * textSize);
  
  currentIndex = 0;
  for (i = 0; i < dualVCOShapeLength; i++)
  {
    if (currentIndex > i)
    {
      //Scroll up
      ShowTwoLinesCentered(textSize, dualVCOShape[i], dualVCOShapeLengths[i], dualVCOShape[i + 1], dualVCOShapeLengths[i + 1]);
      delay(500);
    }
    else if (currentIndex < i)
    {
      //Scroll down
      ShowTwoLinesCentered(textSize, dualVCOShape[i - 1], dualVCOShapeLengths[i - 1], dualVCOShape[i], dualVCOShapeLengths[i]);
      delay(500);
    }
    ShowOneLineCentered(textSize, dualVCOShape[i], dualVCOShapeLengths[i]);
    delay(2000);
    currentIndex = i;
  }
  
  for (i = dualVCOShapeLength - 1; i >= 0; i--)
  {
    if (currentIndex > i)
    {
      //Scroll up
      ShowTwoLinesCentered(textSize, dualVCOShape[i], dualVCOShapeLengths[i], dualVCOShape[i + 1], dualVCOShapeLengths[i + 1]);
      delay(500);
    }
    else if (currentIndex < i)
    {
      //Scroll down
      ShowTwoLinesCentered(textSize, dualVCOShape[i - 1], dualVCOShapeLengths[i - 1], dualVCOShape[i], dualVCOShapeLengths[i]);
      delay(500);
    }
    ShowOneLineCentered(textSize, dualVCOShape[i], dualVCOShapeLengths[i]);
    delay(2000);
    currentIndex = i;
  }
  
  /*
  // Scroll down...
  i = 1;
  ShowTwoLinesCentered(textSize, dualVCOShape[i - 1], dualVCOShapeLengths[i - 1], dualVCOShape[i], dualVCOShapeLengths[i]);
  delay(500);
  ShowOneLineCentered(textSize, dualVCOShape[i], dualVCOShapeLengths[i]);
  delay(2000);
  
  // Scroll down...
  i = 2;
  ShowTwoLinesCentered(textSize, dualVCOShape[i - 1], dualVCOShapeLengths[i - 1], dualVCOShape[i], dualVCOShapeLengths[i]);
  delay(500);
  ShowOneLineCentered(textSize, dualVCOShape[i], dualVCOShapeLengths[i]);
  delay(2000);
  
  // Scroll up...
  i = 1;
  ShowTwoLinesCentered(textSize, dualVCOShape[i], dualVCOShapeLengths[i], dualVCOShape[i + 1], dualVCOShapeLengths[i + 1]);
  delay(500);
  ShowOneLineCentered(textSize, dualVCOShape[i], dualVCOShapeLengths[i]);
  delay(2000);
  
  // Scroll up...
  i = 0;
  ShowTwoLinesCentered(textSize, dualVCOShape[i], dualVCOShapeLengths[i], dualVCOShape[i + 1], dualVCOShapeLengths[i + 1]);
  delay(500);
  ShowOneLineCentered(textSize, dualVCOShape[i], dualVCOShapeLengths[i]);
  delay(2000);
  */
  // text display tests
  // Size 1 = 6 wide x 8 tall.  include 1 pixel to left and below character
  //          21 Characters wide x 8 tall
  // Size 2 = 12 wide x 16 tall.
  //          10 Characters wide x 4 tall
  
}

void ShowTwoLinesCentered(int textSize, char text1[], int text1Length, char text2[], int text2Length)
{
  display.clearDisplay();
  display.setTextSize(textSize);
  display.setCursor((WIDTH - text1Length * SIZE1WIDTH * textSize)/2 , 0);
  display.println(text1);

  display.setTextSize(textSize);
  display.setCursor((WIDTH - text2Length * SIZE1WIDTH * textSize)/2 , HEIGHT - SIZE1HEIGHT * textSize);
  display.println(text2);
 
  display.display();
 
}

void ShowOneLineCentered(int textSize, char text[], int length)
{
  //Serial.println(text);
  // Show text
  display.clearDisplay();
  display.setTextSize(textSize);
  display.setCursor((WIDTH - length * SIZE1WIDTH * textSize)/2 , (HEIGHT - SIZE1HEIGHT * textSize) / 2);
  display.println(text);
  
  display.display();
}

void ShowPreviousCurrentNext()
{
  char previousText[] = "Previous";
  char currentText[] = "Current";
  char nextText[] = "Next";
  
  int previousLength = (sizeof(previousText) - 1)/sizeof(char);  // TODO: make macro
  int currentLength = (sizeof(currentText) - 1)/sizeof(char);
  int nextLength = (sizeof(nextText) - 1)/sizeof(char);
  int i;
  int textSize = 1;
  
  display.setTextColor(WHITE);
  
  display.setTextSize(textSize);
  display.setCursor((WIDTH - previousLength * SIZE1WIDTH * textSize)/2 , 0);
  display.println(previousText);
  
  textSize = 2;
  display.setTextSize(textSize);
  display.setCursor((WIDTH - currentLength * SIZE1WIDTH * textSize)/2 , (HEIGHT - SIZE1HEIGHT * textSize) / 2);
  //display.setTextColor(BLACK, WHITE); // 'inverted' text
  display.println(currentText);
  //display.setTextColor(WHITE);
  
  textSize = 1;
  display.setTextSize(textSize);
  display.setCursor((WIDTH - nextLength * SIZE1WIDTH * textSize)/2 , HEIGHT - SIZE1HEIGHT * textSize);
  display.println(nextText);
  display.display();
  delay(2000);
}


void loop() {
  ShowCurrentWithScroll();
  
}
