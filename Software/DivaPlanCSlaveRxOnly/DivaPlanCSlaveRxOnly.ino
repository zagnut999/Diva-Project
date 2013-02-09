#include <Wire.h>
#include <EasyTransferI2C.h>

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

//define slave i2c address
#define I2C_SLAVE_ADDRESS 1
#define I2C_MASTER_ADDRESS 0
#define ADDRESS_PIN0 2
#define ADDRESS_PIN1 3
#define ADDRESS_PIN2 4

unsigned int i2cSlaveAddress = 0;
unsigned long lastRequest = 0;

void setup(){
  Serial.begin(9600);
  randomSeed(analogRead(0));
  
  Wire.begin(I2C_SLAVE_ADDRESS);
  menuResponseET.begin(details(menuResponseData), &Wire);
  menuRequestET.begin(details(menuRequestData), &Wire);
  Wire.onReceive(receive);
  
  pinMode(13, OUTPUT);
  
  // Find 3 pins to use as address bits
  // read said pins and convert to int
  /*pinMode(ADDRESS_PIN0, INPUT);
  pinMode(ADDRESS_PIN1, INPUT);
  pinMode(ADDRESS_PIN2, INPUT);
  i2cSlaveAddress |= digitalRead(ADDRESS_PIN0) << 0;
  i2cSlaveAddress |= digitalRead(ADDRESS_PIN1) << 1;
  i2cSlaveAddress |= digitalRead(ADDRESS_PIN2) << 2;
  Serial.println(i2cSlaveAddress);*/
}

void loop() {
  if (millis() > lastRequest + 1000)
  {
    Serial.println("sending");
    menuRequestData.i2cAddress = I2C_SLAVE_ADDRESS;
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
  }
  unsigned long temp = millis();
  if (temp % 1000 == 0)
  {
    Serial.println(temp/1000);
    delay(1);
  }
}

void receive(int numBytes) {}
