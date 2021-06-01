#include "SD.h"
#include <Wire.h>
#include <RTClib.h>

RTC_DS1307 RTC;

#define LOG_INTERVAL  10000 // mills between entries.
#define SYNC_INTERVAL 1000 // mills between calls to flush() - to write data to the card
#define ECHO_TO_SERIAL   1 // echo data to serial port.

int sensorValue;

uint32_t syncTime = 0; // time of last sync()
//RTC_DS1307 RTC; // define the Real Time Clock object
// for the data logging shield, we use digital pin 10 for the SD cs line
const int chipSelect = 10;
// the logging file
File logfile;

void setup()
{
  Serial.begin(9600);
  
  initSDcard();

  createFile();

  initRTC();

  logfile.println("millis,stamp,datetime,soilmoisture");
#if ECHO_TO_SERIAL
  Serial.println("millis,stamp,datetime,soilmoisture");
#endif //ECHO_TO_SERIAL
}

void loop()
{
  DateTime now;

  delay((LOG_INTERVAL - 1) - (millis() % LOG_INTERVAL));

  // log milliseconds since starting
  uint32_t m = millis();
  logfile.print(m);           // milliseconds since start
  logfile.print(", ");
#if ECHO_TO_SERAL
  Serial.print(m);         // milliseconds since start
  Serial.print(", ");
#endif

  // fetch the time
  now = RTC.now();
  // log time
  logfile.print(now.unixtime()); // seconds since 2000
  logfile.print(", ");
  logfile.print(now.year(), DEC);
  logfile.print("/");
  logfile.print(now.month(), DEC);
  logfile.print("/");
  logfile.print(now.day(), DEC);
  logfile.print(" ");
  logfile.print(now.hour(), DEC);
  logfile.print(":");
  logfile.print(now.minute(), DEC);
  logfile.print(":");
  logfile.print(now.second(), DEC);
#if ECHO_TO_SERIAL
  Serial.print(now.unixtime()); // seconds since 2000
  Serial.print(", ");
  Serial.print(now.year(), DEC);
  Serial.print("/");
  Serial.print(now.month(), DEC);
  Serial.print("/");
  Serial.print(now.day(), DEC);
  Serial.print(" ");
  Serial.print(now.hour(), DEC);
  Serial.print(":");
  Serial.print(now.minute(), DEC);
  Serial.print(":");
  Serial.print(now.second(), DEC);
#endif //ECHO_TO_SERIAL

 sensorValue = analogRead(0);
 
 if (sensorValue >= 700)
 {
     Serial.print(sensorValue);
     Serial.write(" IN WATER ");
     delay(1000); 
 }

 if (sensorValue <= 300)
 {
     Serial.print(sensorValue);
     Serial.write(" SOIL DRY ");
     delay(1000);
 }

#if ECHO_TO_SERIAL
  Serial.println(sensorValue, DEC);


#endif //ECHO_TO_SERIAL
  logfile.print(", ");
  logfile.println(sensorValue, DEC);

  if ((millis() - syncTime) < SYNC_INTERVAL) return;
  syncTime = millis();

  logfile.flush();

 

  digitalWrite(LED_BUILTIN, LOW);
  delay(2000);
}

void error(char const *str)
{
  Serial.print("error: ");
  Serial.println(str);

  while (1);
}

void initSDcard()
{
  Serial.print("Initializing SD card...");
  // make sure that the default chip select pin is set to
  // output, even if you don't use it:
  pinMode(chipSelect, OUTPUT);

  // see if the card is present and can be initialized:
  if (!SD.begin(chipSelect)) {
    Serial.println("Card failed, or not present");
    // don't do anything more:
    return;
  }
  Serial.println("card initialized.");

}

/**Creates a new CSV file to log to.  If using for another sensor, remember to modify the for loop corresponding to the characters in the file name to reflect the desired name for the sensor logged data.  Has a maximum limit of 99 logs.
*/
void createFile()
{
  //file name must be in 8.3 format (name length at most 8 characters, follwed by a '.' and then a three character extension.
  char filename[] = "MYLOG00.CSV";
  for (uint8_t i = 0; i < 100; i++) {
    filename[5] = i / 10 + '0';
    filename[6] = i % 10 + '0';
    if (! SD.exists(filename)) {
      // only open a new file if it doesn't exist
      logfile = SD.open(filename, FILE_WRITE);
      break;  // leave the loop!
    }
  }

  if (! logfile) {
    error("couldnt create file");
  }

  Serial.print("Logging to: ");
  Serial.println(filename);
}

void initRTC()
{
  Wire.begin();
  if (!RTC.begin()) {
    logfile.println("RTC failed");
#if ECHO_TO_SERIAL
    Serial.println("RTC failed");
#endif  //ECHO_TO_SERIAL
  }
}
