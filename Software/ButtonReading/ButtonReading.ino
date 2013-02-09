#define B_OLED_1 A0

void setup()   {                
  Serial.begin(9600);
  digitalWrite(B_OLED_1, HIGH); // enable the 20k internal pullup
  
  pinMode(B_OLED_1, INPUT);
}

void loop() {
  int c = 0;
  c = analogRead(B_OLED_1);
  if (c > 10)
  {
    Serial.print(millis());
    
    Serial.print(":");
    Serial.print(c);
    
    Serial.print(": ");
    if (c < 512)
      Serial.println("Up");
    else
     Serial.println("Down");
  }
}
