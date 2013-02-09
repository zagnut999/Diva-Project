#include <SD.h>
#include <Wire.h>
#include <EasyTransferI2C.h>

#define SD_CS 10

EasyTransferI2C ET;

struct SEND_DATA_STRUCTURE{
  char subLocation[3]; //which display on the slave are we adding to
  char menuItem[12]; //what text should be added
  boolean hasFinishedLoading;
};

SEND_DATA_STRUCTURE menuData;

String menu = "";

void setup()   {                
  Serial.begin(9600);
  Wire.begin();
  ET.begin(details(menuData), &Wire);
  
  pinMode(10, OUTPUT); //SD library fails if this isn't set
  if (!SD.begin(SD_CS)) {
    Serial.println("SD initialization failed!");
    return;
  } else {
    // open the file for reading:
    File menuFile = SD.open("menu.txt");
    if (menuFile) {
      Serial.println("menu.txt:");
      
      boolean skipLine = false;
      char temp = 0,prev = '\n';
      String line = "";
      
      // read from the file until there's nothing else in it:
      while (menuFile.available()) {
        temp = menuFile.read();
        if (prev == '\n' && !(temp == '@' || temp == '-')) {
          skipLine = true;
        }

        if (temp =='\n') {
          if (line != "") {
            AddMenuItem(line);
            //Serial.println(line);
            line = "";
          }
          skipLine = false;
        }
        else if (!skipLine) {
          line.concat(temp);
        }
        prev = temp;
      }
      // close the file:
      menuFile.close();
      if (menu != "") {
        SendMenu();
        delay(10000);
      }
    } else {
      // if the file didn't open, print an error:
      Serial.println("SD: error opening menu.txt");
    }
  }
}

void loop() {

}

void AddMenuItem(String text) {
  if (menu != "" && text.startsWith("@")) {
    SendMenu();
    menu = text.substring(1);
  } else {
    if (menu == ""){ //first menu set
      menu = text.substring(1);
    } else {
      menu += "|" + text.substring(1);
    }
  }
}

void SendMenu() {
  Serial.println(menu);
  int current = menu.indexOf('|');
  int previous = 0;
  
  String location = menu.substring(previous, current - 1);
  char slaveAddress[4];
  location.substring(0,2).toCharArray(slaveAddress, 4);
  location.substring(2).toCharArray(menuData.subLocation, 3);
  menuData.hasFinishedLoading = false;
  unsigned long i2cSlaveAddress = atol(slaveAddress);
  Serial.print(i2cSlaveAddress);
  Serial.print(" ");
  Serial.println(menuData.subLocation);
  //Loop through rest of text.
  previous = current;
  current = menu.indexOf('|', previous + 1);
  while (previous != -1){
    String item = "";
    if (current == -1)
      item += menu.substring(previous + 1);
    else
      item += menu.substring(previous + 1, current - 1);
    
    item.toCharArray(menuData.menuItem, 12);
    
    ET.sendData(i2cSlaveAddress);
    Serial.println(item);
    
    previous = current;
    current = menu.indexOf('|', previous + 1);
  }
}

