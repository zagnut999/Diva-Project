
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


