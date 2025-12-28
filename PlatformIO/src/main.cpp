#include <Arduino.h>
#include <ThreeWire.h>
#include <RtcDS1302.h>

// declare function 
ThreeWire myWire(7, 6, 8); // DAT, CLK, RST
RtcDS1302<ThreeWire> Rtc(myWire);
void printDateTime(const RtcDateTime& dt);

void setup() {
  Serial.begin(57600);

  Serial.print("RTC DS1302 Test-compiled:");
  Serial.print(__DATE__);
  Serial.println(__TIME__);

  Rtc.Begin();

  RtcDateTime compiled = RtcDateTime(__DATE__, __TIME__);
  printDateTime(compiled);
  Serial.println();

  if (!Rtc.IsDateTimeValid()) 
  {
    Serial.println("RTC lost confidence in the DateTime!");
    Rtc.SetDateTime(compiled);
  }

  if (Rtc.GetIsWriteProtected()) 
  {
    Serial.println("RTC was write protected, disabling write protection");
    Rtc.SetIsWriteProtected(false);
  }

  if (!Rtc.GetIsRunning()) 
  {
    Serial.println("RTC was not running, starting");
    Rtc.SetIsRunning(true);
  }

  RtcDateTime now = Rtc.GetDateTime();
  if (now < compiled) 
  {
    Serial.println("RTC time is older than compile time!  needs update");
    Rtc.SetDateTime(compiled);
  }

  if (now > compiled) 
  {
    Serial.println("RTC time is newer than compile time, not updating");
  }

  if (now == compiled)
  {
    Serial.println("RTC time is same with compile time");
  }
}

void loop() {
  RtcDateTime now = Rtc.GetDateTime();

  printDateTime(now);
  Serial.println();

  if (!now.IsValid())
  {
    Serial.println("The DateTime retrieved from the RTC is not valid!");
  }

  delay(1000);
}

#define countof(a) (sizeof(a) / sizeof(a[0]))

void printDateTime(const RtcDateTime& dt)
{
  char datestring[20];

  snprintf(datestring, 
           countof(datestring),
           "%02u/%02u/%04u %02u:%02u:%02u",
           dt.Month(),
           dt.Day(),
           dt.Year(),
           dt.Hour(),
           dt.Minute(),
           dt.Second() );
  Serial.print(datestring);
}
