/*
* Name: övningsprojekt
* Author: Filip Momot
* Date: 2025-10-14
* Description: This project uses a ds3231 to measure time and displays the time to an 1306 oled display, 
* Further, it measures temprature with ds3231 and displays a mapped value to a 9g-servo-motor.
*/

// Include Libraries
#include <RTClib.h>
#include <Wire.h>
#include <U8glib.h>
#include <Servo.h>
#include <LedControl.h>

U8GLIB_SSD1306_128X64 u8g(U8G_I2C_OPT_NO_ACK);

// Init constants


// Init global variables
int pin = 9;
int pos = 0;
int digitPos = 3;
LedControl lc=LedControl(12,13,10,1);
// Construct objects
RTC_DS3231 rtc;
Servo myservo;
void setup() {
  // init communication
  Serial.begin(9600);
  Wire.begin();

  // Init Hardware
  rtc.begin();
   myservo.attach(pin);
  // Settings
  rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
  
  lc.shutdown(0,false);
  /* Set the brightness to a medium values */
  lc.setIntensity(0,8);
  /* and clear the display */
  lc.clearDisplay(0);
}

void loop() {
  oledWrite( getTime() ); 
  servoWrite( getTemp() ); 
  writeTimeOn7Segment( getTime() );
  Serial.println(getTime());

  delay(200);
}



//This function reads time from an ds3231 module and package the time as a String
//Parameters: Void
//Returns: time in hh:mm:ss as String
String getTime() {
  DateTime now = rtc.now();
  return String(now.hour()) + ":" + String(now.minute()) + ":" + String(now.second());
}

/*
* This function takes temprature from ds3231 and returns as a float
*Parameters: Void
*Returns: temprature as float 
*/
float getTemp() {
  return rtc.getTemperature();
}

/*
* This function takes a string and draws it to an oled display
*Parameters: - text: String to write to display
*Returns: void
*/
void oledWrite(String text) {
  u8g.firstPage();
  do {  
    u8g.setFont(u8g_font_6x10); 
    u8g.drawStr(0, 15, text.c_str());
  } while (u8g.nextPage());
}

/*
* takes a temprature value and maps it to corresppnding degree on a servo
*Parameters: - value: temprature
*Returns: void
*/
void servoWrite(float value) {
  pos = map((int)(getTemp() * 10), 0, 280, 0, 180); //mappar funktion för omvandla tempertur värden till grader på servon 
  pos = constrain(pos, 0, 180);
  myservo.write(pos);

}

/* 
*Tar tim och minut siffrorna och printar de på seven segment displayen
*Parametrar: Tim och min (hour, min)
*Returnerar: void
*/

void writeTimeOn7Segment(String text) {
 lc.clearDisplay(0); 
 DateTime now = rtc.now();

  int hour = now.hour();
  int min = now.minute();

  lc.setDigit(0, 3, hour / 10, false); //detta anger positionen, adressen, siffran och om decimaltecknet ska tändas   
  lc.setDigit(0, 2, hour % 10, true);     
  lc.setDigit(0, 1, min / 10, false);    
  lc.setDigit(0, 0, min % 10, false);
}

