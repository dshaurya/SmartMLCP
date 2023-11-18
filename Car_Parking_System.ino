// Viral Science www.viralsciencecreativity.com www.youtube.com/c/viralscience
// Arduino Car Parking System

#include <Wire.h>
#include <LiquidCrystal_I2C.h>
LiquidCrystal_I2C lcd(0x27, 16, 2);  //Change the HEX address
#include <Servo.h>

Servo boomBarrier;

int inboundIR = 2;   // Inbound to Car Parking
int outboundIR = 4;  // Outbound from Car parking

int totalAvailSlots = 2;  //Enter Total number of parking Slots
int pos = 0;              // variable to store the servo position

int inboundFlag = 0;
int outboundFlag = 0;

void setup() {

  Serial.begin(9600);

  pinMode(inboundIR, INPUT);
  pinMode(outboundIR, INPUT);

  boomBarrier.attach(3);
  // boomBarrier.write(90);  // Barrier is Closed

  lcd.init();
  lcd.begin(2, 16);
  lcd.backlight();
  lcd.setCursor(0, 0);
  lcd.print(" SMART MLCP");
  lcd.setCursor(0, 1);
  lcd.print(" BY SHAURYA D");
  delay(10000);
  lcd.clear();
}

void closeBarrier() {
  for (pos = 0; pos <= 100; pos += 1) {  // goes from 0 degrees to 180 degrees
                                         // in steps of 1 degree
    boomBarrier.write(pos);              // tell servo to go to position in variable 'pos'
    delay(15);                           // waits 15ms for the servo to reach the position
  }
}

void openBarrier() {
  for (pos = 100; pos >= 0; pos -= 1) {  // goes from 180 degrees to 0 degrees
    boomBarrier.write(pos);              // tell servo to go to position in variable 'pos'
    delay(15);                           // waits 15ms for the servo to reach the position
  }
}
void loop() {

  if (digitalRead(inboundIR) == LOW && inboundFlag == 0) {
    if (totalAvailSlots > 0) {
      inboundFlag = 1;
      if (outboundFlag == 0)
      if  {
        // boomBarrier.write(0);                  // Open the Barrier
        openBarrier();
        totalAvailSlots = totalAvailSlots - 1;
      }


      Serial.println("Inside Inbound");
      Serial.print("Inbound Flag =");
      Serial.print(inboundFlag);
      Serial.print("\n");
      Serial.println("Outbound Flag = ");
      Serial.print(outboundFlag);
      Serial.print("\n");

    } else {
      lcd.setCursor(0, 0);
      lcd.print("    SORRY :)    ");
      lcd.setCursor(0, 1);
      lcd.print("  Parking Full  ");
      delay(5000);
      lcd.clear();
    }
  }

  if (digitalRead(outboundIR) == LOW && outboundFlag == 0) {
    outboundFlag = 1;
    if (inboundFlag == 0) {
      boomBarrier.write(0);  // Open the Barrier
      totalAvailSlots = totalAvailSlots + 1;
    }
    Serial.println("Inside Outbound");
    Serial.print("Inbound Flag =");
    Serial.print(inboundFlag);
    Serial.print("\n");
    Serial.print("Outbound Flag = ");
    Serial.print(outboundFlag);
    Serial.print("\n");
  }

  if (inboundFlag == 1 && outboundFlag == 1) {
    Serial.println("Inside Reset");
    Serial.println("Inbound Flag =");
    Serial.print(inboundFlag);
    Serial.print("\n");
    Serial.println("Outbound Flag = ");
    Serial.print(outboundFlag);
    Serial.print("\n");

    // delay(1000);
    //  boomBarrier.write(90);  // Close the barrier
    closeBarrier();
    inboundFlag = 0, outboundFlag = 0;

    Serial.println("Inside Reset - after closing");
    Serial.println("Inbound Flag =");
    Serial.print(inboundFlag);
    Serial.print("\n");
    Serial.println("Outbound Flag = ");
    Serial.print(outboundFlag);
    Serial.print("\n");
  }

  lcd.setCursor(0, 0);
  lcd.print("    WELCOME!    ");
  lcd.setCursor(0, 1);
  lcd.print("Park at  ");
  lcd.print(totalAvailSlots);
}
