

/**
 * 
 * Used for setting the time on the DS3231 RTC module, since that isn't a task that needs to be
 * done very often.
 * 
 */




// ***************************************
// ********** Global Variables ***********
// ***************************************

//Globals for Wifi Setup and OTA
#include <credentials.h>
#include <ESP8266WiFi.h>
#include <ESP8266mDNS.h>
#include <WiFiUdp.h>
#include <RTClib.h>
#include <Wire.h>
#include <NTPClient.h>
#include <TimeLib.h>

RTC_DS3231 rtc;
WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP, "us.pool.ntp.org", 16200, 60000);

//WiFi Credentials
#ifndef STASSID
#define STASSID "your_ssid"
#endif
#ifndef STAPSK
#define STAPSK  "your_password"
#endif
const char* ssid = STASSID;
const char* password = STAPSK;

char t[32];
byte last_second, second_, minute_, hour_, day_, month_;
int year_;




// ***************************************
// *************** Setup *****************
// ***************************************


void setup() {

  Serial.begin(115200);  // Initialize serial communication with a baud rate of 9600
  Wire.begin();  // Begin I2C communication
  rtc.begin();  // Initialize DS3231 RTC module

  // Connect to WiFi
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.print(".");
  }
  Serial.println("Connected to WiFi");

  timeClient.begin();  // Start NTP client
  timeClient.update();  // Retrieve current epoch time from NTP server
  unsigned long unix_epoch = timeClient.getEpochTime();  // Get epoch time

  DateTime now = rtc.now();  // Get initial time from RTC
  last_second = now.second();  // Store initial second

  rtc.adjust(DateTime(unix_epoch));  // Set RTC time using NTP epoch time

}




// ***************************************
// ************* Da Loop *****************
// ***************************************


void loop() {

  timeClient.update();  // Update time from NTP server
  unsigned long unix_epoch = timeClient.getEpochTime();  // Get current epoch time

  DateTime rtcTime = rtc.now();  // Get current time from RTC

  // Format and print RTC time on Serial monitor
  sprintf(t, "%02d:%02d:%02d %02d/%02d/%02d", (rtcTime.hour() + 15) % 24, (rtcTime.minute() + 30) % 60, rtcTime.second(), rtcTime.day(), rtcTime.month(), rtcTime.year());
  Serial.println(t);

  last_second = second_;  // Update last second

  delay(1000);  // Delay for 1 second before the next iteration
}
