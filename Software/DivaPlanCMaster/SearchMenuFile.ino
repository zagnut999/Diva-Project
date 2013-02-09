
#define ADDRESS '@'
#define MENUITEM ':'

struct READ_LINE findMenuItem(unsigned int i2cAddress, unsigned int oledAddress, unsigned int index)
{
    // Search function
    struct READ_LINE line;
    line.i2cAddress = 0;
    line.oledAddress = 0;
    line.menuItem[0] = '\0';
    line.index = 0;
    
    File inputFile = SD.open("menu.txt");
    
    if (inputFile == 0)
    {
        Serial.println(F("Error: Cannot open file menu.txt"));
        return line;
    }
    
    char temp;
    bool isNewLine = true;
    bool isAddress = false;
    bool isMenuItem = false;
    unsigned int menuItemLength = 0;
    
    while (inputFile.available()) 
    {
        temp = inputFile.read();
        if (temp == '\n') {
            isNewLine = true;
            continue;
        }
        
        if (isNewLine)
        {
            isAddress = temp == ADDRESS;
            isMenuItem = temp == MENUITEM;
        }
        
        // Skip comments and other empty lines
        if (!(isMenuItem || isAddress))
            continue;
        
        // Check to see if we are done (only breaks here if last menu item is the requested index)
        if (isAddress && line.oledAddress != 0)
            break; //we're done.
        
        // Check to see if we are at the correct menu, if not, skip to next menu.
        if (isAddress)
        {
            //reset structure
            line.i2cAddress = 0;
            line.oledAddress = 0;
            line.menuItem[0] = '\0';
            line.index = 0;
            menuItemLength = 0;
            
            //read two chars, convert to int, check to see if i2caddress
            char i2cAddressTemp[3];
            i2cAddressTemp[0] = inputFile.read();
            i2cAddressTemp[1] = inputFile.read();
            i2cAddressTemp[2] = '\0';
            
            line.i2cAddress = atoi(i2cAddressTemp);
            if (line.i2cAddress != i2cAddress) {
                line.i2cAddress = 0;
                isMenuItem = false;
                isAddress = false;
                continue;
            }
            
            //if i2caddress, read 2 chars, convert to int, check if oledaddress
            char oledAddressTemp[3];
            oledAddressTemp[0] = inputFile.read();
            oledAddressTemp[1] = inputFile.read();
            oledAddressTemp[2] = '\0';
            
            line.oledAddress = atoi(oledAddressTemp);
            if (line.oledAddress != oledAddress) {
                line.i2cAddress = 0;
                line.oledAddress = 0;
                isMenuItem = false;
                isAddress = false;
                continue;
            }
            
            // finish off this line...
            continue;
        }
        
        // We are in the correct menu.. look for index
        if (isMenuItem && line.oledAddress != 0) {
            if (line.index != index) {
                isMenuItem = false;
                line.index++;
                continue;
            }
            
            if (temp == MENUITEM && isNewLine && line.menuItem[0] != '\0')
            {
                break; // we're done
            }
            
            if (temp != MENUITEM)
            {
                line.menuItem[menuItemLength] = temp;
                line.menuItem[menuItemLength + 1] = '\0';
                menuItemLength++;
            }
        }
        
        isNewLine = false;
    }
    
    inputFile.close();
    
    return line;
}
