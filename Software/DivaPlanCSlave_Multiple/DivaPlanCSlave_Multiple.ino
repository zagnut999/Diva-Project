#include <Wire.h>
#include <EasyTransferI2C.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define OLED_RESET 13   //10	// 13
#define OLED_DC  12 //12	// 11  //MISO
#define OLED_CLK  11 //13	// 10  // CLK
#define OLED_MOSI 10  //11	// 9   //MOSI
#define OLED_CS1 9  //7	// 12
#define OLED_CS2 8
#define OLED_CS3 7
#define OLED_CS4 6

Adafruit_SSD1306 display(OLED_MOSI, OLED_CLK, OLED_DC, OLED_RESET, OLED_CS1);
Adafruit_SSD1306 display2(OLED_MOSI, OLED_CLK, OLED_DC, OLED_RESET, OLED_CS2);
Adafruit_SSD1306 display3(OLED_MOSI, OLED_CLK, OLED_DC, OLED_RESET, OLED_CS3);
Adafruit_SSD1306 display4(OLED_MOSI, OLED_CLK, OLED_DC, OLED_RESET, OLED_CS4);

#if (SSD1306_LCDHEIGHT != 32)
#error("Height incorrect, please fix Adafruit_SSD1306.h!");
#endif

// text display
// Size 1 = 6 wide x 8 tall.  include 1 pixel to left and below character
//          21 Characters wide x 8 tall
// Size 2 = 12 wide x 16 tall.
//          10 Characters wide x 4 tall
#define HEIGHT 32
#define WIDTH 128
#define SIZE1HEIGHT 8
#define SIZE1WIDTH 6

// I2C Comms

// SHOULD BE THE SAME ON MASTER AND CLIENT
EasyTransferI2C menuResponseET;
EasyTransferI2C menuRequestET;

struct MENU_REQUEST_STRUCTURE{
  unsigned int i2cAddress;
  unsigned int oledAddress;
  unsigned int index;
};

struct MENU_RESPONSE_STRUCTURE{
  unsigned int i2cAddress;
  unsigned int oledAddress;
  unsigned int index;
  char menuItem[12];
  boolean showStars;
};

MENU_REQUEST_STRUCTURE menuRequestData;
MENU_RESPONSE_STRUCTURE menuResponseData;
boolean showStars = false;
// SHOULD BE THE SAME ON MASTER AND CLIENT

#define I2C_MASTER_ADDRESS 0
#define ADDRESS_PIN0 2
#define ADDRESS_PIN1 3
#define ADDRESS_PIN2 4

uint8_t i2cSlaveAddress = 0;

void receive(int numBytes) {}
// I2C Comms

#define BUTTON_COUNT 1
uint8_t buttonThreshold = 2;
uint8_t buttonsBase[BUTTON_COUNT]; // Base analog value to use (if its 12 for down, then its 12/2 for up)
uint8_t buttonsRead[BUTTON_COUNT]; // Latest reading
uint8_t buttonsLast[BUTTON_COUNT]; // Reading previous time
uint8_t buttonsPressed[BUTTON_COUNT]; // The last button pressed
unsigned long lastButtonCheck = 0;
unsigned long lastRequest1 = 0, lastRequest2 = 0, lastRequest3 = 0, lastRequest4 = 0;

uint8_t buttonPressed(uint8_t analogPin)
{
 uint8_t returnVal = 0;
 uint8_t base = buttonsBase[analogPin];
 uint8_t value = buttonsRead[analogPin];
 
 if (value <= buttonThreshold)
   returnVal = 0;
 else if (((base - buttonThreshold) <= value) && (value <= (base + buttonThreshold)))
   returnVal = 1; // UP?
 else if (((base/2 - buttonThreshold) <= value) && (value <= (base/2 + buttonThreshold)))
   returnVal = 2; // Down?
 return returnVal;
}

uint8_t menuIndexes[BUTTON_COUNT];

