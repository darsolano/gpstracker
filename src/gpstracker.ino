#include <TinyGPS++.h>
#include <SoftwareSerial.h>
#include <SD.h>

static const int RXPin = 3, TXPin = 4;
static const uint32_t GPSBaud = 9600;
const int chipSelect = 10;

TinyGPSPlus gps;
SoftwareSerial ss(RXPin, TXPin);

void setup()
{
  pinMode(13, OUTPUT);
  pinMode(10, OUTPUT);
  Serial.begin(115200);
  ss.begin(GPSBaud);
  
  if (!SD.begin(chipSelect)) {
    return;
  }
}

void loop()
{
  while (ss.available() > 0) {
    gps.encode(ss.read());
  }
  
  if (gps.location.isValid() && gps.location.isUpdated()) {
    File logFile = SD.open("location.csv", FILE_WRITE);
    if (logFile) {
      logFile.print(gps.location.lat(), 7);
      logFile.print(";");
      logFile.print(gps.location.lng(), 7);
      logFile.print(";");
      logFile.print(gps.altitude.meters(), 2);
      logFile.print(";");
      logFile.print(gps.satellites.value());
      logFile.print(";");
      logFile.print(gps.hdop.value());
      logFile.print(";");
      logFile.print(gps.course.deg(), 2);
      logFile.print(";");
      logFile.print(gps.speed.kmph(), 2);
      logFile.print(";");
      char sz[32];
      sprintf(sz, "%02d.%02d.%02d;", gps.date.day(), gps.date.day(), gps.date.year());
      logFile.print(sz);
      sprintf(sz, "%02d:%02d:%02d", gps.time.hour(), gps.time.minute(), gps.time.second());
      logFile.println(sz);
      logFile.close();
    }
      
    Serial.print(gps.location.lat(), 7);
    Serial.print(";");
    Serial.print(gps.location.lng(), 7);
  }
  else {
    // do nothing
  }
}


