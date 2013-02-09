#include <SD.h>
#include <Wire.h>
#include <EasyTransferI2C.h>

#define SD_CS 10
#define SD_CD 9
#define SHOW_STARS_PIN 7
#define CARD_PIN 5

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

// SHOULD BE THE SAME ON MASTER AND CLIENT

struct READ_LINE
{
    unsigned int i2cAddress;
    unsigned int oledAddress;
    unsigned int index;
    char menuItem[12];
};

void setup()   {                
  Serial.begin(9600);
  Serial.println("0 - Master");
    
  pinMode(SD_CD, INPUT);
  pinMode(SHOW_STARS_PIN, INPUT);
  pinMode(CARD_PIN, OUTPUT);
  pinMode(10, OUTPUT); //SD library fails if this isn't set
  
  /*
  while (1)
  {
    Serial.println(digitalRead(SD_CD) ? "HIGH" : "LOW");
    delay(500); 
  }
  */
  /*
  int isCardNotFound = 1;
  while(isCardNotFound)
  {
    Serial.println(digitalRead(SD_CD));
    isCardNotFound = digitalRead(SD_CD);
    digitalWrite(CARD_PIN, isCardNotFound);
    if (isCardNotFound)
      Serial.println("Card NOT found!!!!");
    else
      Serial.println("Card found");
    delay(10);
  }
  */
  
  if (!SD.begin(SD_CS)) {
      Serial.println(F("SD initialization failed!"));
      //return line;
  }
  
  // Search term
  unsigned int i2cAddress = 1;
  unsigned int oledAddress = 3;
  unsigned int index = 4;
  
  struct READ_LINE tempLine = findMenuItem(i2cAddress, oledAddress, index);
  
  Serial.println(tempLine.index); //TODO VERIFY THIS IS CORRECT!!!!
  if (tempLine.menuItem[0] != '\0')
      Serial.println(tempLine.menuItem);
  else
      Serial.println("NOT FOUND");
  
  // Start communication
  Wire.begin(0);
  menuResponseET.begin(details(menuResponseData), &Wire);
  menuRequestET.begin(details(menuRequestData), &Wire);
  
  //define handler function on receiving data
  Wire.onReceive(receive);
  
  sendMessageToAll(true);
  
}

boolean showStars = false;
boolean lastShowStarsRead = false;
boolean lastShowStarsSent = false;
unsigned long lastShowStarsCheck = 0;
unsigned long registeredSlaves = 0;

void loop() {
  // Do we need to send a "ShowStars" update.
  /*if (millis() > lastShowStarsCheck + 100)
  {
    boolean showStarsRead = digitalRead(SHOW_STARS_PIN);
    //Serial.print("ShowStars: ");Serial.print(showStarsRead);
    //Serial.print(" Last:");Serial.println(lastShowStarsRead);
    
    // only do this if the button was just pressed
    if (showStarsRead == true && lastShowStarsRead == false)
    {
      showStars = !showStars;
      if (showStars != lastShowStarsSent)
      {
        //Send stars message to all slaves
        for (int i = 0; i < 32; i++)
        {
          unsigned long temp = 1 << i;
          if (registeredSlaves && temp == temp)
          {
            //Send them the message
            menuResponseData.i2cAddress = i;
            menuResponseData.oledAddress = 0;
            menuResponseData.index = 0;
            menuResponseData.showStars = showStars;
            menuResponseData.menuItem[0] = '\0';
            menuResponseET.sendData(i);
          }
        }
        lastShowStarsSent = showStars;
      }
      Serial.print("ShowStars:");
      Serial.println(showStars);
    }
    lastShowStarsRead = showStarsRead;
    lastShowStarsCheck = millis();
  }
  */
  // Any requests received?
  if (menuRequestET.receiveData())
  {
    struct READ_LINE tempLine = findMenuItem(menuRequestData.i2cAddress, menuRequestData.oledAddress, menuRequestData.index);
    Serial.print(menuRequestData.i2cAddress);
    Serial.print("-");
    Serial.print(menuRequestData.oledAddress);
    Serial.print("-");
    Serial.println(menuRequestData.index);
    Serial.print(tempLine.i2cAddress);
    Serial.print("-");
    Serial.print(tempLine.oledAddress);
    Serial.print("-");
    Serial.print(tempLine.index);
    Serial.print(": ");
    Serial.println(tempLine.menuItem);
    if (tempLine.menuItem[0] == '\0')
        Serial.println("MENU ITEM NOT FOUND");
    
    registeredSlaves = registeredSlaves || 1 << tempLine.i2cAddress;
    
    menuResponseData.i2cAddress = tempLine.i2cAddress;
    menuResponseData.oledAddress = tempLine.oledAddress;
    menuResponseData.index = tempLine.index; //TODO verify
    menuResponseData.showStars = showStars;
    int counter = 0;
    for (counter = 0; counter < 12; counter++)
      menuResponseData.menuItem[counter] = tempLine.menuItem[counter];
    menuResponseET.sendData(tempLine.i2cAddress);
    delay(5);
  }
  
  unsigned long temp = millis();
  if (temp % 1000 == 0)
  {
    Serial.println(temp/1000);
    delay(1);
  }
}

void sendMessageToAll(boolean allPossible){
  //Send stars message to all slaves
  for (int i = 0; i < 32; i++)
  {
    unsigned long temp = 1 << i;
    if (registeredSlaves && (temp == temp || allPossible))
    {
      //Send them the message
      menuResponseData.i2cAddress = i;
      menuResponseData.oledAddress = 0;
      menuResponseData.index = 0;
      menuResponseData.showStars = showStars;
      menuResponseData.menuItem[0] = '\0';
      menuResponseET.sendData(i);
    }
  }
}


void receive(int numBytes) {
  /*
  // TODO WHAT HAPPENS IF THERE ARE MANY REQUESTS AT ONCE??? 
  // maybe move this loop? and use if(menuRequestET.receiveData())
  menuRequestET.receiveData();
  // TODO Log the request from this i2c address so we can push data to known arduinos. 
  //   Use an unsigned long and assume the arduinos are less than 31.
  struct READ_LINE tempLine = findMenuItem(menuRequestData.i2cAddress, menuRequestData.oledAddress, menuRequestData.index);
  
  Serial.println(tempLine.index); //TODO VERIFY THIS IS CORRECT!!!!
  if (tempLine.menuItem[0] != '\0')
      Serial.println(tempLine.menuItem);
  else
      Serial.println("NOT FOUND");
  
  menuResponseData.i2cAddress = tempLine.i2cAddress;
  menuResponseData.oledAddress = tempLine.oledAddress;
  menuResponseData.index = tempLine.index; //TODO verify
  menuResponseData.showStars = showStars;
  int counter = 0;
  for (counter = 0; counter < 12; counter++)
    menuResponseData.menuItem[counter] = tempLine.menuItem[counter];
  
  menuResponseET.sendData(tempLine.i2cAddress);
  */
}



