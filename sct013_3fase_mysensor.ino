#include "EmonLib.h"                   // Include Emon Library
#include <Wire.h>
#include <LiquidCrystal_I2C.h>

EnergyMonitor emon1;                   // Create an instance for current sensor
EnergyMonitor emon2;  
EnergyMonitor emon3; 
#define MY_DEBUG
#define MY_RADIO_RF24
#define MY_RF24_CE_PIN 9
#define MY_RF24_CS_PIN 8
#define MY_SPLASH_SCREEN_DISABLED
#define MY_NODE_ID 9

#include <MySensors.h> 
#define CHILD_ID 0  
#define CHILD_ID2 1  
#define CHILD_ID3 2  

unsigned long lastSend;
unsigned long SEND_FREQUENCY = 10000; // Minimum time between send (in milliseconds). 

MyMessage realPowerMsg1(CHILD_ID,V_WATT);

MyMessage realPowerMsg2(CHILD_ID2,V_WATT);

MyMessage realPowerMsg3(CHILD_ID3,V_WATT);


LiquidCrystal_I2C lcd(0x3f, 2, 1, 0, 4, 5, 6, 7, 3, POSITIVE); // Set the LCD I2C address
void setup()
{  

  Serial.begin(115200);
  Serial.print("Hello");
 
  emon1.current(0, 30);             // Current: input pin, calibration.
  emon2.current(1, 30);
  emon3.current(2, 30);
  


  delay(1000);
  
  lcd.begin(16,2); // initialize the lcd for 16 chars 2 lines and turn on backlight
  lcd.setCursor(2,0);
  lcd.print("KAASFABRIEK");
  delay(1000);
  lcd.setCursor(1,1); //Start at character 2 on line 0
  lcd.print("3 fase monitor");
  delay(2000);
  lcd.clear();

}

void presentation()  {
  // Send the sketch version information to the gateway and Controller
  sendSketchInfo("3 phase meter", "1.1");

  // Tell the MySensors gateway what kind of sensors this node has, and what their ID's on the node are, as defined in the code above.
  present(CHILD_ID, S_POWER);
  present(CHILD_ID2, S_POWER);
  present(CHILD_ID3, S_POWER);
}
 
void loop()
{
  unsigned long now = millis();
  
  double Irms1 = emon1.calcIrms(1480);  // Calculate Irms only
  double Irms2 = emon2.calcIrms(1480);
  double Irms3 = emon3.calcIrms(1480);
  
  bool sendTime = now - lastSend > SEND_FREQUENCY;
  if (sendTime) { 

    float realPower1       = (Irms1*230.0);        //extract Real Power into variable
    float realPower2       = (Irms2*230.0);        //extract Real Power into variable
    float realPower3       = (Irms3*230.0);        //extract Real Power into variable
//#ifdef MY_DEBUG  
  lcd.clear();
  Serial.print(realPower1);         // Apparent power
  lcd.setCursor(0,0);
  lcd.print(realPower1);
  Serial.print(" ");
  Serial.print(Irms1);          // Irms
  Serial.print(" ");
  Serial.print(realPower2);         // Apparent power
  lcd.setCursor(8,0);
  lcd.print(realPower2);
  Serial.print(" ");
  Serial.print(Irms2);          // Irms
  Serial.print(" ");
  Serial.print(realPower3);         // Apparent power
  lcd.setCursor(0,1);
  lcd.print(realPower3);
  Serial.print(" ");
  Serial.println(Irms3);          // Irms
  lcd.setCursor(8,1);
  lcd.print((Irms1+Irms2+Irms3)*230.0);
//#endif
    if(realPower1 > 0 && realPower1 < 10000){
    send(realPowerMsg1.set(realPower1,1));
    }
    if(realPower2 > 0 && realPower2 < 10000){
    send(realPowerMsg2.set(realPower2,1));
    }
    if(realPower3 > 0 && realPower3 < 10000){
    send(realPowerMsg3.set(realPower3,1));
    }
    lastSend = now;
  } 
}
