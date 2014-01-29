/* shOWTime
 * Thomas Phillips
 * November 9, 2012
 *
 * Contains code from WiiChuckDemo --  
 * 2008 Tod E. Kurt, http://thingm.com/
 *
 */

#include <Wire.h>
#include "nunchuck_funcs.h"
#include <LiquidCrystal.h>
#include <avr/sleep.h>

unsigned int accx, accy, accz; 
byte zbut, cbut;

int loop_cnt = 0;
int endFlag = 1;
int sleepCounter = 0;
int firstX, firstY, firstZ, lastX, lastY, lastZ, force, time;
LiquidCrystal lcd(12, 11, 5, 4, 3, 2);

void setup()
{
  Serial.begin(19200);
  nunchuck_setpowerpins();
  nunchuck_init(); // send the initilization handshake
  time = 0;
  
  lcd.clear();
  lcd.begin(8, 2);
  
  // Display welcome message for 5 seconds
  char message[17] = "=== shOWTime ===";
  lcd.setCursor(0,0);
  lcd.write((byte*)message, 8);   
  lcd.setCursor(0,1);
  lcd.print(message+8);
  Serial.print("\n=== shOWTime ===\n");
  delay(5000);
  
  // 
  lcd.setCursor(0,1);
  char nextMessage[17] = "Hold buttons...";
  lcd.clear();
  lcd.write((byte*)nextMessage,8);
  lcd.setCursor(0,1);
  lcd.print(nextMessage+8);
}

void loop()
{    
  if( loop_cnt > 100 ) 
  { // every 100 msecs get new data
    loop_cnt = 0;

    // Retrieve data from nunchuk every 100ms
    nunchuck_get_data();

    // Data is filled in a buffer, retrieved with the nunchuck_*() functions
    zbut = nunchuck_zbutton();
    cbut = nunchuck_cbutton(); 

    accx  = nunchuck_accelx(); 
    accy  = nunchuck_accely(); 
    accz  = nunchuck_accelz(); 

    // Detect for when C and Z buttons are pressed together
    if ((1 == zbut) || (1 == cbut))
    {      
      if (endFlag)
      {
        firstX = accx;
        firstY = accy;
        firstZ = accz;
      }
   
      endFlag = 0;
      time++;
    }

    // Execution will stay in this loop while C and Z buttons are not pressed.
    // The code inside the else if will only execute if the endFlag value is 
    // equal to 1. Using this also allows the code to execute only once, as 
    // opposed to each iteration of the loop() function.
    else if ((0 == zbut) || (0 == cbut))
    {
      if (!endFlag)
      {
        lastX = accx;
        lastY = accy;
        lastZ = accz;
        force = (((abs(firstX - lastX))+(abs(firstY - lastY))+(abs(firstZ - lastZ)))/time);

        Serial.print("\n\n------------------------------------------------------\n");
        Serial.print("------------------------------------------------------\n\n");

        Serial.print("C and Z buttons were held for ");
        Serial.print(time);
        Serial.print(" ticks.\n");

        Serial.print("\tFirst\tLast\tDifference\nX\t");
        Serial.print(firstX);
        Serial.print("\t");
        Serial.print(lastX);
        Serial.print("\t");
        Serial.print(abs(firstX - lastX));
        Serial.print("\nY\t");
        Serial.print(firstY);
        Serial.print("\t");
        Serial.print(lastY);
        Serial.print("\t");
        Serial.print(abs(firstY - lastY));
        Serial.print("\nZ\t");
        Serial.print(firstZ);
        Serial.print("\t");
        Serial.print(lastZ);
        Serial.print("\t");
        Serial.print(abs(firstZ - lastZ));
        Serial.print("\n\nDerived Force: ");
        Serial.print(force);
        Serial.print("\n\n------------------------------------------------------\n");
        Serial.print("------------------------------------------------------\n\n");

        lcd.clear();
        lcd.print("Force = ");
        lcd.setCursor(0,1);
        lcd.print(force);


        time = 0;

        firstX = 0;
        firstY = 0;
        firstZ = 0;

        lastX = 0;
        lastY = 0;
        lastZ = 0;

        endFlag = 1;

      }
    }
  }

if(sleepCounter > 1000)
{
//    set_sleep_mode(SLEEP_MODE_PWR_DOWN);
}
  loop_cnt++;
  sleepCounter++;
  delay(1);

}