void setup()   {                
  Serial.begin(9600);
  randomSeed(analogRead(5));
  
  // Find 3 pins to use as address bits
  // read said pins and convert to int
  pinMode(ADDRESS_PIN0, INPUT);
  pinMode(ADDRESS_PIN1, INPUT);
  pinMode(ADDRESS_PIN2, INPUT);
  i2cSlaveAddress |= digitalRead(ADDRESS_PIN0) << 0;
  i2cSlaveAddress |= digitalRead(ADDRESS_PIN1) << 1;
  i2cSlaveAddress |= digitalRead(ADDRESS_PIN2) << 2;
  Serial.print(i2cSlaveAddress);
  Serial.println(" - Slave");
  
  Wire.begin(i2cSlaveAddress);
  menuResponseET.begin(details(menuResponseData), &Wire);
  menuRequestET.begin(details(menuRequestData), &Wire);
  Wire.onReceive(receive);
  
  // by default, we'll generate the high voltage from the 3.3v line internally! (neat!)
  display.begin(SSD1306_SWITCHCAPVCC);
  display2.begin(SSD1306_SWITCHCAPVCC);
  display3.begin(SSD1306_SWITCHCAPVCC);
  display4.begin(SSD1306_SWITCHCAPVCC);
  
  buttonsBase[0] = 29;
  //buttonsBase[1] = 12;
  //buttonsBase[2] = 12;
  //buttonsBase[4] = 12;
 
  display.display(); // show splashscreen
  //delay(500);
  display.clearDisplay();   // clears the screen and buffer
  display.setTextColor(WHITE);
  ShowOneLineCentered(2, "60Works", 7);
  
  display2.display();
  display2.clearDisplay();
  display2.setTextColor(WHITE);
  ShowOneLineCentered2(2, "60Works", 7);
  
  display3.display();
  display3.clearDisplay();
  display3.setTextColor(WHITE);
  ShowOneLineCentered3(2, "60Works", 7);
  
  display4.display();
  display4.clearDisplay();
  display4.setTextColor(WHITE);
  ShowOneLineCentered4(2, "60Works", 7);
  delay(3000);
}

