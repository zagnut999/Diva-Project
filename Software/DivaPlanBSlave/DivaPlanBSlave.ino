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
boolean haveMenusFinishedLoading = false;
EasyTransferI2C ET; 
struct SEND_DATA_STRUCTURE{
  char subLocation[3]; //which display on the slave are we adding to
  char menuItem[12]; //what text should be added
  boolean hasFinishedLoading;
};

SEND_DATA_STRUCTURE menuData;

#define I2C_SLAVE_ADDRESS 1
void receive(int numBytes) {}
// I2C Comms

// Defining the max(?) number of menus a board could have.
String menu01 = "";
String menu02 = "";
String menu03 = "";
String menu04 = "";

void setup()   {                
  Serial.begin(9600);
  Serial.println("asdf");
  Wire.begin(I2C_SLAVE_ADDRESS);
  //start the library, pass in the data details and the name of the serial port. Can be Serial, Serial1, Serial2, etc. 
  ET.begin(details(menuData), &Wire);
  //define handler function on receiving data
  Wire.onReceive(receive);
  
  // by default, we'll generate the high voltage from the 3.3v line internally! (neat!)
  display.begin(SSD1306_SWITCHCAPVCC);
  // init done
  
  display.display(); // show splashscreen
  //delay(500);
  display.clearDisplay();   // clears the screen and buffer
}

void loop() {
  //check and see if a data packet has come in. 
  if(ET.receiveData()){
    Serial.print("RX: ");
    Serial.print(menuData.subLocation);
    Serial.print(" ");
    Serial.print(menuData.menuItem);
    Serial.print(" ");
    Serial.println(menuData.hasFinishedLoading);
    
    /*if (String(menuData.subLocation) == "01")
    {
      if (menu01 != "")
        menu01 += "|";
      menu01 += menuData.menuItem;
    }
    else */
    if (String(menuData.subLocation) == "02")
    {
      if (menu02 != "")
        menu02 += "|";
      menu02 += menuData.menuItem;
    }
    
    
    //Serial.println();
    //Serial.println("-----------");
  
    //Serial.print("01 - ");
    //Serial.println(menu01);
    
    Serial.print("02 - ");
    Serial.println(menu02);
    /*
    //Serial.print("03 - ");
    //Serial.println(menu03);
    
    //Serial.print("04 - ");
    //Serial.println(menu04);
    */
  }
  
  //if (haveMenusFinishedLoading)
    //ShowCurrentWithScroll();
}

