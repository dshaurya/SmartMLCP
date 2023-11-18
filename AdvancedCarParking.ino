// Arduino Advanced Car Parking System

#include <Wire.h>
#include <LiquidCrystal_I2C.h>
LiquidCrystal_I2C lcd(0x27, 16, 2);  //Change the HEX address
#include <Servo.h>

Servo boomBarrier;

int inboundIR = 2;   // Inbound to Car Parking
int outboundIR = 4;  // Outbound from Car parking

int inboundFlag = 0;
int outboundFlag = 0;

int irPinCount = 8;
int irPinsArray[8] = {
  5, 6, 7, 12, 8, 9, 10, 11  // Parking Slots IR pins
};                           // Change this based on your wiring

int parkingSlots = 8;
bool parkingSlotsStatus[8] = {
  true, true, true, true, true, true, true, true  // Initially All Slots are available
};

void closeBarrier() {

  for (int pos = 0; pos <= 100; pos += 1) {  // goes from 0 degrees to 180 degrees
                                             // in steps of 1 degree
    boomBarrier.write(pos);                  // tell servo to go to position in variable 'pos'
    delay(15);                               // waits 15ms for the servo to reach the position
  }
}

void openBarrier() {
  for (int pos = 100; pos >= 0; pos -= 1) {  // goes from 180 degrees to 0 degrees
    boomBarrier.write(pos);                  // tell servo to go to position in variable 'pos'
    delay(15);                               // waits 15ms for the servo to reach the position
  }
}

int getFreeParkingSlot() {
  int freeSlot = -1;
  Serial.println("Finding Free Slots ... ");
  for (int pos = 0; pos < parkingSlots; pos++) {  // goes through all slots
    if (parkingSlotsStatus[pos] == true) {
      freeSlot = pos + 1;
      break;
    }
  }
  Serial.print("Found Slot ..  ");
  Serial.println(freeSlot);

  return freeSlot;
}


void setup() {

  Serial.begin(9600);

  // the array elements are numbered from 0 to (irPinCount - 1).
  // use a for loop to initialize each pin as an input:

  for (int thisPin = 0; thisPin < irPinCount; thisPin++) {
    pinMode(irPinsArray[thisPin], INPUT);
  }


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



void loop() {
  int myfreeslot = getFreeParkingSlot();
  if (myfreeslot > 0) {
    lcd.setCursor(0, 0);
    lcd.print("WELCOME!");
    lcd.setCursor(0, 1);
    lcd.print("Park at  ");
    lcd.print(myfreeslot);
    //delay(5000);
  } else {
    lcd.setCursor(0, 0);
    lcd.print("Slots are full");
    lcd.setCursor(0, 1);
    lcd.print("Sorry :(  ");
    delay(5000);
    //lcd.clear();
  }

  if (digitalRead(inboundIR) == LOW && inboundFlag == 0) {  // Inbound IR pin
    if (myfreeslot > 0) {
      inboundFlag = 1;
      if (outboundFlag == 0) {
        // Open the Barrier
        openBarrier();
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
    }
  }

  if (digitalRead(outboundIR) == LOW && outboundFlag == 0) {  // OutBound IR pin
    outboundFlag = 1;
    if (inboundFlag == 0) {
      boomBarrier.write(0);  // Open the Barrier
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

    // Close the barrier
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

  // Park in available free slot
  for (int pos = 0; pos < parkingSlots; pos++) {
    if (digitalRead(irPinsArray[pos]) == LOW) {
      Serial.print("Parking at  = ");
      Serial.println(pos);
      parkingSlotsStatus[pos] = false;  // Occupy the Parking Slot
    }
    if (digitalRead(irPinsArray[pos]) == HIGH) {
      parkingSlotsStatus[pos] = true;  // Free the Parking Slot
      Serial.print("Leaving from  = ");
      Serial.println(pos);
    }
  }
}