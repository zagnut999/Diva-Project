#include <Wire.h>
#include <EasyTransferI2C.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define OLED_DC 12	// 11  //MISO
#define OLED_CS 7	// 12  
#define OLED_CLK 13	// 10  // CLK
#define OLED_MOSI 11	// 9   //MOSI
#define OLED_RESET 6	// 13
Adafruit_SSD1306 display(OLED_MOSI, OLED_CLK, OLED_DC, OLED_RESET, OLED_CS);

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
unsigned long lastRequest = 0;
void receive(int numBytes) {}
// I2C Comms

void setup()   {                
  Serial.begin(9600);
  randomSeed(analogRead(0));
  
  // Find 3 pins to use as address bits
  // read said pins and convert to int
  pinMode(ADDRESS_PIN0, INPUT);
  pinMode(ADDRESS_PIN1, INPUT);
  pinMode(ADDRESS_PIN2, INPUT);
  i2cSlaveAddress |= digitalRead(ADDRESS_PIN0) << 0;
  i2cSlaveAddress |= digitalRead(ADDRESS_PIN1) << 1;
  i2cSlaveAddress |= digitalRead(ADDRESS_PIN2) << 2;
  Serial.println(i2cSlaveAddress);
  
  Wire.begin(i2cSlaveAddress);
  menuResponseET.begin(details(menuResponseData), &Wire);
  menuRequestET.begin(details(menuRequestData), &Wire);
  Wire.onReceive(receive);
  
  // by default, we'll generate the high voltage from the 3.3v line internally! (neat!)
  display.begin(SSD1306_SWITCHCAPVCC);
  // init done
  
  display.display(); // show splashscreen
  //delay(500);
  display.clearDisplay();   // clears the screen and buffer
  display.setTextColor(WHITE);
  ShowOneLineCentered(2, "60Works", 7);
  delay(3000);
}

void loop() {
  if (millis() > lastRequest + 1000)
  {
    Serial.println("sending");
    menuRequestData.i2cAddress = i2cSlaveAddress;
    menuRequestData.oledAddress = 1;
    menuRequestData.index = random(4);
    
    menuRequestET.sendData(I2C_MASTER_ADDRESS);
    lastRequest = millis();
  }
  
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
      int i = 0;
      while (menuResponseData.menuItem[i] != '\0')
        i++;
        
      ShowOneLineCentered(2, menuResponseData.menuItem, i);
    }
  }
  unsigned long temp = millis();
  if (temp % 1000 == 0)
  {
    Serial.println(temp/1000);
    delay(1);
  }
  //if (haveMenusFinishedLoading)
    //ShowCurrentWithScroll();
}

