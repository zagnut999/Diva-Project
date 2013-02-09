#include <Wire.h>
#include <EasyTransferI2C.h>

//create object
EasyTransferI2C ET; 

struct SEND_DATA_STRUCTURE{
  char subLocation[3]; //which display on the slave are we adding to
  char menuItem[12]; //what text should be added
  boolean hasFinishedLoading;
};

SEND_DATA_STRUCTURE menuData;

//define slave i2c address
#define I2C_SLAVE_ADDRESS 1

void setup(){
  Serial.begin(9600);
  Wire.begin(I2C_SLAVE_ADDRESS);
  //start the library, pass in the data details and the name of the serial port. Can be Serial, Serial1, Serial2, etc. 
  ET.begin(details(menuData), &Wire);
  //define handler function on receiving data
  Wire.onReceive(receive);
  
  pinMode(13, OUTPUT);
  
}

void loop() {
  //check and see if a data packet has come in. 
  if(ET.receiveData()){
    Serial.print(menuData.subLocation);
    Serial.print(" ");
    Serial.println(menuData.menuItem);
  }
}

void receive(int numBytes) {}