void loop() {
  /*
  if (millis() > lastButtonCheck + 50)
  {
    for (int i = 0; i < BUTTON_COUNT; i++)
    {
      buttonsRead[i] = analogRead(i);
      
      if (buttonsRead[i] != 0)
      {
        Serial.print("button ");
        Serial.print(i);
        Serial.print(" : ");
        Serial.println(buttonsRead[i]);
      }
    }
    for (int i = 0; i < BUTTON_COUNT; i++)
    {
      uint8_t pressed = buttonPressed(i);
      if (pressed != 0 && pressed != buttonsPressed[i])
      {
        buttonsLast[i] = buttonsRead[i];
        if(pressed == 1)
        {
          menuIndexes[i] = menuIndexes[i] - 1;
        }
        else
        {
          menuIndexes[i] = menuIndexes[i] + 1;
        }
        
        menuRequestData.i2cAddress = i2cSlaveAddress;
        menuRequestData.oledAddress = i + 1;
        menuRequestData.index = menuIndexes[i];
  
        menuRequestET.sendData(I2C_MASTER_ADDRESS);
        
        Serial.print("A");
        Serial.print(i);
        Serial.print(": ");
        Serial.print(buttonsRead[i]);
        Serial.print(" ");
        if (pressed == 1)
          Serial.print("UP");
        else
          Serial.print("DOWN");
        Serial.print(" Index: ");
        Serial.println(menuIndexes[i]);  
        
      }
      buttonsPressed[i] = pressed;
    }
    lastButtonCheck = millis();
  }
  */
  // Simulate button presses for oled #1
  if (millis() > lastRequest1 + random(5000))
  {
    menuRequestData.i2cAddress = i2cSlaveAddress;
    menuRequestData.oledAddress = 1;
    menuRequestData.index = random(3);
    
    Serial.print(menuRequestData.i2cAddress);
    Serial.print("-");
    Serial.print(menuRequestData.oledAddress);
    Serial.print("-");
    Serial.println(menuRequestData.index);
   
    menuRequestET.sendData(I2C_MASTER_ADDRESS);
    lastRequest1 = millis();
    delay(500);
  }
  
  // Simulate button presses for oled #2
  if (millis() > lastRequest2 + random(5000))
  {
    menuRequestData.i2cAddress = i2cSlaveAddress;
    menuRequestData.oledAddress = 2;
    menuRequestData.index = random(8);
    
    Serial.print(menuRequestData.i2cAddress);
    Serial.print("-");
    Serial.print(menuRequestData.oledAddress);
    Serial.print("-");
    Serial.println(menuRequestData.index);
   
    menuRequestET.sendData(I2C_MASTER_ADDRESS);
    lastRequest2 = millis();
    delay(500);
  }
    // Simulate button presses for oled #3
  if (millis() > lastRequest3 + random(5000))
  {
    menuRequestData.i2cAddress = i2cSlaveAddress;
    menuRequestData.oledAddress = 3;
    menuRequestData.index = random(8);
   
    Serial.print(menuRequestData.i2cAddress);
    Serial.print("-");
    Serial.print(menuRequestData.oledAddress);
    Serial.print("-");
    Serial.println(menuRequestData.index);
    
    menuRequestET.sendData(I2C_MASTER_ADDRESS);
    lastRequest3 = millis();
    delay(500);
  }
    // Simulate button presses for oled #4
  if (millis() > lastRequest4 + random(5000))
  {
    menuRequestData.i2cAddress = i2cSlaveAddress;
    menuRequestData.oledAddress = 4;
    menuRequestData.index = random(8);
    
    Serial.print(menuRequestData.i2cAddress);
    Serial.print("-");
    Serial.print(menuRequestData.oledAddress);
    Serial.print("-");
    Serial.println(menuRequestData.index);
   
    menuRequestET.sendData(I2C_MASTER_ADDRESS);
    lastRequest4 = millis();
    delay(500);
  }
  
  // Not sure we need the loop
  int responseLoop = 0;
  for (responseLoop = 0; responseLoop < 2 ; responseLoop++)
  {
  //check and see if a data packet has come in. 
  if(menuResponseET.receiveData()){
    Serial.print(menuResponseData.i2cAddress);
    Serial.print("-");
    Serial.print(menuResponseData.oledAddress);
    Serial.print("-");
    Serial.print(menuResponseData.index);
    Serial.print(": ");
    Serial.print(menuResponseData.showStars);
    Serial.print(" ");
    Serial.println(menuResponseData.menuItem);
    
    if(menuResponseData.menuItem[0] != '\0')
    {
      display.setTextColor(WHITE);
      int j = 0;
      while (menuResponseData.menuItem[j] != '\0')
        j++;
      
      if (menuResponseData.oledAddress == 1)
        ShowOneLineCentered(2, menuResponseData.menuItem, j);
      else if (menuResponseData.oledAddress == 2)
        ShowOneLineCentered2(2, menuResponseData.menuItem, j);
      else if (menuResponseData.oledAddress == 3)
        ShowOneLineCentered3(2, menuResponseData.menuItem, j);
      else if (menuResponseData.oledAddress == 4)
        ShowOneLineCentered4(2, menuResponseData.menuItem, j);
    }
    else
    {
      Serial.print("index adjusted ");
      
      if (menuIndexes[menuResponseData.oledAddress - 1] == 255)
        menuIndexes[menuResponseData.oledAddress - 1] = 0;
      else
        menuIndexes[menuResponseData.oledAddress - 1] = menuResponseData.index - 1;
    }
  }
  }
  unsigned long temp = millis();
  if (temp % 1000 == 0)
  {
    Serial.println(temp/1000);
    delay(1);
  }
}

